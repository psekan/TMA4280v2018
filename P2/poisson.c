/**
 * C program to solve the two-dimensional Poisson equation on
 * a unit square using one-dimensional eigenvalue decompositions
 * and fast sine transforms.
 *
 * Einar M. Rønquist
 * NTNU, October 2000
 * Revised, October 2001
 * Revised by Eivind Fonn, February 2015
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

#define PI 3.14159265358979323846
#define true 1
#define false 0

typedef double real;
typedef int bool;

// Function prototypes
real *mk_1D_array(size_t n, bool zero);
real **mk_2D_array(size_t n1, size_t n2, bool zero);
void transpose(real **bt, real **b, size_t m, int *sizes, int *displacement, int* count, int numProcs, int rank);
real rhs(real x, real y);
real solution(real x, real y);

// Functions implemented in FORTRAN in fst.f and called from C.
// The trailing underscore comes from a convention for symbol names, called name
// mangling: if can differ with compilers.
void fst_(real *v, int *n, real *w, int *nn);
void fstinv_(real *v, int *n, real *w, int *nn);

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage:\n");
        printf("  poisson n\n\n");
        printf("Arguments:\n");
        printf("  n: the problem size (must be a power of 2)\n");
        return 1;
    }

    int numProcs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /*
     *  The equation is solved on a 2D structured grid and homogeneous Dirichlet
     *  conditions are applied on the boundary:
     *  - the number of grid points in each direction is n+1,
     *  - the number of degrees of freedom in each direction is m = n-1,
     *  - the mesh size is constant h = 1/n.
     */
    int n = atoi(argv[1]);
    int m = n - 1;
    real h = 1.0 / n;

    /*
     * Grid points are generated with constant mesh size on both x- and y-axis.
     */
    real *grid = mk_1D_array(n+1, false);
    for (size_t i = 0; i < n+1; i++) {
        grid[i] = i * h;
    }
    
    /*
     * Calculate number of rows of each process
     */
    int* sizes = calloc(numProcs, sizeof(int));
    int* displacement = calloc(numProcs, sizeof(int));
    int* count = calloc(numProcs, sizeof(int));
    int* globalPos = calloc(numProcs, sizeof(int));

    int leftProc = numProcs;
    int leftRows = m;
    int tmp;
    while(leftProc>0){
        tmp = leftRows/leftProc;
        if (leftRows%leftProc > 0) tmp++;
        sizes[numProcs-leftProc] = tmp;
        leftProc--;
        leftRows -= tmp;
     }
       
     globalPos[0] = 0;
     for (int i=1; i < numProcs; i++){
        globalPos[i] = globalPos[i-1] + sizes[i-1];
     } 

     int sum=0;
     for (int i=0; i<numProcs; i++){
        count[i] = sizes[rank]*sizes[i];
        displacement[i] = sum;
        sum += count[i]; 
     }
     
     for (int i=0; i < numProcs; i++){
        printf("i:%d --> %d\n", i sizes[i]);
     }

     for (int i=0; i < numProcs; i++){
        printf("i:%d --> %d\n", i globalPos[i]);
     }


    /*
     * The diagonal of the eigenvalue matrix of T is set with the eigenvalues
     * defined Chapter 9. page 93 of the Lecture Notes.
     * Note that the indexing starts from zero here, thus i+1.
     */
    real *diag = mk_1D_array(m, false);

    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < m; i++) {
        diag[i] = 2.0 * (1.0 - cos((i+1) * PI / n));
    }

    /*
     * Allocate the matrices b and bt which will be used for storing value of
     * G, \tilde G^T, \tilde U^T, U as described in Chapter 9. page 101.
     */
    real **b = mk_2D_array(m, m, false);
    real **bt = mk_2D_array(m, m, false);

    /*
     * This vector will holds coefficients of the Discrete Sine Transform (DST)
     * but also of the Fast Fourier Transform used in the FORTRAN code.
     * The storage size is set to nn = 4 * n, look at Chapter 9. pages 98-100:
     * - Fourier coefficients are complex so storage is used for the real part
     *   and the imaginary part.
     * - Fourier coefficients are defined for j = [[ - (n-1), + (n-1) ]] while 
     *   DST coefficients are defined for j [[ 0, n-1 ]].
     * As explained in the Lecture notes coefficients for positive j are stored
     * first.
     * The array is allocated once and passed as arguments to avoid doings 
     * reallocations at each function call.
     */
    int nn = 4 * n;
    int threads = omp_get_max_threads();
    real **z = mk_2D_array(threads, nn, false);

    /*
     * Initialize the right hand side data for a given rhs function.
     * Note that the right hand-side is set at nodes corresponding to degrees
     * of freedom, so it excludes the boundary (bug fixed by petterjf 2017).
     * 
     */
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < sizes[rank]; i++) {
        for (size_t j = 0; j < m; j++) {
            b[i + globalPos[rank]][j] = h * h * rhs(grid[i+1 + globalPos[rank]], grid[j+1]);
        }
    }

    real startTime = MPI_Wtime();

    /*
     * Compute \tilde G^T = S^-1 * (S * G)^T (Chapter 9. page 101 step 1)
     * Instead of using two matrix-matrix products the Discrete Sine Transform
     * (DST) is used.
     * The DST code is implemented in FORTRAN in fsf.f and can be called from C.
     * The array zz is used as storage for DST coefficients and internally for 
     * FFT coefficients in fst_ and fstinv_.
     * In functions fst_ and fst_inv_ coefficients are written back to the input 
     * array (first argument) so that the initial values are overwritten.
     */
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < sizes[rank]; i++) {
        fst_(b[i + globalPos[rank]], &n, z[omp_get_thread_num()], &nn);
    }

    transpose(bt, b, m, sizes, displacement, count, numProcs, rank);
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < sizes[rank]; i++) {
        fstinv_(bt[i + globalPos[rank]], &n, z[omp_get_thread_num()], &nn);
    }

    /*
     * Solve Lambda * \tilde U = \tilde G (Chapter 9. page 101 step 2)
     */
    #pragma omp paralle for schedule(static)
    for (size_t i = 0; i < sizes[rank]; i++) {
        for (size_t j = 0; j < m; j++) {
            bt[i][j] = bt[i][j] / (diag[i + globalPos[rank]] + diag[j]);
        }
    }

    /*
     * Compute U = S^-1 * (S * Utilde^T) (Chapter 9. page 101 step 3)
     */
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < sizes[rank]; i++) {
        fst_(bt[i + globalPos[rank]], &n, z[omp_get_thread_num()], &nn);
    }

    transpose(b, bt, m, sizes, displacement, count, numProcs, rank);

    #pragma omp paralle for schedule(static) 
    for (size_t i = 0; i < sizes[rank]; i++) {
        fstinv_(b[i + globalPos[rank]], &n, z[omp_get_thread_num()], &nn);
    }

    /*
     * Compute maximal value of solution for convergence analysis in L_\infty
     * norm.
     */
    double umax = 0.0;
    for (size_t i = 0; i < sizes[rank]; i++) {
        for (size_t j = 0; j < m; j++) {
            umax = umax > b[i + globalPos[rank]][j] ? umax : b[i + globalPos[rank]][j];
        }
    }

    /**
     * Compute maximal error value
     */
    double emax = 0.0, elocal;
    real x,y;
    for(size_t i = 0; i < sizes[rank]; i++){ 
        for(size_t j = 0; j < m; j++) {
            x = grid[i+1 + globalPos[rank]];
            y = grid[j+1];
            //printf("sol = %.15f, comp = %.15f\n", solution(x,y), b[i + globalPos[rank]][j]);
            elocal = fabs(solution(x, y) - b[i + globalPos[rank]][j]);
            emax = emax > elocal ?  emax : elocal;
        }
    }
    
    double totalTime = MPI_Wtime()-startTime;
    double t = totalTime;
    double max, error;
    MPI_Reduce(&t, &totalTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&emax, &error, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Allreduce(&umax, &max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0){
       printf("umax = %.15f\n", max);
       printf("max error = %.15f\n", error);
       printf("h^2 = %.15f\n", h*h);
       printf("MPI = %d\n", numProcs);
       printf("threads = %d\n", threads);
       printf("processors = %d\n", numProcs * threads);
       printf("average runtime = %.15f\n" , totalTime/numProcs);
       printf("max runtime = %.15f\n" , totalTime);
    }

    free(z);
    free(bt);
    free(b);
    free(sizes);
    free(displacement);
    free(count);
    free(grid);
    free(globalPos);
    free(diag);
    MPI_Finalize();

    return 0;
}

/*
 * This function is used for initializing the right-hand side of the equation.
 * Other functions can be defined to swtich between problem definitions.
 */

real rhs(real x, real y) {
    //return 2 * (y - y*y + x - x*x);
    //return 1;
    return 5*PI*PI*sin(PI*x)*sin(2*PI*y);
}

real w1(real x, real y) {
    if (x == 0 || x == 1) return 0;
    if (y == 0) return 0;
    return x*(x-1)/2;
}

real w2(real x, real y) {
    if (x == 0 || x == 1) return 0;
    if (y == 0) return x*(x-1)/2;
    return 0;
}

real solution(real x, real y) {
    //return x*(x-1)/2 - w1(x,y) - w2(x,y);
    return sin(PI*x)*sin(2*PI*y);
}

/*
 * Write the transpose of b a matrix of R^(m*m) in bt.
 * In parallel the function MPI_Alltoallv is used to map directly the entries
 * stored in the array to the block structure, using displacement arrays.
 */

void transpose(real **bt, real **b, size_t m, int* sizes, int* displacement, int* count, int numProcs, int rank)
{
    //for (size_t i = 0; i < m; i++) {
    //    for (size_t j = 0; j < m; j++) {
    //        bt[i][j] = b[j][i];
    //    }
    //}
    int buffLen = displacement[numProcs-1] + count[numProcs-1];
    double* sendbuff = (double*)malloc(buffLen * sizeof(double));
    double* recvbuff = (double*)malloc(buffLen * sizeof(double));
    int local = 0;
    int position = 0;
    for (int i = 0; i < numProcs; i++){
       for(int j = 0; j < sizes[rank]; j++){
           for(int k = 0; k < sizes[i]; k++){
               sendbuff[position++] = b[j][k+local];
           }
       }
       local += sizes[i]; 
    }

    MPI_Alltoallv(sendbuff,count, displacement, MPI_DOUBLE, recvbuff, count, displacement, MPI_DOUBLE, MPI_COMM_WORLD);

    position = 0;
    for (int i = 0; i < m; i++){
       for (int j = 0; j < sizes[rank]; j++){
          bt[j][i] = recvbuff[position++];
       }
    }  

    free(sendbuff);
    free(recvbuff);
}

/*
 * The allocation of a vectore of size n is done with just allocating an array.
 * The only thing to notice here is the use of calloc to zero the array.
 */

real *mk_1D_array(size_t n, bool zero)
{
    if (zero) {
        return (real *)calloc(n, sizeof(real));
    }
    return (real *)malloc(n * sizeof(real));
}

/*
 * The allocation of the two-dimensional array used for storing matrices is done
 * in the following way for a matrix in R^(n1*n2):
 * 1. an array of pointers is allocated, one pointer for each row,
 * 2. a 'flat' array of size n1*n2 is allocated to ensure that the memory space
 *   is contigusous,
 * 3. pointers are set for each row to the address of first element.
 */

real **mk_2D_array(size_t n1, size_t n2, bool zero)
{
    // 1
    real **ret = (real **)malloc(n1 * sizeof(real *));

    // 2
    if (zero) {
        ret[0] = (real *)calloc(n1 * n2, sizeof(real));
    }
    else {
        ret[0] = (real *)malloc(n1 * n2 * sizeof(real));
    }
    
    // 3
    for (size_t i = 1; i < n1; i++) {
        ret[i] = ret[i-1] + n2;
    }
    return ret;
}

args = commandArgs(TRUE)
data = read.csv2(args[1],colClasses=c("numeric","numeric"), stringsAsFactors=FALSE, dec=".")
png(args[2], 1920,1080)
barplot(data[[2]],names.arg=data[[1]], main=args[3], xlab =args[4], ylab=args[5])

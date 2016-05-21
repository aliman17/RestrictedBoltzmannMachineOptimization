#!/usr/bin/env Rscript

cur_dir <- getwd()

# Parse arguments
args = commandArgs(trailingOnly=TRUE)
if (length(args)<1) {
    current_dir <- "./X_gibbs/X_gibbs_baseline"
    next_dir <- "./X_gibbs/X_gibbs_unroll8_O3"
    img_name <- "test"
} else {
  current_dir <- args[1]
  next_dir <- args[2]
  img_name <- args[3]
}

peak_performance <- 16              # Set peak performance
old_peak_performance <- 4
#folder <- args[2]

# Set working directory to the folder with measuring data
library(ggplot2)
#setwd("~/Desktop/GIT/FastNumericalCode/rbm/update1")
#setwd(paste(current_dir, "/", folder, sep = ""))

y.axis.angle = 90

# Multiple plot function
#
# ggplot objects can be passed in ..., or to plotlist (as a list of ggplot objects)
# - cols:   Number of columns in layout
# - layout: A matrix specifying the layout. If present, 'cols' is ignored.
#
# If the layout is something like matrix(c(1,2,3,3), nrow=2, byrow=TRUE),
# then plot 1 will go in the upper left, 2 will go in the upper right, and
# 3 will go all the way across the bottom.
#


plot_perf <- function(name, n_koef, n_intercept, col){
  cod <- read.table(name)             # Load table
  n <- cod[,1]                        # Get n
  cycles <- cod[,2]                   # Get cycles
  flops <- n_koef * n + n_intercept   # Compute flops 
  perf <- flops / cycles              # Get performance
  perf_proc <- perf/peak_performance * 100
  data <- data.frame(x = n, y = perf_proc)
  return(geom_line(data = data, aes(x=x, y=y), col = col))

}


# Plot
p <- qplot(
    xlab = "Number of hidden layers", 
    ylab = "% of peak performance", geom = "path",
    ylim = c(0, 100)) +
    theme(axis.title.y = element_text(angle=y.axis.angle)) + 
    geom_hline(yintercept = 100, col = 2) +
    geom_hline(yintercept = old_peak_performance/peak_performance * 100 , col = "grey") +
    annotate("text", label = "vec. peak performance", x = 600, y = 98, size = 4, colour = "red") +
    annotate("text", label = "old peak performance", x = 600, y = 23, size = 4, colour = "grey")

counter <- 1

for(dir in c(current_dir, next_dir)){
    setwd(cur_dir) # reset
    setwd(dir)
    # Open config file
    config <- read.table("config") 
    
    #pdf(file='plot.pdf')
    for(i in 1:nrow(config)){
      name <- as.character(config[i, 1])
      
      # Check if the row is commented
      if (substring(name, 1, 1) == "#")
        next
      
      n_val <- as.numeric(config[i, 2])
      intercept <- as.numeric(config[i, 3])
      col = counter + 2
      line <- plot_perf(name, n_val, intercept, col)
      p <- p + line + annotate("text", label = paste("Input",counter), x = 200, y = (20*counter), size = 4, colour = col)
    }
    counter <- counter + 1
}
setwd(cur_dir) # reset
# Store plot
jpeg(paste("./img/00_", img_name, "_compare.jpeg", sep = ""))
print(p)
dev.off()
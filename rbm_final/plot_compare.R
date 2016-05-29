#!/usr/bin/env Rscript
# Parse arguments
args = commandArgs(trailingOnly=TRUE)

cur_dir <- getwd()

NAMES = c("Baseline", "W_update_O3", "W_update_O3_vectorized")
POSITION = c(0.7, 1.7, 4.5)
SELECTED_FUNCTION_NAME = "PERF_W_UPDATE"


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




# Plot
p <- qplot(
    xlab = "Number of hidden layers", 
    ylab = "% of peak performance", geom = "path") +
    theme(axis.title.y = element_text(angle=y.axis.angle)) + 
    geom_hline(yintercept = 16, col = 2) +
    geom_hline(yintercept = old_peak_performance , col = "grey") +
annotate("text", label = "vec. peak performance", x = 1600, y = 15.7, size = 4, colour = "red") +
annotate("text", label = "old peak performance", x = 1600, y = 3.8, size = 4, colour = "grey")


counter <- 1
img_name <- args[1]

while (counter < length(args)){
    dir = args[counter+1]
    print(dir)
    print(dir)
    setwd(cur_dir) # reset
    setwd(dir)
    # Open config file
    config <- read.table("config") 
    
    #pdf(file='plot.pdf')
    for(i in 1:nrow(config)){
      name <- as.character(config[i, 1])
      if (name == SELECTED_FUNCTION_NAME){
      
      # Check if the row is commented
      if (substring(name, 1, 1) == "#")
        next
      
      n_val <- as.numeric(config[i, 2])
      intercept <- as.numeric(config[i, 3])
      col = counter + 2
      cod <- read.table(name)             # Load table
      n <- cod[,1]                        # Get n
      cycles <- cod[,2]                   # Get cycles
      flops <- n_val * n + intercept   # Compute flops
      perf <- flops / cycles              # Get performance
      perf_proc <- perf#/peak_performance * 100
      data <- data.frame(x = n, y = perf_proc)
      line = geom_line(data = data, aes(x=x, y=y), col = col)
      p <- p + line + annotate("text", label = NAMES[counter], x = 1000, y = POSITION[counter], size = 6, colour = col)
    }
    }
    counter <- counter + 1
}
setwd(cur_dir) # reset
# Store plot
jpeg(paste("./img/00_", img_name, "_compare.jpeg", sep = ""))
print(p)
dev.off()
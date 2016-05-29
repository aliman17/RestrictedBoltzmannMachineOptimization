#!/usr/bin/env Rscript

# Parse arguments
args = commandArgs(trailingOnly=TRUE)
if (length(args)!=1) {
  setwd("~/Desktop/GIT/FastNumericalCode/rbm/baseline")
} else {
  current_dir <- args[1]  
  setwd(current_dir)
}

print("Start plotting ...")
library(ggplot2)

y.axis.angle = 90
ghz = 2.6
bandwidth = 25.6
peak_performance <- 16       
old_peak_performance <- 4
CACHE_BANDWIDTH <- 32

plot_perf <- function(name, n_koef, n_intercept){
  cod <- read.table(name)             # Load table
  n <- cod[,1]                        # Get n
  cycles <- cod[,2]                   # Get cycles
  flops <- n_koef * n + n_intercept   # Compute flops 
  perf <- flops / cycles              # Get performance
 
  perf_proc <- perf/peak_performance * 100

  # Plot
  p <- qplot(n, perf_proc, 
        xlab = "Number of hidden layers", 
        ylab = "% of peak performance", geom = "path",
        ylim = c(0, 100),
        main = paste("Cycles: ", floor(cycles))) +
    theme(axis.title.y = element_text(angle=y.axis.angle)) + 
    geom_hline(yintercept = 100, col = 2) +
    geom_hline(yintercept = 100 * (old_peak_performance/peak_performance) , col = "grey") +
    annotate("text", label = "vec. peak performance", x = 600, y = 98, size = 4, colour = "red") +
    annotate("text", label = "old peak performance", x = 600, y = 23, size = 4, colour = "grey") +
    annotate("text", label = substring(name, 6), x = 300, y = max(perf_proc)+10, size = 4, colour = "black")

  return(p)
}


plot_roofline <- function(name, n_koef, n_intercept, n_cost, intercept_cost, n_byte, intercept_byte, PI)
{
  cod <- read.table(name)             # Load table
  n <- cod[,1]                        # Get n
  cycles <- cod[,2]                   # Get cycles
  flops <- n_koef * n + n_intercept   # Compute flops 
  perf <- flops / cycles              # Get performance
  
  cost <- n_cost * n + intercept_cost
  byte <- n_byte * n + intercept_byte
  
  op.intensity <- cost/byte
  
  perf_proc <- perf

  # Plot
  p <- qplot((op.intensity), (perf_proc),
        xlab = "Operational Intensity",
        ylab = "Performance",
        #ylim = c(0, 100),
        main = paste("Cycles: ", floor(cycles))) +
        geom_text(aes(label=n))+
    geom_line(aes((c(1/20, peak_performance/PI)), (c(1/20 * PI, peak_performance))), col = 2) +
    theme(axis.title.y = element_text(angle=y.axis.angle)) + 
    geom_hline(yintercept = (peak_performance), col = 2) +
    geom_hline(yintercept = (old_peak_performance) , col = "grey") # +
    #annotate("text", label = "vec. peak performance", x = 300, y = 95, size = 4, colour = "red") +
    #annotate("text", label = "old peak performance", x = 300, y = 20, size = 4, colour = "red") +
    #annotate("text", label = substring(name, 6), x = 600, y = max(perf_proc)+10, size = 4, colour = "black")

  return(p)
}


# Open config file
config <- read.table("config") 

#pdf(file='plot.pdf')
for(i in 1:nrow(config)){
  name <- as.character(config[i, 1])
  
  # Check if the row is commented
  if (substring(name, 1, 1) == "#")
    next
  
  # Performance
  n_val <- as.numeric(config[i, 2])
  intercept <- as.numeric(config[i, 3])
  plot <- plot_perf(name, n_val, intercept)
  jpeg(paste("00", name, ".jpeg", sep = ""))
  print(plot)
  dev.off()

  # Roofline for memory
  if (ncol(config) > 3){
    n_cost <- as.numeric(config[i, 4])
    intercept_cost <- as.numeric(config[i, 5])
    n_byte <- as.numeric(config[i, 6])
    intercept_byte <- as.numeric(config[i, 7])
    PI <- bandwidth / ghz
    plot <- plot_roofline(name, n_val, intercept, n_cost, intercept_cost, n_byte, intercept_byte, PI)
    jpeg(paste("00", name, "_roofline.jpeg", sep = ""))
    print(plot)
    dev.off()
  }
  
  # Roofline for cache
  if (ncol(config) > 7){
    n_cost <- as.numeric(config[i, 4])
    intercept_cost <- as.numeric(config[i, 5])
    n_byte <- as.numeric(config[i, 8])
    intercept_byte <- as.numeric(config[i, 9])
    PI <- CACHE_BANDWIDTH
    plot <- plot_roofline(name, n_val, intercept, n_cost, intercept_cost, n_byte, intercept_byte, PI)
    jpeg(paste("00", name, "_roofline_cache.jpeg", sep = ""))
    print(plot)
    dev.off()
  }
}

print("Plot finished!")

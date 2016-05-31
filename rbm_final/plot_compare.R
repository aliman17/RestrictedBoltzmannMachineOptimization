#!/usr/bin/env Rscript

MAIN_DIR <- getwd()
peak_performance <- 16
old_peak_performance <- 4
library(ggplot2)
y.axis.angle = 90

get_base_plot <- function(name, y_lim){
  p <- qplot(
      main = name,
      ylim = y_lim,
      xlab = "Number of hidden layers [n]", 
      ylab = "Performance [flops/cycle]", geom = "path") +
      theme(axis.title.y = element_text(angle=y.axis.angle)) + 
      geom_hline(yintercept = 16, col = 2) +
  geom_hline(yintercept = 0, col = "black") +
  geom_vline(xintercept = 0, col = "black") +
      geom_hline(yintercept = old_peak_performance , col = "#dd0000", size = 0.8, alpha=1) +
  annotate("text", label = "vec. peak performance", x = 1600, y = 15.5, size = 6, colour = "red") +
  annotate("text", label = "scalar peak performance", x = 1600, y = 4.13, size = 5, colour = "#dd0000") + theme(text = element_text(size=20))
  return(p)
}


plot_compare <- function(SELECTED_FUNCTION_NAME, NAMES, POSITION, COLORS, DIRS, y_lim){
  p <- get_base_plot(SELECTED_FUNCTION_NAME, y_lim)
  counter <- length(DIRS)

  while (counter > 0){
      setwd(MAIN_DIR) # reset
      dir = DIRS[counter]
      print(dir)
      setwd(dir)
      # Open config file
      config <- read.table("config") 
      
      for(i in 1:nrow(config)){
        name <- as.character(config[i, 1])
        if (name == SELECTED_FUNCTION_NAME){
          # Check if the row is commented
          if (substring(name, 1, 1) == "#")
            next
          
          n_val <- as.numeric(config[i, 2])
          intercept <- as.numeric(config[i, 3])
          col = COLORS[counter]
          cod <- read.table(name)             # Load table
          n <- cod[,1]                        # Get n
          cycles <- cod[,2]                   # Get cycles
          flops <- n_val * n + intercept   # Compute flops
          perf <- flops / cycles              # Get performance
          perf_proc <- perf#/peak_performance * 100
          data <- data.frame(x = n, y = perf_proc)
          line <- geom_area(data = data, aes(x=x, y=y), col = col, alpha = 0.2, size=1) 
          #line <- line + geom_area(stat = "identity")
          p <- p + line + annotate("text", label = NAMES[counter], x = 1000, y = POSITION[counter], size = 6, colour = col)
        }
      }
      counter <- counter - 1
  }
  setwd(MAIN_DIR) # reset
  # Store plot
  jpeg(paste("./img/00_", SELECTED_FUNCTION_NAME, "_compare.jpeg", sep = ""))
  print(p)
  dev.off()
}






###################################################################
# W_update
###################################################################
SELECTED_FUNCTION_NAME = "PERF_W_UPDATE"
NAMES = c("Baseline", "O3 optimization", "Best scalar", "Best vectorized")
DIRS = c( "./baseline/",
          "./scalar/W_update_onlyO3/",
          "./scalar/W_update_unroll4_O3_fma",
          "./vectorized/W_update_unroll16_O3_fma_Vectorize_double/")
POSITION = c(0.3, 0.9, 1.8, 3.0)
COLORS = c("#003300", "#555555", "#dd0000", "#0000ff")
#SELECTED_FUNCTION_NAME = "PERF_W_UPDATE"
#NAMES = c("Baseline", "Best scalar", "Best vectorized")
#DIRS = c( "./baseline/",
#          "./scalar/W_update_unroll4_O3_fma",
#          "./vectorized/W_update_unroll16_O3_fma_Vectorize_double/")
#POSITION = c(0.3, 1.8, 3.0)
#COLORS = c("#003300", "#dd0000", "#0000ff")


plot_compare(SELECTED_FUNCTION_NAME, NAMES, POSITION, COLORS, DIRS, c(0, 6))


###################################################################
# Gibbs_X
###################################################################
SELECTED_FUNCTION_NAME = "PERF_GIBBS_X"
NAMES = c("Baseline", "W_update_O3", "Best scalar", "Best vectorized")
DIRS = c( "./baseline/",
          "./scalar/X_gibbs_O3/",
          "./scalar/X_gibbs_unroll8_O3/",
          "./vectorized/X_gibbs_unroll8_O3_Vectorize")
POSITION = c(0.12, 0.45, 1.2, 2)
COLORS = c("#003300", "#555555", "#dd0000", "#0000ff")

plot_compare(SELECTED_FUNCTION_NAME, NAMES, POSITION, COLORS, DIRS, c(0, 4))


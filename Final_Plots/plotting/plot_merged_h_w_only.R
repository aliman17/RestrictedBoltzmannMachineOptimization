#!/usr/bin/env Rscript

MAIN_DIR <- getwd()
peak_performance <- 16
old_peak_performance <- 4
library(ggplot2)
y.axis.angle = 90

get_base_plot <- function(name, y_lim){
  p <- qplot(
      main = name,
      #ylim = y_lim,
      xlab = "Number of hidden units [n]",
      ylab = "Cycles", geom = "path") +
      theme(axis.title.y = element_text(angle=y.axis.angle)) +
      theme(text = element_text(size=20))

      #geom_hline(yintercept = 16, col = 2) #+
      #geom_hline(yintercept = 0, col = "black") +
      #geom_vline(xintercept = 0, col = "black") +
      #geom_hline(yintercept = old_peak_performance , col = "#dd0000", size = 0.8, alpha=1) +
      #annotate("text", label = "vec. peak performance", x = 1600, y = 15.5, size = 6, colour = "red") +
      #annotate("text", label = "scalar peak performance", x = 1600, y = 4.13, size = 5, colour = "#dd0000")   return(p)
}


plot_compare <- function(NAMES, POSITION, COLORS, DIRS, y_lim){
    p <- get_base_plot("Vectorized W and H Update", y_lim)

      file1 = DIRS[1]
      file2 = DIRS[2]
      file3 = DIRS[3]

      # Open config file
      cod1 <- read.table(file1)
      cod2 <- read.table(file2)
      cod3 <- read.table(file3)
      
      n <- cod3[,1]                        # Get n
      cycles <- cod3[,2]                   # Get cycles
      
      cycles_combined <- cod1[, 2] + cod2[, 2]
          data1 <- data.frame(x = n, y = cycles)
          data2 <- data.frame(x = n, y = cycles_combined)
          p <- p + geom_line(data = data1, aes(x=x, y=y), col = COLORS[1], alpha = 1, size=1)
          p <- p + geom_line(data = data2, aes(x=x, y=y), col = COLORS[2], alpha = 1, size=1)
          p <- p + annotate("text", label = NAMES[1], x = 1000, y = POSITION[1], size = 6, colour = COLORS[1])
          p <- p + annotate("text", label = NAMES[2], x = 1000, y = POSITION[2], size = 6, colour = COLORS[2])

    p <- p + geom_vline(xintercept = 40, col = "black", linetype = "longdash") +
        geom_vline(xintercept = 400, col = "black", linetype = "longdash") +
        annotate("text", label = "L2", x = 120, y = 5500000, size = 6, colour = "#555555") +
        annotate("text", label = "L3", x = 470, y = 5500000, size = 6, colour = "#555555")
    setwd(MAIN_DIR) # reset
    # Store plot
    jpeg("../00_w_h_compare.jpeg")
    print(p)
    dev.off()
}






NAMES = c("H_W_fused_update", "Sum of H_update and W_update")
DIRS = c( "./h_w_merged/PERF_W_UPDATE",
          "./h_w_merged/PERF_H_UPDATE",
          "./h_w_merged/PERF_W_UPDATE_H_UPDATE")
POSITION = c(1000000, 5000000)
COLORS = c("#00aa00", "#0000ff")


plot_compare(NAMES, POSITION, COLORS, DIRS, c(0, 6))



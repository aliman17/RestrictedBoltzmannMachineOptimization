#!/usr/bin/env Rscript


library(ggplot2)

y.axis.angle = 90
ghz = 2.6
bandwidth = 25.6
peak_performance <- 16       
old_peak_performance <- 4
PI <- CACHE_BANDWIDTH <- 32



plot_roofline <- function(name, n_koef, n_intercept, n_cost, intercept_cost, n_byte, intercept_byte, PI)
{
    
    cod <- read.table(name)             # Load table
    sub <- 4:length(cod[,1])
    print(cod[4, 1])
    n <- cod[sub,1]                        # Get n
    cycles <- cod[sub,2]                   # Get cycles
    flops <- n_koef * n + n_intercept   # Compute flops
    perf <- flops / cycles              # Get performance
    
    cost <- n_cost * n + intercept_cost
    byte <- n_byte * n + intercept_byte
    
    op.intensity <- cost/byte
    
    perf_proc <- perf
    
    # Plot
    p <- qplot((op.intensity), (perf_proc),
    xlab = "Operational Intensity [flops/byte]",
    ylab = "Performance [flops/cycle]",
    ylim = c(0, 4),
    xlim = c(0, 0.43),
    main = "Vectorized Training Phase Roofline", geom="path") +
    geom_line(size = 1.4, col = "blue")+
    #geom_text(aes(label=n), col = "blue", size = 6)+
    geom_line(aes((c(0, peak_performance/PI/4)), (c(0, peak_performance/4))), col = 2) +
    geom_line(aes((c(0, 1/2/4)), (c(0, 1/2 * CACHE_BANDWIDTH/4))), col = "grey") +
    theme(axis.title.y = element_text(angle=y.axis.angle)) +
    geom_hline(yintercept = (peak_performance), col = 2) +
    geom_hline(yintercept = (old_peak_performance) , col = "red") +
    annotate("text", label = "vec. peak performance", x = 1, y = 15.7, size = 6, colour = "red") +
    annotate("text", label = "scalar peak performance", x = 0.12, y = 3.9, size = 7, colour = "red") +
    annotate("text", label = "memory bound", x = 0.3, y = 3.2, size = 7, colour = "red", angle = "45") +
    annotate("text", label = "cache bound", x = 0.1, y = 2.9, size = 7, colour = "grey", angle = "73")+
    
    #annotate("text", label = "2", x = 0.36, y = 1.15, size = 7, colour = "blue")+
    annotate("text", label = "10", x = 0.205, y = 1.6, size = 7, colour = "blue")+
    annotate("text", label = "300", x = 0.13, y = 3.0, size = 7, colour = "blue")+
    annotate("text", label = "800", x = 0.12, y = 1.55, size = 7, colour = "blue")+
    annotate("text", label = "1100", x = 0.12, y = 1.4, size = 7, colour = "blue")+
    annotate("text", label = "2000", x = 0.12, y = 1.3, size = 7, colour = "blue")+
    #annotate("text", label = "cache bound", x = 0.1, y = 2.9, size = 7, colour = "grey", angle = "73")+
    theme(text = element_text(size=20))
    
    #annotate("text", label = substring(name, 6), x = 600, y = max(perf_proc)+10, size = 4, colour = "black")
    
    return(p)
}




# Open config file
config <- read.table("./vector/merged/config")

#pdf(file='plot.pdf')
for(i in 1:nrow(config)){
  name <- as.character(config[i, 1])
  # Check if the row is commented
  if (substring(name, 1, 1) == "#")
    next

if (name == "PERF_COD_TRAIN_UPDATE"){
    n_val <- as.numeric(config[i, 2])
    intercept <- as.numeric(config[i, 3])
  n_cost <- as.numeric(config[i, 4])
  intercept_cost <- as.numeric(config[i, 5])
  n_byte <- as.numeric(config[i, 6])
  intercept_byte <- as.numeric(config[i, 7])
  PI <- bandwidth / ghz
  plot <- plot_roofline("./vector/merged/PERF_COD_TRAIN_UPDATE", n_val, intercept, n_cost, intercept_cost, n_byte, intercept_byte, PI)
  jpeg("../COD_roofline.jpeg")
  print(plot)
  dev.off()
  }
  }

print("Plot finished!")

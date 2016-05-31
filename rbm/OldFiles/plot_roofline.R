#!/usr/bin/env Rscript

# Parse arguments
args = commandArgs(trailingOnly=TRUE)
if (length(args)!=1) {
  setwd("~/Desktop/GIT/FastNumericalCode/rbm/baseline")
} else {
  current_dir <- args[1]  
  setwd(current_dir)
}

ghz = 2.6 #
bandwidth = 25.6 #G
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
multiplot <- function(..., plotlist=NULL, file, cols=1, layout=NULL) {
  library(grid)

  # Make a list from the ... arguments and plotlist
  plots <- c(list(...), plotlist)

  numPlots = length(plots)

  # If layout is NULL, then use 'cols' to determine layout
  if (is.null(layout)) {
    # Make the panel
    # ncol: Number of columns of plots
    # nrow: Number of rows needed, calculated from # of cols
    layout <- matrix(seq(1, cols * ceiling(numPlots/cols)),
                    ncol = cols, nrow = ceiling(numPlots/cols))
  }

 if (numPlots==1) {
    print(plots[[1]])

  } else {
    # Set up the page
    grid.newpage()
    pushViewport(viewport(layout = grid.layout(nrow(layout), ncol(layout))))

    # Make each plot, in the correct location
    for (i in 1:numPlots) {
      # Get the i,j matrix positions of the regions that contain this subplot
      matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))

      print(plots[[i]], vp = viewport(layout.pos.row = matchidx$row,
                                      layout.pos.col = matchidx$col))
    }
  }
}

plot_roofline <- function(name, n_koef, n_intercept, n_cost, intercept_cost, n_byte, intercept_byte)
{
  cod <- read.table(name)             # Load table
  n <- cod[,1]                        # Get n
  cycles <- cod[,2]                   # Get cycles
  flops <- n_koef * n + n_intercept   # Compute flops 
  perf <- flops / cycles              # Get performance
  
  cost <- n_cost * n + intercept_cost
  byte <- n_byte * n + intercept_byte
  
  op.intensity <- cost/byte
  PI <- bandwidth / ghz
  
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
  
  n_val <- as.numeric(config[i, 2])
  intercept <- as.numeric(config[i, 3])
  n_cost <- as.numeric(config[i, 4])
  intercept_cost <- as.numeric(config[i, 5])
  n_byte <- as.numeric(config[i, 6])
  intercept_byte <- as.numeric(config[i, 7])
  plot <- plot_roofline(name, n_val, intercept, n_cost, intercept_cost, n_byte, intercept_byte)
  #plots <- c(plot)
  # Store plot
  jpeg(paste("00", name, "_roofline.jpeg", sep = ""))
  print(plot)
  dev.off()
}
#multiplot(plotlist = plots, col = 2)
#dev.off()

# Optimization of Restricted Boltzmann Machines for Classification

### Abstract

Classification problems are one of the central tasks in machine learning. Due to its widespread use not only efficient algorithms are required, but also efficient implementations. Here we present an optimized implementation of restricted Boltzmann machines for classification of images of handwritten digits. We show how it is possible to increase the performance of a simple implementation by a factor of 7 for realistic values of the number of hidden units, and point out to the limiting factors that prevent higher levels of performance.

### Requirements
 
* gcc compiler 
* R (optional) 

If R is missing there won't be any plots but the benchmark will still be performed. 


### Run

* Open terminal and navigate into directory `rbm`
* Then run `RUNALL.sh <optimization folder> "<compiler flags>"`, where `<optimization directory>` represents folder with optimization and `<compiler flags>` compilation flags

Compiler flags can be also found in `<optimization folder>/compiler_flags`.
Examples:

* `RUNALL.sh ./baseline`
* `RUNALL.sh ./scalar/merged "-mfma -O3 -fno-tree-vectorize"`
* `RUNALL.sh ./vector/merged "-mfma -O3"`

### Result

The result are __performance plots__, __roofline plots__ and __cache roofline plots__ for functions specified in file `<optimization folder>/config`. Plots are stored in `<optimization folder>`.

### Details

#### Optimization

We invested significant time proportion to set up the structure for optimization which came after. We wanted to be like Latex, that we don't need to waste time on structure and collection of data but rather on the content - optimization. The goal was (1) optimize file (2) run benchmark and (3) take a look at the plots.

We designed a structure which holds the main program for benchmarking and benchmark plotting in the main directory. The optimizations are done in a subfolder where we also keep and store all neccessary data and informations connected to that particular optimization. The most important is rbm.h, the header file which is the copy of the baseline and then we use this file to optimize particular function or more functions. After we are satisfied with the optimization, we run `RUNALL.sh` in the main directory. The arguments are (1) path to the folder of the optimization, which contains `rbm.h`, and (2) compiler flags to be used for compilation. 

#### Config file

`config` is another neccessary file along `rbm.h` to be in the optimizational folder before we can run benchmarking. It tells to the main program, which functions should be benchmarked. Each line represents one function. Functions, that are not listed or commented by `#`, are ignored. `config` file contains another important information - floating point operations, memory transfer and cache transfer. This parameters are listed just after the function name. Sometimes number of floating point operations or cache/memory transfer depends on number of hidden units `n`. To solve this we need to pass numbers in pairs. First one represents coefficient in front of the `n` and the second one the intercept. There are 6 numbers in total, which are used for plotting performance and roofline for memory and cache. We can also omit cache transfer numbers or cache and memory transfer numbers. Then we simply won't get plots for this.

#### Plots

__Live plotting__ is another feature, which allows us to see the performance while benchmark is running. The benchmark computes for different n in range from `n=2` to `n=2000` and why would we wait until the end and waste time, when we can see much sooner if something goes wrong. Beside that, it's much easier to wait.

### Team
Wish you a great day Ales Omerzel, Sergio Solorzano and Prabhakaran Santhanam
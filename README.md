## Optimization of Restricted Boltzmann Machines for Classification

### Abstract

Classification problems are one of the central tasks in machine learning. Due to its widespread use not only efficient algorithms are required, but also efficient implementations. Here we present an optimized implementation of restricted Boltzmann machines for classification of images of handwritten digits. We show how it is possible to increase the performance of a simple implementation by a factor of 7 for realistic values of the number of hidden units, and point out to the limiting factors that prevent higher levels of performance.

### Requirements
 
* gcc compiler 
* R (optional - if no R installed, there will be an error without any plots but number of cycles will be computed anyway and successfully stored into PERF_\* files)
* ...

### Run

* Open terminal and navigate into directory rbm
* `RUNALL.sh ./<optimization directory> "<compiler flags>"`

Compiler flags can be found also in optimization directory in the folder `compiler_flags`.
Examples:

* `RUNALL.sh ./baseline`
* `RUNALL.sh ./scalar/merged "-mfma -O3 -fno-tree-vectorize"`
* `RUNALL.sh ./vector/merged "-mfma -O3"`

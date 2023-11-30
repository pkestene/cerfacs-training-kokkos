# Teaching material for C++/kokkos training at [CERFACS](https://cerfacs.fr/)

- date : December 11-13, 2023
- event [web site](https://cerfacs.fr/event/parallelisme-multi-architecture-avec-la-bibliotheque-c-kokkos/)
- location: [Cerfacs](https://cerfacs.fr/en/come-to-cerfacs-by-bike-bus-or-car/), Toulouse, France

## What is it about ?

About 15 years ago, Nvidia introduced the [CUDA architecture](https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html), i.e. the use of graphics of processing units for doing general purpose high performance applicative tasks.  Today, it is still a very challenging task to write new applications or simply refactor existing ones with portability and performance in mind, that is for being able to use efficiently available hardware HPC ressources, either multicore CPU (x86, ARM64, ...) or GPU (Nvidia /AMD / Intel).
This training aims at providing a dedicated introduction to the open source kokkos c++ library, which is mainly developped in US (Sandia and Oak Ridge labs)for about a decade, and funded by the Departement of Energy, under the [Exascale Computing Project](https://www.exascaleproject.org/) (ECP). We will present a theoretical and practical introduction to the Kokkos programming model, illustrating the advantages over other alternative shared memory programming model like OpenMP/OpenACC.
The participants of the training are expected to be able to integrate [kokkos library](https://github.com/kokkos/kokkos) in their own HPC projects.

## Objective of the training

Provide a theoretical and practical introduction to [kokkos programming model](https://github.com/kokkos/kokkos) from abstract concepts, i.e. parallel programming patterns (parallel for, reduce and scan loops), data containers to an overview of the ecosystem (profiling tools, linear algebra, python bindings, ...).

## Learning Outcomes

On completion of this course, you will be able to:
- build, install and integrate Kokkos into an existing application or write a kokkos application from scratch
- write kokkos computing kernels, manage memory data containers in a heterogeneous platform (CPU/GPU).
- able to access performance by using profiling tools
- refactor existing code for [performance portability](https://performanceportability.org/)

## Program

### Day 1

- Refresher on hardware architecture basics (CPU / GPU), on performance measurements (memory bandwidth, FLOPs), all that is needed to understand the difficulty of writing portable and performant code. Practical exercise.
- General introduction to Kokkos c++ library, its origin, overview of concepts and software abstractions: abstract machine model (host/device), the kokkos parallel programing model
- Examples of production codes using c++/kokkos
- Pratical exercise: how to build and install Kokkos c++ library, for using  CPU OpenMP backend and GPU/CUDA backend; how to chose compiler, how to integrate Kokkos in a Cmake based project; how to write a modulefile to simplify the use of the library.

### Day 2

- Writing c++/kokkos computing kernels: parallel programing patterns: for loops, reduce loops, scan loops.
- Overview of execution space and memory space concepts: why do we need them, how to use them. Concepts of execution policy
- Using hardware-aware data containers : multidimensional arrays (Kokkos::View) and hash maps (Kokkos::UnorderedMap)
- profiling tools (Kokkos::Tools)

### Day 3

- Advanced use of  Kokkos/c++: [hierarchical parallelism](https://kokkos.github.io/kokkos-core-wiki/ProgrammingGuide/HierarchicalParallelism.html?highlight=hierarchical) (about using teams of threads in a portable way). Examples of use.
- Coupling MPI and Kokkos for distributed applications. Introduction the [Kokkos/RemoteSpace](https://github.com/kokkos/kokkos-remote-spaces) (experimental feature, accessing remote nodes memory)
- Overview of Kokkos ecosystem: linear algebra ([KokkosKernels](https://github.com/kokkos/kokkos-kernels)), [pykokkos](https://github.com/kokkos/pykokkos) (python bindings)
- Kokkos for Fortran users: [Kokkos/FLCL](https://github.com/kokkos/kokkos-fortran-interop) (Fortran Language Compatibility Layer)

# Compute Mandelbrot set

**importante note:** :
- the following exercises are designed to work with kokkos and cmake.
- to avoid re-building kokkos, just use the modulefile to load kokkos library we built during exerice 02-build-a-kokkos-app-with-cmake

## about

Here is very simple kokkos example to compute Mandelbrot set and illustrate gradually several aspect of kokkos:
- simple range and multidimensionnal range execution policy
- asynchronous memory copy (i.e. overlap between a computationnal functor and a deep copy operation).

A reference serial version is proposed.

Three different versions provided:

* range : using Kokkos::Range policy

* mdrange : using Kokkos::MDRange policy

* pipeline0 : performs computations piece by piece and overlap device computation with host/device memory transfer

**Go into each of these directory and following instructions from dedicated Readme.**

NB: version pipeline1 require kokkos to be configured with both Kokkos::Cuda and Kokkos::OpenMP
backends, and lambda function enabled.

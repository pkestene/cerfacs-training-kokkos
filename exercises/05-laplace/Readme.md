# Laplace (Stencil)

**importante note:** :
- the following exercises are designed to work with kokkos and cmake.
- to avoid re-building kokkos, just use the modulefile to load kokkos library we built during exerice 02-build-a-kokkos-app-with-cmake

## about

Here is very simple stencil computation compute
- simple range and multidimensionnal range execution policy
- asynchronous memory copy (i.e. overlap between a computationnal functor and a deep copy operation).

A reference serial version is proposed.

You need to fill TODO in kokkos version.

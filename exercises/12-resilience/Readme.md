# [kokkos-resilience](https://github.com/kokkos/kokkos-resilience)

Kokkos Resilience is an experimental extension to Kokkos for providing convenient resilience and checkpointing to scientific applications.

## instructions to build kokkos-resilience dependencies

```shell
git clone git@github.com:pkestene/kokkos-resilience.git
cd kokkos-resilience
git checkout install_dependencies
```

This fork of kokkos-resilience contains a branch named `install_dependencies` which is a cmake sub-project to help build all dependencies.

```shell
cd dependencies
mkdir _build; cd _build
cmake ..
make
```

This should install all dependencies in `$HOME/.kokkos-resilience_dependencies` and provide a modulefile to ease the use of those tools.

```shell
module use $HOME/.kokkos-resilience_dependencies/share/modulefiles
module use kokkos-resilience_dependencies/1.0
```

## instructions to build kokkos-resilience itself

Current version of kokkos-resilience can only be build for kokkos OpenMP backend.

```shell
module load kokkos/4.1.00-openmp-gnu-9.4.0-RelWithDebInfo

module use $HOME/.kokkos-resilience_dependencies/share/modulefiles
module use kokkos-resilience_dependencies/1.0

# go back at top-level of kokkos-resilience source tree
mkdir _build/openmp
cd _build/openmp
cmake ../..
make
```

Then test one of the example, e.g. `ping-pong`.

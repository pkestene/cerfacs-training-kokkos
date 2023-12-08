#ifndef DATA_WAREHOUSE_H
#define DATA_WAREHOUSE_H

#include "kokkos_shared.h"

/**
 * Holds data, i.e. Fields (on DEVICE memory)
 */
class DataWarehouse
{

public:
  DataWarehouse(int xdim, int ydim);
  ~DataWarehouse();

  // fields that hold the solution 2D
  Field2d x_new;
  Field2d x_old;

  // fields that hold the boundary points 1D
  Field1d bndN;
  Field1d bndE;
  Field1d bndS;
  Field1d bndW;

private:
  // privatize copy constructors so we can't pass by value,
  // we only want one copy
  DataWarehouse(const DataWarehouse & w) {}
  DataWarehouse &
  operator=(const DataWarehouse & w)
  {
    return *this;
  }

  int xdim_;
  int ydim_;

}; // class DataWarehouse

#endif // DATA_WAREHOUSE_H

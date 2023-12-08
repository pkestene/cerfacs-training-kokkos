#include "data_warehouse.h"

// =======================================================
// =======================================================
DataWarehouse::DataWarehouse(int xdim, int ydim)
  : x_new("x_new", xdim, ydim)
  , x_old("x_old", xdim, ydim)
  , bndN("bndN", xdim)
  , bndE("bndE", ydim)
  , bndS("bndS", xdim)
  , bndW("bndW", ydim)
  , xdim_(xdim)
  , ydim_(ydim)
{}

// =======================================================
// =======================================================
DataWarehouse::~DataWarehouse()
{
}

//******************************************
// operators.f90
// based on min-app code written by Oliver Fuhrer, MeteoSwiss
// modified by Ben Cumming, CSCS
// *****************************************

// Description: Contains simple operators which can be used on 3d-meshes

#ifndef OPERATORS_H
#define OPERATORS_H

#include "data.h"
#include "data_warehouse.h"
#include "kokkos_shared.h"

namespace operators
{
void
diffusion(Field2d up, Field2d sp, const DataWarehouse & dw);
}

#endif // OPERATORS_H

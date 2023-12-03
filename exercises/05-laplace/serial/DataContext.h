#ifndef DATA_CONTEXT_H_
#define DATA_CONTEXT_H_

#include "common.h"
#include "params.h"

struct DataContext {

  real_t* A;
  real_t* Anew;
  real_t* Aref;
  real_t* rhs;

  DataContext(Params& params) {
    int NX = params.NX;
    int NY = params.NY;
    
    A    = new real_t[NX*NY];
    Anew = new real_t[NX*NY];
    Aref = new real_t[NX*NY];
    rhs  = new real_t[NX*NY];

    // reset
    memset(A,    0, NY * NX * sizeof(real_t));
    memset(Aref, 0, NY * NX * sizeof(real_t));

  };

  ~DataContext() {

    delete[] A;
    delete[] Anew;
    delete[] Aref;
    delete[] rhs;
  };
  
};

#endif // DATA_CONTEXT_H_

#include "solver.h"

Solver::Solver()
{

}

void Solver::solve()
{
    Kx = randu<mat> (100, 100);
    Ky = randu<mat> (100, 100);
    mat result = Kx*Ky.t();
    result.save(hdf5_name("A.h5", "my_data"));
}

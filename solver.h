#ifndef SOLVER_H
#define SOLVER_H

#include <armadillo>

using namespace arma;

/*!
 \class Solver
 \brief Класс, в котором будут происходить вычисления
*/
class Solver
{
public:
    Solver();
    Mat<float> H;
    Mat<float> Kx, Ky;
    Mat<float> Sw;
    Col<float> Krw;

};

#endif // SOLVER_H

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
//! Методы
public:
    Solver();

//! Поля
    Mat<float> H;
    Mat<float> Kx, Ky;
    Mat<float> Sw, Po;
    Col<float> Krw;

    int nx, ny;
    int number_of_time_steps;
    float dt;
};

#endif // SOLVER_H

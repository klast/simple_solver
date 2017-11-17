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

    void solve();

//! Поля
    mat H;
    mat Kx, Ky;
    mat Sw, Po;
    Col<float> Krw;

    int nx, ny;
    int number_of_time_steps;
    float dt;
};

#endif // SOLVER_H

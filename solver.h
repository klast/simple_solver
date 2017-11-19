#ifndef SOLVER_H
#define SOLVER_H

#include <omp.h>
#include <QString>
#include <QDebug>

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
    void finalize_hdf5();

//! Поля
    mat H;
    mat Kx, Ky;
    mat Sw, Po;
    vec Krw;

    int nx, ny;
    int number_of_time_steps;
    float dt;

};

#endif // SOLVER_H

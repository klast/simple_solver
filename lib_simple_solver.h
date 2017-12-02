#ifndef LIB_SIMPLE_SOLVER_H
#define LIB_SIMPLE_SOLVER_H

#include "lib_simple_solver_global.h"
#include "reader.h"
#include "solver.h"
#include <QFile>
#include <QDataStream>

class LIB_SIMPLE_SOLVERSHARED_EXPORT Lib_simple_solver
{

public:
    Lib_simple_solver();

private:

    /*!
     * \brief Полный цикл от считывания до конечного решения
     */
    void simulate();

    /*!
     * \brief Считывание модели
     */
    void read();

    bool set_datafile(QString &filename);

    /*!
     * \brief Инициализация класса Solver из переменных класса Reader
     */
    void init();

    /*!
     * \brief Вызов решения из класса Solver
     */
    void solve();

//! элементы
    Solver solver;
    Reader reader;
    int nx, ny, nt;
    QString title;
    QFile logfile;
    QDataStream debug;
};

#endif // LIB_SIMPLE_SOLVER_H

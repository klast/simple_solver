#ifndef LIB_SIMPLE_SOLVER_H
#define LIB_SIMPLE_SOLVER_H

#include "lib_simple_solver_global.h"
#include "model.h"
#include <QFile>
#include <QDataStream>

class LIB_SIMPLE_SOLVERSHARED_EXPORT Lib_simple_solver : Model
{

public:
    Lib_simple_solver();

    bool set_datafile(QString filename);

    void simulate();

};

#endif // LIB_SIMPLE_SOLVER_H

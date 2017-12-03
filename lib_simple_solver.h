#ifndef LIB_SIMPLE_SOLVER_H
#define LIB_SIMPLE_SOLVER_H

#include "lib_simple_solver_global.h"
#include "model.h"
#include "reader.h"
#include <QFile>
#include <QTextStream>



extern "C"
{

    static QString datafile_name;
    LIB_SIMPLE_SOLVERSHARED_EXPORT int get_2()
    {
        return 2;
    }
    LIB_SIMPLE_SOLVERSHARED_EXPORT void set_datafile(char *filename)
    {
        datafile_name = QString(filename);
    }
    LIB_SIMPLE_SOLVERSHARED_EXPORT void get_datafile(char *result)
    {
        strcpy(result, datafile_name.toStdString().c_str());
    }

}

class LIB_SIMPLE_SOLVERSHARED_EXPORT Lib_simple_solver
{

public:
    Lib_simple_solver();
    Model model;

};

#endif // LIB_SIMPLE_SOLVER_H

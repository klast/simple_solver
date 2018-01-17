#ifndef LIB_SIMPLE_SOLVER_H
#define LIB_SIMPLE_SOLVER_H

#include "lib_simple_solver_global.h"
#include "model.h"
#include "reader.h"
#include <QFile>
#include <QTextStream>




extern "C"
{
    QString datafile_name[6];
    Model model;
    LIB_SIMPLE_SOLVERSHARED_EXPORT int get_2()
    {
        return 2;
    }
    LIB_SIMPLE_SOLVERSHARED_EXPORT void set_file(int type, char *filename)
    {
        filetypes f_type = model.reader.get_type(type);
        datafile_name[type] = QString(filename);
        model.reader.set_file(f_type, datafile_name[type]);
    }
    LIB_SIMPLE_SOLVERSHARED_EXPORT int simulate()
    {
        return model.simulate();
    }
    LIB_SIMPLE_SOLVERSHARED_EXPORT void get_file(int type, char *result)
    {
        strcpy(result, datafile_name[type].toStdString().c_str());
    }

}

class LIB_SIMPLE_SOLVERSHARED_EXPORT Lib_simple_solver
{

public:
    Lib_simple_solver();

};

#endif // LIB_SIMPLE_SOLVER_H

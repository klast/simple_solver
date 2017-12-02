#include "lib_simple_solver.h"


Lib_simple_solver::Lib_simple_solver()
{
    nx = 100;
    ny = 100;
    nt = 15;
    logfile.setFileName("solver.log");
    logfile.open(QIODevice::WriteOnly | QIODevice::Text);
    debug.setDevice(&logfile);
    debug << "Constructor";
    qDebug() << "Constructor";
}

bool LIB_SIMPLE_SOLVERSHARED_EXPORT Lib_simple_solver::set_datafile(QString filename)
{
    bool result = reader.set_datafile(filename);
    debug << "setting datafile";
    logfile.close();
    return result;
}

void LIB_SIMPLE_SOLVERSHARED_EXPORT Lib_simple_solver::simulate()
{
    debug << "Trying to simulate";
    read();
    init();
    solve();
}

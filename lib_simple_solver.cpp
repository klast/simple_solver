#include "lib_simple_solver.h"


Lib_simple_solver::Lib_simple_solver()
{
    nx = 100;
    ny = 100;
    nt = 15;
    logfile.setFileName("solver.log");
    logfile.open(QFile::WriteOnly);
    debug.setDevice(&logfile);
    debug << "Constructor";
}

bool Lib_simple_solver::set_datafile(QString filename)
{
   return reader.set_datafile(filename);
}

void Lib_simple_solver::simulate()
{
    debug << "Trying to simulate";
    read();
    init();
    solve();
}

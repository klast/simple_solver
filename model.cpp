#include "model.h"

Model::Model()
{
    nx = 100;
    ny = 100;
    nt = 15;
}

void Model::simulate()
{
    read();
    init();
    solve();
}

void Model::read()
{
    reader.read();
}

void Model::init()
{
    // TODO: вычислить шаг по времени по условию Куранта
    nx = reader.input_constants["nx"];
    ny = reader.input_constants["ny"];
    solver.model_directory = reader.model_directory;
    solver.datafile_name = reader.datafile.fileName();
    reader.input_1d_arrays.clear();
    reader.input_constants.clear();
}

void Model::solve()
{
    solver.solve();
}


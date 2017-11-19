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
    nx = reader.input_constants["nx"];
    ny = reader.input_constants["ny"];
}

void Model::solve()
{
    solver.solve();
}


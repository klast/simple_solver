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

}

void Model::init()
{

}

void Model::solve()
{

}

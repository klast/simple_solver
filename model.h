#ifndef MODEL_H
#define MODEL_H

#include "solver.h"
#include "reader.h"


/*!
 \class Model
 \brief Управляющий класс, в котором будут вызываться чтение, вычисление и тд
 через объекты вспомогательных подклассов
*/
class Model
{
//! функции
public:
    Model();

    void simulate();
    void read();
    void init();
    void solve();

//! элементы
    Solver solver;
    Reader reader;
    int nx, ny, nt;
    QString title;

};

#endif // MODEL_H

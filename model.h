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

//! элементы
    Solver solver;
    Reader reader;

};

#endif // MODEL_H

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

  //!  void output_animation_file(double current_time);
    void simulate();
    void read();
    void init();
    void solve();

//! элементы
    Solver solver;
    Reader reader;
    int nx, ny, nt;

};

#endif // MODEL_H

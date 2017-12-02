#ifndef MODEL_H
#define MODEL_H

#include "solver.h"
#include "reader.h"
#include <QFile>
#include <QTextStream>


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

    //! TODO: Критерий Куранта

    /*!
     * \brief Полный цикл от считывания до конечного решения
     */
    void simulate();

    /*!
     * \brief Считывание модели
     */
    void read();

    /*!
     * \brief Инициализация класса Solver из переменных класса Reader
     */
    void init();

    /*!
     * \brief Вызов решения из класса Solver
     */
    void solve();

//! элементы
    Solver solver;
    Reader reader;
    int nx, ny, nt;
    QString title;
    QFile logfile;
    QTextStream debug;

};

#endif // MODEL_H

#ifndef MODEL_H
#define MODEL_H

#include "solver.h"
//#include <QApplication>
#include <QtGlobal>
#include "reader.h"
#include "logger.h"
#include <QScopedPointer>
#include <QFile>
#include <QTextStream>
#include <QLoggingCategory>
#include <QDateTime>


/*!
 \class Model
 \brief Управляющий класс, в котором будут вызываться чтение, вычисление и тд
 через объекты вспомогательных подклассов
*/

static QScopedPointer<QFile> logFile;

class Model
{
//! функции
public:
    Model();

    //! TODO: Критерий Куранта

    /*!
     * \brief Полный цикл от считывания до конечного решения
     */
    int simulate();

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

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

//! элементы
    Solver solver;
    Reader reader;
    int nx, ny, nt;
    QString title;    

};

#endif // MODEL_H

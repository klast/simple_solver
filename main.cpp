#include "mainwindow.h"
#include <QApplication>
#include <armadillo>

using namespace arma;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    mat A = randu<mat>(4,5);
    mat B = randu<mat>(4,5);

    std::cout << A*B.t() << endl;

    return a.exec();
}

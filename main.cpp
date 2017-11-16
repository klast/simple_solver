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
    mat result = A*B.t();
    std::cout << result << std::endl;
    return a.exec();
}

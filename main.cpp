#include "mainwindow.h"
#include <QApplication>
#include <armadillo>

using namespace arma;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    mat A = randu<mat>(100, 100);
    mat B = randu<mat>(100, 100);
    A(0,0) = 5;
    mat result = A * B.t();
    result(0, 0) = 0;
    result.save("result.csv", csv_ascii);
    std::cout << result << std::endl;
    return a.exec();
}

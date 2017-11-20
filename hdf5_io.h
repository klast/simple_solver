#ifndef HDF5_IO_H
#define HDF5_IO_H

#include <QString>
#include <QDebug>
#include <QDir>


#include <armadillo>

using namespace arma;

/*!
 \class hdf5_io
 \brief Класс работы с hdf5 файлами, используется в solver и визуализации
*/
class hdf5_io
{
public:
    hdf5_io();

    void hdf5_test_solve(int time_step, int nx, int ny);

    void finalize_hdf5(QString datafile_name, int num_global_steps);
};

#endif // HDF5_IO_H

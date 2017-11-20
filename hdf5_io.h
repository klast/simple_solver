#ifndef HDF5_IO_H
#define HDF5_IO_H

#include "hdf5.h"

#include <QString>
#include <QDebug>
#include <QDir>

enum CUBES_TO_EXPORT
{
  PRESSURE,
  SATURATION
};

#define group_id(cube_enum) { (cube_enum==CUBES_TO_EXPORT::PRESSURE) ? pressure_group_id : pressure  }

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

    void save_cube_on_timestep(Mat<double> &cube, QString cube_name, int time_step);

    void copy_cube_on_timestep(CUBES_TO_EXPORT cube, int time_step);

    void finalize_hdf5(QString datafile_name, int num_global_steps);

    hid_t group_id[2];

    hid_t pressure_group_id, saturation_group_id;
};

#endif // HDF5_IO_H

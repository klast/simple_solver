#define HDF5_SOLVE
#ifdef HDF5_SOLVE
#ifndef HDF5_IO_H
#define HDF5_IO_H

#include <QString>
#include <QDebug>
#include <QDir>

#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5File.hpp>
#include <highfive/H5Group.hpp>
#include <highfive/H5Attribute.hpp>

#include <Eigen>
#include <Eigen/Dense>
#include "logger.h"

using namespace Eigen;

enum CUBES_TO_EXPORT_ENUM
{
  PRESSURE,
  SATURATION
};

/*!
 \class hdf5_io
 \brief Класс работы с hdf5 файлами, используется в solver и визуализации
*/
class hdf5_io
{
public:
    hdf5_io();

    void hdf5_test_solve(int time_step);

    void save_cube_on_timestep(std::vector<double> &cube, std::string cube_name, int time_step);

    void openfile();

    void get_cube_on_timestep(std::string cube_name, int time_step, std::vector<double> *cube);

    //void copy_cube_on_timestep(CUBES_TO_EXPORT cube, int time_step);

   // void finalize_hdf5(QString datafile_name, int num_global_steps);

    QString datafile_name;

    int nx, ny;
};

#endif // HDF5_IO_H
#endif

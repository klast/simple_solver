#include "hdf5_io.h"

hdf5_io::hdf5_io()
{

}

/*void hdf5_io::hdf5_test_solve(int time_step, int nx, int ny)
{
    mat A = randu<mat>(nx, ny);
    mat B = randu<mat>(nx, ny);
    mat result = arma::solve(A, B);
    qDebug() << "Step " << time_step << "calculated\n";
    save_cube_on_timestep(result, "pressure", time_step);
    save_cube_on_timestep(result, "saturation", time_step);
    qDebug() << "Step " << time_step << "saved\n";
}

void hdf5_io::save_cube_on_timestep(Mat<double> &cube, QString cube_name, int time_step)
{
    QString cube_step_file = QString("step.%1.%2.h5").arg(cube_name, QString::number(time_step));
    QFile(cube_step_file).remove();
    cube.save(hdf5_name(cube_step_file.toStdString(), cube_name.toStdString()));
    qDebug() << QString("%1 on %2 time_step is saved").arg(cube_name, QString::number(time_step));
}

void hdf5_io::copy_cube_on_timestep(CUBES_TO_EXPORT cube, int time_step)
{
    QString cube_name = (cube == CUBES_TO_EXPORT::PRESSURE)? "pressure": "saturation";
    QString step_file_name = QString("step.%1.%2.h5").arg(cube_name, QString::number(time_step));
    hid_t step_file = H5Fopen(step_file_name.toStdString().c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
    herr_t status = H5Ocopy(step_file, cube_name.toStdString().c_str(), group_id[cube], QString::number(time_step).toStdString().c_str(), H5P_OBJECT_COPY_DEFAULT, H5P_LINK_CREATE_DEFAULT);
    if(status < 0)
    {
        qDebug() << "Can't open step " << time_step << " file\n";
    }
    H5Fclose(step_file);
    QFile(step_file_name).remove();
}

void hdf5_io::finalize_hdf5(QString datafile_name, int num_global_steps)
{
    hid_t result_file;
    QString h5_file = datafile_name.split('.').at(0) + ".h5";
    result_file = H5Fcreate(h5_file.toStdString().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if(result_file < 0)
    {
        qDebug() << "Can't open result file\n";
    }
    group_id[CUBES_TO_EXPORT::PRESSURE] = H5Gcreate(result_file, "pressure_cube", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    group_id[CUBES_TO_EXPORT::SATURATION] = H5Gcreate(result_file, "saturation_cube", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if(group_id[CUBES_TO_EXPORT::PRESSURE] < 0 || group_id[CUBES_TO_EXPORT::SATURATION] < 0)
    {
        qDebug() << "Can't create group\n";
    }
    for(int i = 0; i < num_global_steps; i++)
    {
        copy_cube_on_timestep(CUBES_TO_EXPORT::PRESSURE, i);
        copy_cube_on_timestep(CUBES_TO_EXPORT::SATURATION, i);
    }
    H5Gclose(group_id[CUBES_TO_EXPORT::PRESSURE]);
    H5Gclose(group_id[CUBES_TO_EXPORT::SATURATION]);
    H5Fclose(result_file);
}*/

#include "hdf5_io.h"

hdf5_io::hdf5_io()
{

}

void hdf5_io::hdf5_test_solve(int time_step, int nx, int ny)
{
    mat A = randu<mat>(nx, ny);
    mat B = randu<mat>(nx, ny);
    mat result = arma::solve(A, B);
    qDebug() << "Step " << time_step << "calculated\n";
    QString pressure_step_file = QString("step.pressure.%1.h5").arg(QString::number(time_step));
    QString saturation_step_file = QString("step.saturation.%1.h5").arg(QString::number(time_step));
    QFile(pressure_step_file).remove();
    QFile(saturation_step_file).remove();
    result.save(hdf5_name(pressure_step_file.toStdString(), "pressure"));
    result.save(hdf5_name(saturation_step_file.toStdString(), "saturation"));
    qDebug() << "Step " << time_step << "saved\n";
}

void hdf5_io::finalize_hdf5(QString datafile_name, int num_global_steps)
{
    hid_t result_file, pressure_group_id, saturation_group_id;
    herr_t pressure_status, saturation_status;
    QString h5_file = datafile_name.split('.').at(0) + ".h5";
    result_file = H5Fcreate(h5_file.toStdString().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if(result_file < 0)
    {
        qDebug() << "Can't open result file\n";
    }
    pressure_group_id = H5Gcreate(result_file, "pressure_cube", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    saturation_group_id = H5Gcreate(result_file, "saturation_cube", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if(pressure_group_id < 0 || saturation_group_id)
    {
        qDebug() << "Can't create group\n";
    }
    for(int i = 0; i < num_global_steps; i++)
    {
        QString pressure_step_file_name = QString("step.pressure.%1.h5").arg(QString::number(i));
        QString saturation_step_file_name = QString("step.saturation.%1.h5").arg(QString::number(i));
        hid_t pressure_step_file, saturation_step_file;
        pressure_step_file = H5Fopen(pressure_step_file_name.toStdString().c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        saturation_step_file = H5Fopen(saturation_step_file_name.toStdString().c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        if(pressure_step_file < 0 || saturation_step_file < 0)
        {
            qDebug() << "Can't open step " << i << " file\n";
        }
        pressure_status = H5Ocopy(pressure_step_file, "pressure", pressure_group_id, QString::number(i).toStdString().c_str(), H5P_OBJECT_COPY_DEFAULT, H5P_LINK_CREATE_DEFAULT);
        saturation_status = H5Ocopy(saturation_step_file, "saturation", saturation_group_id, QString::number(i).toStdString().c_str(), H5P_OBJECT_COPY_DEFAULT, H5P_LINK_CREATE_DEFAULT);
        if(pressure_status < 0 || saturation_status < 0)
        {
            qDebug() << "Can't copy step " << i << " file\n";
        }
        else
        {
            qDebug() << "Step " << i << " merged\n";
        }
        H5Fclose(saturation_step_file);
        H5Fclose(pressure_step_file);
        QFile(pressure_step_file_name).remove();
        QFile(saturation_step_file_name).remove();
    }
    H5Gclose(pressure_group_id);
    H5Gclose(saturation_group_id);
    H5Fclose(result_file);
}

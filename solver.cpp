#include "solver.h"
#include "hdf5.h"
#include <QDir>

Solver::Solver()
{
    number_of_time_steps = 5;
    nx = 500;
    ny = 500;
}

void Solver::solve()
{
    for(int time_step = 0; time_step < number_of_time_steps; time_step++)
    {
        mat A = randu<mat>(nx, ny);
        mat B = randu<mat>(nx, ny);
        mat result = arma::solve(A, B);
        qDebug() << "Step " << time_step << "calculated\n";
        QString step_file = QString("step.%1.h5").arg(QString::number(time_step));
        QFile(step_file).remove();
        result.save(hdf5_name(step_file.toStdString(), "result"));
        qDebug() << "Step " << time_step << "saved\n";
    }
    finalize_hdf5();
}

void Solver::finalize_hdf5()
{
    hid_t step_file, result_file, group_id;
    herr_t status;
    QString h5_file = datafile_name.split('.').at(0) + ".h5";
    result_file = H5Fcreate(h5_file.toStdString().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if(result_file < 0)
    {
        qDebug() << "Can't open result file\n";
    }
    group_id = H5Gcreate(result_file, "results", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if(group_id < 0)
    {
        qDebug() << "Can't create group\n";
    }
    for(int i = 0; i < number_of_time_steps; i++)
    {
        QString step_file_name = QString("step.%1.h5").arg(QString::number(i));
        step_file = H5Fopen(step_file_name.toStdString().c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        if(step_file < 0)
        {
            qDebug() << "Can't open step " << i << " file\n";
        }
        QString name = "result";
        status = H5Ocopy(step_file, "result", group_id, QString::number(i).toStdString().c_str(), H5P_OBJECT_COPY_DEFAULT, H5P_LINK_CREATE_DEFAULT);
        if(status < 0)
        {
            qDebug() << "Can't copy step " << i << " file\n";
        }
        else
        {
            qDebug() << "Step " << i << " merged\n";
        }
        H5Fclose(step_file);
        QFile(step_file_name).remove();
    }
    H5Gclose(group_id);
    H5Fclose(result_file);
}


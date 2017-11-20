#include "solver.h"
#include "hdf5.h"
#include <QDir>

Solver::Solver()
{
    num_global_steps = 5;
    nx = 500;
    ny = 500;
}

void Solver::hdf5_test_solve(int time_step)
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

void Solver::finalize_hdf5()
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

void Solver::solve()
{
    for (int step = 0; step < num_global_steps; step++)
    {

        double global_dt = T / num_global_steps; // Определяем глобальный шаг по времени
        //inner_solve(step * global_dt, (step + 1) * global_dt); // Выполняем внутренние итерации
        hdf5_test_solve(step);
        // TODO: обновляем необходимые данные в классе Solver

    }
    finalize_hdf5();
    //! TODO(Вова): объединить h5 с каждого шага в один, или сразу всё в один h5 записывать
}

void Solver::inner_solve(double begin_time, double end_time)
{

    // Если глобальный временной шаг меньше шага по Куранту
    if ((end_time - begin_time) < dt)
        dt = end_time - begin_time;

    double time = begin_time; // FIX: переименовать
    while (time < end_time)
    {

        // Уменьшаем шаг по времени, если он перескакивает конечное время расчёта
        if ((end_time - time) < dt)
            dt = end_time - time;

        double residual = 1.0; // невязка
        while(residual < epsilon)
        {

            // TODO: выполняем расчёт явной схемой
            // TODO: выполняем расчёт неявной схемой
            residual = calc_residual();

        };

        // TODO: обновляем необходимые данные класса Solver

        time += dt;

    };

}

double Solver::calc_residual()
{

    // TODO: реализовать расчёт невязки

    return 1.0;

}


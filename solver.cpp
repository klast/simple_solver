#include "solver.h"
#include "hdf5.h"
#include <QDir>

Solver::Solver()
{
    num_global_steps = 5;
    nx = 500;
    ny = 500;
}

void Solver::solve()
{
    for (int step = 0; step < num_global_steps; step++)
    {
        double global_dt = T / num_global_steps; // Определяем глобальный шаг по времени
        //inner_solve(step * global_dt, (step + 1) * global_dt); // Выполняем внутренние итерации
        //hdf5_worker.hdf5_test_solve(step, nx, ny);
        // TODO: обновляем необходимые данные в классе Solver
    }
   // hdf5_worker.finalize_hdf5(datafile_name, num_global_steps);
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


#include "solver.h"

Solver::Solver()
{

}

void Solver::solve()
{
    for (int step = 0; step < num_global_steps; step++)
    {

        double global_dt = T / num_global_steps; // Определяем глобальный шаг по времени
        inner_solve(step * global_dt, (step + 1) * global_dt); // Выполняем внутренние итерации
        //! TODO(Вова): сохраняем полученное решение в h5
        // TODO: обновляем необходимые данные в классе Solver

    };
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
        int num_inner_it = 0; // номер внутренней итерации
        while((residual >= epsilon) && (num_inner_it < MAX_INNER_ITERATIONS))
        {

            explicit_scheme_calc();
            // TODO: выполняем расчёт неявной схемой
            residual = calc_residual();

            num_inner_it++;

        };

        if (num_inner_it == MAX_INNER_ITERATIONS)
        {

            // TODO: сбросить результаты до начального состояния
            continue;

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

void Solver::explicit_scheme_calc()
{
    // TODO: рассчитать все параметры, зависящие от водонасыщенности s_w, при s_w = s_w(t_{n + 1})

    for (int node_x = 0; node_x < nx; node_x++)
        for (int node_y = 0; node_y < ny; node_y++)
        {
            // double volume = dx * dy * dz;
            // double coeff_1 = dt * compress_oil / porosity / volume;

            double T_x1_x = dy * dz / dx * k_relat_water[node_x][node_y] * k_relat_water[node_x + 1][node_y] / (k_relat_water[node_x][node_y] + k_relat_water[node_x + 1][node_y]) / 0.5 / viscosity_water / compress_water;
            double lambda_x1_x = k_absol[node_x][node_y] * k_absol[node_x + 1][node_y] / (k_absol[node_x][node_y] + k_absol[node_x + 1][node_y]) / 0.5;
            double potential_x1_x = water_press_prev[node_x + 1][node_y] - water_press_prev[node_x][node_y] - gravity * density_water * (heights[node_x + 1][node_y] - heights[node_x][node_y]);
            double coeff_x1_x = T_x1_x * lambda_x1_x * potential_x1_x;

            // water_press_next[node_x][node_y] =
        };

}

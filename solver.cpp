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
            // Коэффициенты T в точках (i +, j) и (i -, j) (здесь и далее см. обозн. в схеме Емченко)
            double T_xp_x = dy * dz / dx * k_absol_x[node_x + 1][node_y] * k_absol_x[node_x][node_y] / (k_absol_x[node_x + 1][node_y] + k_absol_x[node_x][node_y]) / 0.5;
            double T_x_xm = dy * dz / dx * k_absol_x[node_x - 1][node_y] * k_absol_x[node_x][node_y] / (k_absol_x[node_x - 1][node_y] + k_absol_x[node_x][node_y]) / 0.5;

            // Коэффициенты T в точках (i, j +) и (i, j -)
            double T_yp_y = dx * dz / dy * k_absol_y[node_x][node_y + 1] * k_absol_y[node_x][node_y] / (k_absol_y[node_x][node_y + 1] + k_absol_y[node_x][node_y]) / 0.5;
            double T_y_ym = dx * dz / dy * k_absol_y[node_x][node_y - 1] * k_absol_y[node_x][node_y] / (k_absol_y[node_x][node_y - 1] + k_absol_y[node_x][node_y]) / 0.5;

            // Коэффициенты lambda в точках (i +, j) и (i -, j)
            double lambda_xp_x = k_relat_water[node_x + 1][node_y] * k_relat_water[node_x][node_y] / (k_relat_water[node_x + 1][node_y] + k_relat_water[node_x][node_y]) / 0.5 / viscosity_water / compress_water;
            double lambda_x_xm = k_relat_water[node_x - 1][node_y] * k_relat_water[node_x][node_y] / (k_relat_water[node_x - 1][node_y] + k_relat_water[node_x][node_y]) / 0.5 / viscosity_water / compress_water;

            // Коэффициенты lambda в точках (i, j +) и (i, j -)
            double lambda_yp_y = k_relat_water[node_x][node_y + 1] * k_relat_water[node_x][node_y] / (k_relat_water[node_x][node_y + 1] + k_relat_water[node_x][node_y]) / 0.5 / viscosity_water / compress_water;
            double lambda_y_ym = k_relat_water[node_x][node_y - 1] * k_relat_water[node_x][node_y] / (k_relat_water[node_x][node_y - 1] + k_relat_water[node_x][node_y]) / 0.5 / viscosity_water / compress_water;

            // TODO: подумать, как работать с давлением воды (вводить его или пересчитывать через капиллярное давление и давление нефти)
            // Давление воды в точках (i +, j) и (i -, j)
            double pressure_xp_x = (capillary_press[node_x + 1][node_y] - capillary_press[node_x    ][node_y]) - (oil_press_prev[node_x + 1][node_y] - oil_press_prev[node_x    ][node_y]);
            double pressure_x_xm = (capillary_press[node_x    ][node_y] - capillary_press[node_x - 1][node_y]) - (oil_press_prev[node_x    ][node_y] - oil_press_prev[node_x - 1][node_y]);

            // Давление воды в точках (i, j +) и (i, j -)
            double pressure_yp_y = (capillary_press[node_x][node_y + 1] - capillary_press[node_x][node_y    ]) - (oil_press_prev[node_x][node_y + 1] - oil_press_prev[node_x][node_y    ]);
            double pressure_y_ym = (capillary_press[node_x][node_y    ] - capillary_press[node_x][node_y - 1]) - (oil_press_prev[node_x][node_y    ] - oil_press_prev[node_x][node_y - 1]);

            // Потенциал Phi в точках (i +, j) и (i -, j)
            double potential_xp_x = pressure_xp_x - gravity * density_water * (heights[node_x + 1][node_y] - heights[node_x    ][node_y]);
            double potential_x_xm = pressure_x_xm - gravity * density_water * (heights[node_x    ][node_y] - heights[node_x - 1][node_y]);

            // Потенциал Phi в точках (i, j +) и (i, j -)
            double potential_yp_y = pressure_yp_y - gravity * density_water * (heights[node_x][node_y + 1] - heights[node_x][node_y    ]);
            double potential_y_ym = pressure_y_ym - gravity * density_water * (heights[node_x][node_y    ] - heights[node_x][node_y - 1]);

            // Слагаемые, полученные при разложении интеграла
            double coeff_xp_x = T_xp_x * lambda_xp_x * potential_xp_x;
            double coeff_x_xm = T_x_xm * lambda_x_xm * potential_x_xm;
            double coeff_yp_y = T_yp_y * lambda_yp_y * potential_yp_y;
            double coeff_y_ym = T_y_ym * lambda_y_ym * potential_y_ym;

            // Рассчитываем водонасыщенность s_w на шаге t_{n + 1}
            water_press_next[node_x][node_y] = dt * compress_oil / porosity / (dx * dy * dz) * (coeff_xp_x - coeff_x_xm + coeff_yp_y - coeff_y_ym);
        };

}

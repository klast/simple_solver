#include "solver.h"
#include <algorithm>

Solver::Solver()
{

}

Well::Well()
{

}

void Solver::init(input_data_type &input_data)
{
    qInfo(logInit()) << "НАЧИНАЕМ ИНИЦИАЛИЗАЦИЮ ВХОДНЫХ ДАННЫХ";
    qInfo(logInit()) << "ИНИЦИАЛИЗАЦИЯ ПРОСТЫХ ПЕРЕМЕННЫХ";
    nx = 100;
    qInfo(logInit()) << "Количество ячеек по OX =" << nx;
    ny = 100;
    qInfo(logInit()) << "Количество ячеек по OY =" << ny;
    nz = 1;
    qInfo(logInit()) << "Количество ячеек по OZ =" << nz;
    dx = 10;
    qInfo(logInit()) << "Размер ячеек по OX =" << dx;
    dy = 10;
    qInfo(logInit()) << "Размер ячеек по OY =" << dy;
    dz = 1;
    qInfo(logInit()) << "Размер ячеек по OZ =" << dz;
    num_global_steps = 1000;
    qInfo(logInit()) << "Количество глобальных временных шагов =" << num_global_steps;
    T = num_global_steps * 1; //! типо 1 день, надо ли вообще так
    for(int i = 0; i < nx; i++)
    {
        //! SWAT == 0.4, инициализация
        std::vector<double> tmp(100);
        std::fill(tmp.begin(), tmp.begin() + tmp.size(), 0.4);
        s_water_init.push_back(tmp);
    }
    qInfo(logInit()) << "Начальная водонасыщенность =" << 0.4;
    qInfo(logInit()) << "ИНИЦИАЛИЗАЦИЯ СКВАЖИН";
    init_wells(input_data);
    init_swof(input_data);
    qInfo(logInit()) << "ИНИЦИАЛИЗАЦИЯ СВОЙСТВ";
    rock = input_data["rock"].at(1);//! коэффициент сжимаемости породы
    qInfo(logInit()) << "Коэффициент сжимаемости породы =" << rock;
    b_water = input_data["pvtw"].at(1);
    qInfo(logInit()) << "Коэффициент объемного расширения воды =" << b_water;
    compress_water = input_data["pvtw"].at(2);
    qInfo(logInit()) << "Коэффициент сжимаемости воды =" << compress_water;
    viscosity_water = input_data["pvtw"].at(3);
    qInfo(logInit()) << "Вязкость воды = " << viscosity_water;
    b_oil = input_data["pvdo"].at(1);
    qInfo(logInit()) << "Коэффициент объемного расширения нефти =" << b_oil;
    viscosity_oil = input_data["pvdo"].at(2);
    qInfo(logInit()) << "Вязкость нефти =" << viscosity_oil;
    density_oil = input_data["density"].at(0);
    qInfo(logInit()) << "Плотность нефти" << density_oil;
    density_water = input_data["density"].at(1);
    qInfo(logInit()) << "Плотность воды =" << density_water;
    init_array(input_data["tops"], &tops);
    qInfo(logInit()) << "Глубины ячеек =" << tops;
    init_array(input_data["pressure"], &pressure);
    qInfo(logInit()) << "Давление =" << pressure;
    init_array(input_data["poro"], &porosity);
    qInfo(logInit()) << "Пористость =" << porosity;
    init_array(input_data["permx"], &permx);
    qInfo(logInit()) << "Проницаемость по OX =" << permx;
    init_array(input_data["permx"], &permy);
    qInfo(logInit()) << "Пронимаемость по OY =" << permy;
    qInfo(logInit()) << "ИНИЦИАЛИЗАЦИЯ ПОКА ЗАКОНЧЕНА!";
    input_data.clear();
}

void Solver::init_swof(input_data_type &input_data)
{
    QVector<double> swof = input_data["swof"];
    int swof_size = swof.size() / 4;
    qInfo(logInit()) << "ИНИЦИАЛИЗАЦИЯ SWOF";
    for(int i = 0; i < swof_size; i++)
    {
        int index = i * 4;
        sw_init.push_back(swof[index]);
        krw_init.push_back(swof[index + 1]);
        krow_init.push_back(swof[index + 2]);
        pcow_init.push_back(swof[index + 3]);
    }
    qInfo(logInit()) << "Водонасыщенность" << sw_init;
    qInfo(logInit()) << "ОФП воды" << krw_init;
    qInfo(logInit()) << "ОФП нефти" << krow_init;
    qInfo(logInit()) << "Капиллярка" << pcow_init;
    swof.clear();
}

void Solver::init_wells(input_data_type &input_data)
{
    prod1.set_location(1, 1);
    prod2.set_location(98, 98);
    inj1.set_location(1, 98);
    inj2.set_location(98, 1);
    //! Инициализация дебитов и приемистостей
    QVector<double> wellinfo = input_data["wellinfo"];
    int wellinfo_size = wellinfo.size() / 4;
    for(int i = 0; i < wellinfo_size; i++)
    {
        int index = i * 4;
        prod1.values.push_back(wellinfo[index]);
        prod2.values.push_back(wellinfo[index + 1]);
        inj1.values.push_back(wellinfo[index + 2]);
        inj2.values.push_back(wellinfo[index + 3]);
    }
    qInfo(logInit()) << "prod1 (" << prod1.ix + 1 << "," << prod1.iy + 1 << ")";
    qInfo(logInit()) << "prod2 (" << prod2.ix + 1 << "," << prod2.iy + 1 << ")";
    qInfo(logInit()) << "inj1 (" << inj1.ix + 1 << "," << inj1.iy + 1 << ")";
    qInfo(logInit()) << "inj2 (" << inj2.ix + 1 << "," << inj2.iy + 1 << ")";
    qInfo(logInit()) << "Дебит жидкости prod1 размером" << prod1.values.size() << prod1.values;
    qInfo(logInit()) << "Дебит жидкости prod2 размером" << prod2.values.size() << prod2.values;
    qInfo(logInit()) << "Приемистость inj1 размером" << inj1.values.size() << inj1.values;
    qInfo(logInit()) << "Приемистость inj2 размером" << inj2.values.size() << inj2.values;
    wellinfo.clear();
}

void Solver::init_array(QVector<double> &data , vector_double_2d *arr)
{
    for(int i = 0; i < nx; i++)
    {
        std::vector<double> this_row(100);
        std::copy(data.begin() + i * ny, data.begin() + (i + 1) * ny, this_row.begin());
        arr->push_back(this_row);
    }
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

// Стандартные библиотеки C++
#include <algorithm>

// Пользовательские библиотеки
#include "solver.h"
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/Core>

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

    // TODO: убрать comperss_oil, оставить b_oil
    compress_oil = b_oil;

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

    // Инициализация ускорения свободного падения
    gravity = 9.81;

    // TODO: "встроить в интерфейс"
    epsilon = 1.0E-3;

    // TODO: попробовать упростить?
    capillary_press.resize(nx);
    oil_press_prev.resize(nx);
    oil_press_next.resize(nx);
    s_water_prev.resize(nx);
    s_water_next.resize(nx);
    k_relat_oil.resize(nx);
    k_relat_water.resize(nx);

    heights = tops; // TODO: убрать heights, переименовать в нужных местах в tops
    k_absol = permx;

    for (int node = 0; node < nx; node++)
    {
        capillary_press[node].resize(ny);
        oil_press_prev[node].resize(ny);
        oil_press_next[node].resize(ny);
        s_water_prev[node].resize(ny);
        s_water_next[node].resize(ny);
        k_relat_oil[node].resize(ny);
        k_relat_water[node].resize(ny);
    };
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
    krw_inter.init(swof_size, 1.0, sw_init, krw_init);
    krow_inter.init(swof_size, 1.0, sw_init, krow_init);
    pcow_inter.init(swof_size, 1.0, sw_init, pcow_init);

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

double Solver::middle_point(vector_double_2d &arr, int i, int j, const int side)
{
    // Флаг, определяющий принадлежность элемента границе сетки
    bool right_border = (i == nx - 1);
    bool left_border  = (i == 0);
    bool down_border  = (j == ny - 1);
    bool up_border    = (j == 0);

    switch(side)
    {
    case NODE_SIDE::X_PLUS:
    {
        return right_border ? 0.0 : (arr[i + 1][j] + arr[i][j]) * 0.5;
    }
    case NODE_SIDE::X_MINUS:
    {
        return left_border ? 0.0 : (arr[i - 1][j] + arr[i][j]) * 0.5;
    }
    case NODE_SIDE::Y_PLUS:
    {
        return down_border ? 0.0 : (arr[i][j + 1] + arr[i][j]) * 0.5;
    }
    case NODE_SIDE::Y_MINUS:
    {
        return up_border ? 0.0 : (arr[i][j - 1] + arr[i][j]) * 0.5;
    }
    };
}


void Solver::solve()
{
    for (int step = 0; step < num_global_steps; step++)
    {
        qInfo(logSolve()) << "Начитаем" << step << "шаг";
        double global_dt = T / num_global_steps; // Определяем глобальный шаг по времени
        inner_solve(step * global_dt, (step + 1) * global_dt); // Выполняем внутренние итерации
        //! TODO(Вова): сохраняем полученное решение в h5
        // TODO: обновляем необходимые данные в классе Solver

    };
    //! TODO(Вова): объединить h5 с каждого шага в один, или сразу всё в один h5 записывать

}

void Solver::inner_solve(double begin_time, double end_time)
{
    // TODO: добавить расчёт шага dt через условие Куранта
    dt = end_time - begin_time;

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
        s_water_next = s_water_init;
        oil_press_next = pressure;

        while((residual >= epsilon) && (num_inner_it < MAX_INNER_ITERATIONS))
        {
            // TODO: проверить корректность копирования
            s_water_prev = s_water_next;
            oil_press_prev = oil_press_next;

            // Заполнение двумерных матриц проницаемостей и капиллярки
            fill_data();

            implicit_scheme_calc();
            explicit_scheme_calc();

            residual = calc_residual();

            num_inner_it++;

            if (num_inner_it == MAX_INNER_ITERATIONS)
            {
                // TODO: сбросить результаты до начального состояния
                dt = 0.5 * dt;
                s_water_next = s_water_init;
                oil_press_next = pressure;
                continue;
            };

            if (dt < 1.0E-16)
                throw "FUCK MY BRAIN! EVERYTHING IS AWFUL!";
        };
        qInfo(logSolve()) << "Количество внутренних итераций =" << num_inner_it;

        // TODO: обновляем необходимые данные класса Solver
        s_water_init = s_water_next;
        pressure = oil_press_next;

        time += dt;

    };

}

double Solver::calc_residual()
{
    double norm_sw = 0.0;
    double norm_po = 0.0;
    for (int node_x = 0; node_x < nx; node_x++)
        for (int node_y = 0; node_y < ny; node_y++)
        {
            norm_sw += pow(s_water_prev[node_x][node_y] - s_water_next[node_x][node_y], 2.0);
            norm_po += pow(oil_press_prev[node_x][node_y] - oil_press_next[node_x][node_y], 2.0);
        };

    norm_sw = sqrt(norm_sw);
    norm_po = sqrt(norm_po);

    qInfo(logSolve()) << "Невязка по водонасыщенности =" << norm_sw << ", по давлению нефти =" << norm_po;

    return std::max(norm_sw, norm_po);
}

void Solver::fill_data()
{
    qDebug(logSolve()) << "Заполняем данные, зависящие от водонасыщенности";
    for (int node_x = 0; node_x < nx; node_x++)
        for (int node_y = 0; node_y < ny; node_y++) {
            k_relat_oil[node_x][node_y] = krow_inter.y(s_water_prev[node_x][node_y]);
            k_relat_water[node_x][node_y] = krw_inter.y(s_water_prev[node_x][node_y]);
            capillary_press[node_x][node_y] = pcow_inter.y(s_water_prev[node_x][node_y]);
        };
}

void Solver::implicit_scheme_calc()
{
    qDebug(logSolve()) << "Начинаем расчет по неявной схеме";
    Eigen::SparseMatrix<double> pres_mat(nx * ny, nx * ny); // матрица коэффициентов в уравнениях на давление нефти
    Eigen::VectorXd pres_vec(nx * ny); // правая часть СЛАУ в уравнениях на давление нефти
    Eigen::BiCGSTAB< Eigen::SparseMatrix<double> > mat_solver; // решатель системы СЛАУ

    // Структура "триплет" для ускорения заполнения разреженной матрицы
    typedef Eigen::Triplet<double> Trip;
    std::vector<Trip> tripletList;
    qDebug(logSolve()) << "Начинаем формирование матрицы";
    for (int node_y = 0; node_y < ny; node_y++)
        for (int node_x = 0; node_x < nx; node_x++)
        {
            // Матрица pres_mat содержит коэффициенты при давлениях p_{i, j}.
            // Самих неизвестных величин M * N штук, поэтому матрица
            // имеет размер M * N. Для облегчения работы вводятся
            // переменные, в которых пересчитывается номер элемента в матрице.
            Point<int> index;
            index[X_PLUS] = (node_x + 1) + node_y * ny; // номер p_{i + 1, j} в матрице
            index[X_Y] =     node_x      + node_y * ny; // номер p_{i    , j} в матрице
            index[X_MINUS] = (node_x - 1) + node_y * ny;// номер p_{i - 1, j} в матрице
            index[Y_PLUS] = node_x + (node_y + 1) * ny; // номер p_{i, j + 1} в матрице
            index[Y_MINUS] = node_x + (node_y - 1) * ny;// номер p_{i, j - 1} в матрице

            // Флаг, определяющий принадлежность элемента границе сетки
            Point<bool> border;
            border[X_PLUS] = (node_x == nx - 1);
            border[X_MINUS] = (node_x == 0);
            border[Y_MINUS] = (node_y == ny - 1);
            border[Y_PLUS] = (node_y == 0);

            // Коэффициенты T в точках (i +, j), (i -, j) (i, j +) и (i, j -) (здесь и далее см. обозн. в схеме Емченко)
            Point<double> T_coeff;
            T_coeff[X_PLUS] = middle_point(k_absol, node_x, node_y, X_PLUS) * dy * dz / dx;
            T_coeff[X_MINUS] = middle_point(k_absol, node_x, node_y, X_MINUS) * dy * dz / dx;
            T_coeff[Y_PLUS] = middle_point(k_absol, node_x, node_y, Y_PLUS) * dx * dz / dy;
            T_coeff[Y_MINUS] = middle_point(k_absol, node_x, node_y, Y_MINUS) * dx * dz / dy;

            Point<double> lambda_oil, lambda_water;
            // Коэффициенты lambda_o * B в точках (i +, j), (i -, j), (i, j +) и (i, j -)
            for(auto & side: sides)
            {
                lambda_oil[side] = middle_point(k_relat_oil, node_x, node_y, side) / viscosity_oil;
                lambda_water[side] = middle_point(k_relat_water, node_x, node_y, side) / viscosity_water;
            }

            // Капиллярное давление в точках (i +, j), (i -, j),  (i, j +) и (i, j -)
            Point<double> press;
            press[X_PLUS] = (border[X_PLUS]) ? 0.0 : capillary_press[node_x + 1][node_y] - capillary_press[node_x    ][node_y];
            press[X_MINUS] = (border[X_MINUS]) ? 0.0 : capillary_press[node_x    ][node_y] - capillary_press[node_x - 1][node_y];
            press[Y_PLUS] = (border[Y_MINUS]) ? 0.0 : capillary_press[node_x][node_y + 1] - capillary_press[node_x][node_y    ];
            press[Y_MINUS] = (border[Y_PLUS]) ? 0.0 : capillary_press[node_x][node_y    ] - capillary_press[node_x][node_y - 1];

            Point<double> grav_press;
            // Давление в следствие гравитации в точках (i +, j), (i -, j),  (i, j +) и (i, j -)
            grav_press[X_PLUS] = (border[X_PLUS]) ? 0.0 : gravity * (heights[node_x + 1][node_y] - heights[node_x    ][node_y]);
            grav_press[X_MINUS] = (border[X_MINUS]) ? 0.0 : gravity * (heights[node_x    ][node_y] - heights[node_x - 1][node_y]);
            grav_press[Y_PLUS] = (border[Y_MINUS]) ? 0.0 : gravity * (heights[node_x][node_y + 1] - heights[node_x][node_y    ]);
            grav_press[Y_MINUS] = (border[Y_PLUS]) ? 0.0 : gravity * (heights[node_x][node_y    ] - heights[node_x][node_y - 1]);

            // Заполнение матрицы
            // TODO: убрать проверки (лишние)
            Point<double> temp;
            temp[X_PLUS] = (border[X_PLUS]) ? 0.0 : T_coeff[X_PLUS] * (lambda_oil[X_PLUS] + lambda_water[X_PLUS]);
            temp[X_MINUS] = (border[X_MINUS]) ? 0.0 : T_coeff[X_MINUS] * (lambda_oil[X_MINUS] + lambda_water[X_MINUS]);
            temp[Y_PLUS] = (border[Y_MINUS]) ? 0.0 : T_coeff[Y_PLUS] * (lambda_oil[Y_PLUS] + lambda_water[Y_PLUS]);
            temp[Y_MINUS] = (border[Y_PLUS]) ? 0.0 : T_coeff[Y_MINUS] * (lambda_oil[Y_MINUS] + lambda_water[Y_MINUS]);
            temp[X_Y]  = - (temp[X_PLUS] + temp[X_MINUS] + temp[Y_PLUS] + temp[Y_MINUS]);

            for(auto & side: sides)
                if(temp[side] != 0.0) tripletList.push_back(Trip(index[side], index[X_Y], temp[side]));
            if (temp[X_Y] != 0.0) tripletList.push_back(Trip(index[X_Y], index[X_Y], temp[X_Y]));

            // Заполнение правой части
            temp[X_PLUS] = (border[X_PLUS]) ? 0.0 : T_coeff[X_PLUS] * (grav_press[X_PLUS] * (density_oil * lambda_oil[X_PLUS] + density_water * lambda_water[X_PLUS]) + lambda_water[X_PLUS] * press[X_PLUS]);
            temp[X_MINUS] = (border[X_MINUS])  ? 0.0 : T_coeff[X_MINUS] * (grav_press[X_MINUS] * (density_oil * lambda_oil[X_MINUS] + density_water * lambda_water[X_MINUS]) + lambda_water[X_MINUS] * press[X_MINUS]);
            temp[Y_PLUS] = (border[Y_MINUS])  ? 0.0 : T_coeff[Y_PLUS] * (grav_press[Y_PLUS] * (density_oil * lambda_oil[Y_PLUS] + density_water * lambda_water[Y_PLUS]) + lambda_water[Y_PLUS] * press[Y_PLUS]);
            temp[Y_MINUS] = (border[Y_PLUS])    ? 0.0 : T_coeff[Y_MINUS] * (grav_press[Y_MINUS] * (density_oil * lambda_oil[Y_MINUS] + density_water * lambda_water[Y_MINUS]) + lambda_water[Y_MINUS] * press[Y_MINUS]);

            pres_vec(index[X_Y]) = - temp[X_PLUS] + temp[X_MINUS] - temp[Y_PLUS] + temp[Y_MINUS];
        };


    pres_mat.setFromTriplets(tripletList.begin(), tripletList.end());
    mat_solver.compute(pres_mat);
    mat_solver.analyzePattern(pres_mat);
    qDebug(logSolve()) << "Вызываем решатель";
    // Получить вектор решений
    Eigen::VectorXd solution(nx * ny);
    solution = mat_solver.solve(pres_vec);
    for (int node_x = 0; node_x < nx; node_x++)
        for (int node_y = 0; node_y < ny; node_y++)
            oil_press_next[node_x][node_y] = solution(node_x * ny + node_y);

    // Вывод отладочной информации
    qDebug(logSolve()) << "Количество итераций" << mat_solver.iterations();
    qDebug(logSolve()) << "Норма матрицы" << mat_solver.error();
}

void Solver::explicit_scheme_calc()
{
    // TODO: рассчитать все параметры, зависящие от водонасыщенности s_w, при s_w = s_w(t_{n + 1})
    fill_data();
    qInfo(logSolve()) << "Начинаем расчет по явной схеме";
    for (int node_x = 0; node_x < nx; node_x++)
        for (int node_y = 0; node_y < ny; node_y++)
        {
            // Флаг, определяющий принадлежность элемента границе сетки
            Point<bool> border;
            border[X_PLUS] = (node_x == nx - 1);
            border[X_MINUS]  = (node_x == 0);
            border[Y_MINUS]  = (node_y == ny - 1);
            border[Y_PLUS]  = (node_y == 0);

            Point<bool> T_coeff;
            // Коэффициенты T в точках (i +, j) и (i -, j) (здесь и далее см. обозн. в схеме Емченко)
            T_coeff[X_PLUS] = dy * dz / dx * middle_point(k_absol, node_x, node_y, X_PLUS);
            T_coeff[X_MINUS] = dy * dz / dx * middle_point(k_absol, node_x, node_y, X_MINUS);

            // Коэффициенты T в точках (i, j +) и (i, j -)
            T_coeff[Y_PLUS] = dx * dz / dy * middle_point(k_absol, node_x, node_y, Y_PLUS);
            T_coeff[Y_MINUS] = dx * dz / dy * middle_point(k_absol, node_x, node_y, Y_MINUS);

            Point<double> lambda;
            // Коэффициенты lambda в точках (i +, j) и (i -, j)
            for(auto & side: sides)
                lambda[side] = middle_point(k_relat_water, node_x, node_y, side) / viscosity_water / compress_water;

            // TODO: подумать, как работать с давлением воды (вводить его или пересчитывать через капиллярное давление и давление нефти)
            Point<double> press;
            // Давление воды в точках (i +, j) и (i -, j)
            press[X_PLUS] = (border[X_PLUS]) ? 0.0 : (capillary_press[node_x + 1][node_y] - capillary_press[node_x    ][node_y]) - (oil_press_next[node_x + 1][node_y] - oil_press_next[node_x    ][node_y]);
            press[X_MINUS] = (border[X_MINUS]) ? 0.0 : (capillary_press[node_x    ][node_y] - capillary_press[node_x - 1][node_y]) - (oil_press_next[node_x    ][node_y] - oil_press_next[node_x - 1][node_y]);

            // Давление воды в точках (i, j +) и (i, j -)
            press[Y_PLUS] = (border[Y_MINUS]) ? 0.0 : (capillary_press[node_x][node_y + 1] - capillary_press[node_x][node_y    ]) - (oil_press_next[node_x][node_y + 1] - oil_press_next[node_x][node_y    ]);
            press[Y_MINUS] = (border[Y_PLUS]) ? 0.0 : (capillary_press[node_x][node_y    ] - capillary_press[node_x][node_y - 1]) - (oil_press_next[node_x][node_y    ] - oil_press_next[node_x][node_y - 1]);

            Point<double> potential;
            // Потенциал Phi в точках (i +, j) и (i -, j)
            potential[X_PLUS] = (border[X_PLUS]) ? 0.0 : press[X_PLUS] - gravity * density_water * (heights[node_x + 1][node_y] - heights[node_x    ][node_y]);
            potential[X_MINUS] = (border[X_MINUS]) ? 0.0 : press[X_MINUS] - gravity * density_water * (heights[node_x    ][node_y] - heights[node_x - 1][node_y]);

            // Потенциал Phi в точках (i, j +) и (i, j -)
            potential[Y_PLUS] = (border[Y_MINUS]) ? 0.0 : press[Y_PLUS] - gravity * density_water * (heights[node_x][node_y + 1] - heights[node_x][node_y    ]);
            potential[Y_MINUS] = (border[Y_PLUS]) ? 0.0 : press[Y_MINUS] - gravity * density_water * (heights[node_x][node_y    ] - heights[node_x][node_y - 1]);

            // Слагаемые, полученные при разложении интеграла
            Point<double> coeff;
            for(auto & side: sides)
                coeff[side] = T_coeff[side] * lambda[side] * potential[side];

            // Рассчитываем водонасыщенность s_w на шаге t_{n + 1}    

            if(border[X_PLUS] || border[X_MINUS] || border[Y_PLUS] || border[Y_MINUS])
                s_water_next[node_x][node_y] = 0;
            else
            {
                Q_ASSERT(porosity[node_x][node_y] != 0);
                s_water_next[node_x][node_y] = dt * compress_oil / porosity[node_x][node_y] / (dx * dy * dz) * (coeff[X_PLUS] - coeff[X_MINUS] + coeff[Y_PLUS] - coeff[Y_MINUS]);
            }
        };
    qDebug(logSolve()) << "Значение водонасыщенности найдено";
}

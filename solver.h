#ifndef SOLVER_H
#define SOLVER_H

// Стандартные библиотеки C++
#include <vector>
#include "logger.h"

/*!
 \class Solver
 \brief Класс, в котором будут происходить вычисления
*/
class Solver
{
private:

    // Постоянные параметры
    double porosity; // пористость []
    double compress_oil; // коэффициент сжимаемости нефти [1 / атм]
    double compress_water; // коэффициент сжимаемости воды [1 / атм]
    double viscosity_oil; // вязкость нефти [Па с]
    double viscosity_water; // вязкость воды [Па с]
    double density_oil; // плотность нефти [кг / м^3]
    double density_water; // плотность воды [кг / м^3]
    double gravity; // ускорение свободного падения [м / с^2]

    // Параметры, зависящие от водонасыщенности s_w
    std::vector< std::vector<double> > k_absol; // абсолютная проницаемость [1 Д = 1 мкм^2]
    std::vector< std::vector<double> > k_relat_oil; // относительная проницаемость нефти []
    std::vector< std::vector<double> > k_relat_water; // относительная проницаемость воды []
    std::vector< std::vector<double> > heights; // высоты D(x, y) [1 м]
    std::vector< std::vector<double> > capillary_press; // капиллярное давление [1 Па]

    // Искомые величины
    // TODO: возможно, нужно отделить данные на предыдущ. и следующ. внутренних итерациях в отдельный класс InnerState
    std::vector< std::vector<double> > oil_press; // давление нефти [1 Па]
    std::vector< std::vector<double> > water_press; // давление воды [1 Па]
    std::vector< std::vector<double> > oil_press_prev;
    std::vector< std::vector<double> > oil_press_next;
    std::vector< std::vector<double> > water_press_prev;
    std::vector< std::vector<double> > water_press_next;

    // Шаги
    double dx; // Шаг сетки по оси OX [1 м]
    double dy; // Шаг сетки по оси OY [1 м]
    double dz; // Шаг сетки по оси OZ [1 м]
    double dt; // Шаг по времени [1 с]

    // Разное
    // TODO: правильно "распределить" переменные
    double T; // Конечное время расчёта [с]
    double num_global_steps; // Число глобальных шагов
    double epsilon; // Число epsilon (погрешность)

public:

    /*!
     * \brief Конструктор
     */
    Solver();

    /*!
     * \brief Получить решение задачи
     */
    void solve();
    /*!
     * \brief Выполнить внутренние итерации
     * \param begin_time - начальное время расчёта [с]
     * \param end_time - конечное время расчёта [с]
     */
    void inner_solve(double begin_time, double end_time);
    /*!
     * \brief Рассчитать невязку
     * \return Значение невязки
     */
    double calc_residual();

};

#endif // SOLVER_H

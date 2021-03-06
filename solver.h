#ifndef SOLVER_H
#define SOLVER_H

// Стандартные библиотеки C++
#include <vector>
#include "logger.h"

typedef QMap<QString, QVector<double>> input_data_type;
typedef std::vector< std::vector<double> > vector_double_2d;

/*!
  \class Well
  \brief Класс для хранения структур для скважины
*/
class Well
{
public:
    Well();
    int ix;
    int iy;
    std::vector<double> values; //! для добывающей - дебит, для нагнетательной приемистость

    /*!
     * \brief Задать расположение скважины
     * \param _ix - координата по OX [0;nx - 1]
     * \param _iy - координата по OY [0;ny - 1]
     */
    inline void set_location(int _ix, int _iy){ ix = _ix; iy = _iy;}
};

/*!
 \class Solver
 \brief Класс, в котором будут происходить вычисления
*/
class Solver
{
private:

    // Постоянные параметры
    double compress_oil; // коэффициент сжимаемости нефти [1 / атм]
    double compress_water; // коэффициент сжимаемости воды [1 / атм]
    double viscosity_oil; // вязкость нефти [Па с]
    double viscosity_water; // вязкость воды [Па с]
    double density_oil; // плотность нефти [кг / м^3]
    double density_water; // плотность воды [кг / м^3]
    double gravity; // ускорение свободного падения [м / с^2]
    double rock; //! коэффициент сжимаемости породы
    double b_water; //! коэффициент объемного расширения воды
    double b_oil; //! коэффициент объемного расширения нефти

    // Параметры, зависящие от водонасыщенности s_w
    vector_double_2d k_absol; // абсолютная проницаемость [1 Д = 1 мкм^2]
    vector_double_2d k_relat_oil; // относительная проницаемость нефти []
    vector_double_2d k_relat_water; // относительная проницаемость воды []
    vector_double_2d heights; // высоты D(x, y) [1 м]
    vector_double_2d capillary_press; // капиллярное давление [1 Па]
    vector_double_2d s_water_init; //! начальная водонасыщенность []
    vector_double_2d s_water; //! водонасыщенность


    // Искомые величины
    // TODO: возможно, нужно отделить данные на предыдущ. и следующ. внутренних итерациях в отдельный класс InnerState
    vector_double_2d oil_press; // давление нефти [1 Па]
    vector_double_2d water_press; // давление воды [1 Па]
    vector_double_2d oil_press_prev;
    vector_double_2d oil_press_next;
    vector_double_2d water_press_prev;
    vector_double_2d water_press_next;
    vector_double_2d pressure; //! давление
    vector_double_2d tops; //! глубины ячеек
    vector_double_2d porosity; //! пористость
    vector_double_2d permx; //! проницаемость по OX
    vector_double_2d permy; //! проницаемость по OY

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
    int nx; //! Количество ячеек по оси X
    int ny; //! Количество ячеек по оси Y
    int nz; //! Количество ячеек по оси Z
    Well prod1; //! Первая добывающая скважина
    Well prod2; //! Вторая добывающая скважина
    Well inj1; //! Первая нагнетательная скважина
    Well inj2; //! Вторая нагнетательная скважина

    std::vector<double> sw_init;
    std::vector<double> krw_init;
    std::vector<double> krow_init;
    std::vector<double> pcow_init;

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

    /*!
     * \brief Инициализация данных
     * \param input_data - карта по имени ключевого слова выдать массив входных данных
     */
    void init(input_data_type &input_data);

    void init_wells(input_data_type &input_data);

    void init_swof(input_data_type &input_data);

    void init_array(QVector<double> &data , vector_double_2d *arr);

};

#endif // SOLVER_H

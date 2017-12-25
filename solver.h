#ifndef SOLVER_H
#define SOLVER_H

// Стандартные библиотеки C++
#include <vector>
#include <iostream>
#include <array>
#include <sstream>
#include <QApplication>
#include "logger.h"
#include "interpol.h"

typedef QMap<QString, QVector<double>> input_data_type;
typedef std::vector< std::vector<double> > vector_double_2d;

enum NODE_SIDE
{
    X_PLUS,
    X_MINUS,
    Y_PLUS,
    Y_MINUS,
    X_Y
};

template<class Type>
using Point = std::array<Type, 5>;

template<class Type>
QString print(Point<Type> &p, const QString &name)
{
    QString result_string;
    QTextStream result(&result_string);
    result << QString("%1[%2] = ").arg(name,QString("X_PLUS")) << p[X_PLUS] << " ";
    result << QString("%1[%2] = ").arg(name,QString("X_MINUS")) << p[X_MINUS] << " ";
    result << QString("%1[%2] = ").arg(name,QString("Y_PLUS")) << p[Y_PLUS] << " ";
    result << QString("%1[%2] = ").arg(name,QString("Y_MINUS")) << p[Y_MINUS] << " ";
    return result_string;
    //result << QString("%1[%2] = ").arg(name,QString("X_Y")).toStdString() << p[X_Y] << endl;
}


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
    double viscosity_oil; // вязкость нефти [Па с]
    double viscosity_water; // вязкость воды [Па с]
    double density_oil; // плотность нефти [кг / м^3]
    double density_water; // плотность воды [кг / м^3]
    double gravity; // ускорение свободного падения [м / с^2]
    double rock; //! коэффициент сжимаемости породы
    double b_water; // коэффициент объемного расширения воды []
    double b_oil; // коэффициент объемного расширения нефти []

    // Параметры, зависящие от водонасыщенности s_w
    vector_double_2d k_absol; // абсолютная проницаемость [1 Д = 1 мкм^2]
    vector_double_2d k_relat_oil; // относительная проницаемость нефти []
    vector_double_2d k_relat_water; // относительная проницаемость воды []
    vector_double_2d heights; // высоты D(x, y) [1 м]
    vector_double_2d capillary_press; // капиллярное давление [1 Па]
    vector_double_2d porosity; // пористость

    // Начальные данные
    vector_double_2d s_water_init; // начальная водонасыщенность []
    vector_double_2d oil_press_init; // начальное распределение давления нефти [1 Па]

    // Искомые величины (найденное сошедшееся решение)
    vector_double_2d s_water; // текущая водонасыщенность
    vector_double_2d oil_press; // текущее давление нефти [1 Па]

    // Данные, полученные на внутренних итерациях
    vector_double_2d oil_press_prev; // давление нефти на n внутренней итерации [1 Па]
    vector_double_2d oil_press_next; // давление нефти на n + 1 внутренней итерации [1 Па]
    vector_double_2d s_water_prev; // водонасыщенность на n внутренней итерации [1 Па]
    vector_double_2d s_water_next; // водонасыщенность на n + 1 внутренней итерации [1 Па]

    // TODO: убрать за ненадобностью
    vector_double_2d permx; //! проницаемость по OX
    vector_double_2d permy; //! проницаемость по OY

    // Шаги
    double dx; // Шаг сетки по оси OX [1 м]
    double dy; // Шаг сетки по оси OY [1 м]
    double dz; // Шаг сетки по оси OZ [1 м]
    double dt; // Шаг по времени [1 с]
    int step; //! текущий номер глобального шага по времени

    // Разное
    // TODO: правильно "распределить" переменные
    double T; // Конечное время расчёта [1 с]
    double num_global_steps; // Число глобальных шагов
    double epsilon_press; // Число epsilon (погрешность давления)
    double epsilon_swater; // Число epsilon (погрешность водонасыщенности)
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
    L_Interpol krw_inter;
    L_Interpol krow_inter;
    L_Interpol pcow_inter;
    
    static const int MAX_INNER_ITERATIONS = 20; // Максимальное число внутренних итераций
    const std::array<int, 4> sides { { NODE_SIDE::X_PLUS, NODE_SIDE::X_MINUS, NODE_SIDE::Y_PLUS, NODE_SIDE::Y_MINUS } };
    bool sw_over_1; // Флаг, проверяющий, стала ли насыщенность больше 1
    double prod_con1, prod_con2;
    double inj_con1, inj_con2;

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
     * \param
     * \param
     */
    void calc_residual(double& residual_press, double& residual_swater);
    /*!
     * \brief Рассчитать по явной схеме водонасыщенность
     */
    void explicit_scheme_calc();
    /*!
     * \brief Рассчитать по явной схеме водонасыщенность
     */
    void implicit_scheme_calc();

    /*!
     * \brief Инициализация данных
     * \param input_data - карта по имени ключевого слова выдать массив входных данных
     */
    void init(input_data_type &input_data);

    void init_wells(input_data_type &input_data);

    void init_swof(input_data_type &input_data);

    void init_array(QVector<double> &data , vector_double_2d *arr);

    void fill_data();

    void set_dimens(int t_nx, int t_ny){nx = t_nx; ny = t_ny;}
    double middle_point(vector_double_2d &arr, int i, int j, const int side);

};

#endif // SOLVER_H

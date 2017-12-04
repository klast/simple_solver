#ifndef READER_H
#define READER_H
#include <QFile>
#include <QString>
#include <QIODevice>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QByteArray>
#include <QTextStream>
#include <QMap>
#include "logger.h"

enum filetypes
{
    init_well = 0,
    SCAL = 1,
    PVT = 2,
    GRID = 3,
    INIT = 4,
    GPRO = 5
};

/*!
 \class Reader
 \brief Класс открытия и считывания DATA файла
*/
class Reader
{
public:

    /*!
     * \brief Конструктор
     */
    Reader();

    /*!
     * \brief Задаем файл
     * \param type - тип файла
     * \param _filename - имя открываемого файла
     * \return Открылся ли data файл
     */
    bool set_file(filetypes type, QString &_filename);

    /*!
     * \brief Чтение всей модели
     */
    void read();

    /*!
     * \brief Чтение массива
     * \param keyword_name - Название ключевого слова
     */
    void read_1d_array(QString keyword_name);
    //! TODO: переписать данный код с учетом 4-х файлов
    //! Закрытие файла
    void close();
    float nx, ny;//! размерности моделей
    QString title;//! название модели, нужно ли оно???

    QVector<QSharedPointer<QFile>> input_files;
    QMap<QString, QVector<double>> input_1d_arrays;//! входные массивы
    QMap<QString, double> input_constants;//! входные константы
    QFile * current_file;//! входные 2 файла, переделать под 4
    QDir model_directory;//! директория в которой находится data файл
};

#endif // READER_H

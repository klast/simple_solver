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

    //! TODO: переписать данный код с учетом 4-х файлов
    /*!
     * \brief Открыть файл
     * \param _filename - имя открываемого файла
     * \return Открылся ли файл
     */
    bool open(QString &_filename);

    /*!
     * \brief Задаем data файл
     * \param _filename - имя открываемого файла
     * \return Открылся ли data файл
     */
    bool set_datafile(QString &_filename);

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


    QMap<QString, QVector<float>> input_1d_arrays;//! входные массивы
    QMap<QString, float> input_constants;//! входные константы
    QFile datafile, current_file;//! входные 2 файла, переделать под 4
    QDir model_directory;//! директория в которой находится data файл
};

#endif // READER_H

#ifndef READER_H
#define READER_H
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QIODevice>
#include <QDebug>
#include <QMap>

/*!
 \class Reader
 \brief Класс открытия и считывания DATA файла
*/
class Reader
{
public:
    //! Конструктор по умолчанию
    Reader();
    //! Открытие файла
    bool open(QString &_filename);
    bool set_datafile(QString &_filename);
    //! Закрытие файла
    void close();

    QMap<QString, QVector<QVector<float>>> input_2d_arrays;
    QMap<QString, QVector<float>> input_1d_arrays;
    QMap<QString, float> input_constants;
    QFile datafile, current_file;
    QDir datafile_directory;
};

#endif // READER_H

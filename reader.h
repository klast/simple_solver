#ifndef READER_H
#define READER_H
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QIODevice>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QByteArray>
#include <QTextStream>
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
    float nx, ny;
    QString title;
    //или std::string, как хочешь
    void read();
    void read_datafile();
    void read_1d_array(QString keyword_name);
    QMap<QString, QVector<float>> input_1d_arrays;
    QMap<QString, float> input_constants;
    QFile datafile, current_file;
    //! будут 4 файла вроде
    QFile second_file, third_file, fourth_file;
    QDir model_directory;

};

#endif // READER_H

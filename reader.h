#ifndef READER_H
#define READER_H
#include <QFile>
#include <QString>
#include <QIODevice>
#include <QDebug>
#include <QByteArray>
#include <QTextStream>

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
    bool open(QString _filename);
    //! Закрытие файла
    void close();
    double nx, ny;
    QString title;
    //или std::string, как хочешь
    void read();
private:
    QFile datafile;
};

#endif // READER_H

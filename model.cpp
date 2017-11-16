#include "model.h"

Model::Model()
{
    nx = 100;
    ny = 100;
    nt = 15;
}

bool Model::open(QString &_filename)
{
    if (reader.open(_filename))
    {
        qDebug() << "Файл открыт";
        return true;
    }
    else
    {
        qDebug() << "Ошибка открытия файла";
        return false;
    }
}

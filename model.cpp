#include "model.h"

Model::Model()
{

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

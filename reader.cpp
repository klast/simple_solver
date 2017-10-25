#include "reader.h"

Reader::Reader()
{

}

/*!
  \brief Функция открытия файла
  \param _filename - имя файла
 */
bool Reader::open(QString _filename)
{
    datafile.setFileName(_filename);
    //Пытаемся открыть файл в режиме для чтения
    if(!datafile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Ошибка открытия файла";
        return false;
    }
    else
    {
        qDebug() << "Файл открыт";
        return true;
    }
}

void Reader::close()
{
    datafile.close();
}

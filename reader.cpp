#include "reader.h"

Reader::Reader()
{

}

/*!
  \brief Функция открытия файла
  \param _filename - имя файла
 */
bool Reader::open(QString &_filename)
{
    datafile.setFileName(_filename);
    bool is_opened = datafile.open(QIODevice::ReadOnly);
    //Пытаемся открыть файл в режиме для чтения
    if(!is_opened)
    {
        return false;
    }
    else
    {
        QFileInfo info1(datafile);
        datafile_directory = info1.absoluteDir();
        return true;
    }
}

void Reader::close()
{
    datafile.close();
}

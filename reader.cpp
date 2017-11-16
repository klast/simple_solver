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
    current_file.setFileName(_filename);
    bool is_opened = current_file.open(QIODevice::ReadOnly);
    //Пытаемся открыть файл в режиме для чтения
    if(!is_opened)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Reader::set_datafile(QString &_filename)
{
    if(open(_filename))
    {
        datafile.setFileName(_filename);
        current_file.close();
        datafile.open(QIODevice::ReadOnly);
        QFileInfo info1(datafile);
        datafile_directory = info1.absoluteDir();
        return true;
    }
    else
        return false;
}

void Reader::close()
{
    datafile.close();
}

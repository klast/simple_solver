#include "reader.h"


Reader::Reader()
{

}

bool Reader::set_datafile(QString &_filename)
{
    if(open(_filename))
    {
        current_file.close();
        datafile.setFileName(_filename);
        datafile.open(QIODevice::ReadOnly);
        QFileInfo info1(datafile);
        model_directory = info1.absoluteDir();
        return true;
    }
    else
        return false;
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
        qDebug() << "Ошибка открытия файла";
        return false;
    }
    else
    {
        qDebug() << "Файл открыт";
        return true;
    }    
}
void Reader::read()
{
    read_datafile();
    //! потом считать остальные файлы, когда узнаем спецификации
}

void Reader::read_datafile()
{
    QString str="";
    QString s, s1;
    while (!datafile.atEnd())
    {
        str = datafile.readLine();
        if (str.contains ("title", Qt::CaseInsensitive))
        {
             title = datafile.readLine();
             title = title.simplified();
             title = title.remove(QChar('"'), Qt::CaseInsensitive);
        }
        else if (str.contains ("dimens", Qt::CaseInsensitive))
        {
             s = datafile.readLine();
             s = s.trimmed();
             s = s.prepend(" ");
             s1 = s.section(' ', 1, 1);
             nx = s1.toFloat();
             s1 = s.section(' ', 2, 2);
             ny = s1.toFloat();
             input_constants["nx"] = nx;
             input_constants["ny"] = ny;
        }
        else if(str.contains("poro", Qt::CaseInsensitive))
        {
             read_1d_array("poro");
        }
        else if(str != "\r\n")
        {
             qDebug() << "Необработанная строчка + " << str;
        }
    }
    qDebug() << title;
    qDebug() << nx;
    qDebug() << ny;
    qDebug() << "Код сюда дошел";
}

void Reader::read_1d_array(QString keyword_name)
{
    QVector<float> this_array;
    QString tmp; float p;
    while(true)
    {
        tmp = datafile.readLine();
        if (tmp[0] == "/")
            break;
        else
        {
            QStringList my_row = tmp.split(' ');
            for(QString item: my_row)
            {
                p = item.toFloat();
                this_array.push_back(p);
            }
        }
    }
    qDebug() << this_array;
    input_1d_arrays[keyword_name] = this_array;
}

void Reader::close()
{
    datafile.close();
    current_file.close();
}



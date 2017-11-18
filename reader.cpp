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
        read();
        return true;
    }    
}
void Reader::read()
{
     QString str="";
     QString s, s1;
     while (!datafile.atEnd())
     {
         str = datafile.readLine();
         if (str.contains ("title", Qt::CaseInsensitive)) {
                 title = datafile.readLine();
                 title = title.simplified();
                 title = title.remove(QChar('"'), Qt::CaseInsensitive);
             }
          else if (str.contains ("dimens", Qt::CaseInsensitive))  {
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
    /*
        считываешь также из datafile пока что
        Формат данных будет скорее всего таким
        PORO
        2 3 5 6 7 1 2
        4 7 2 5 1 3 123
        1 2 3 4
        /
    */
    // вот такой строкой ниже = добавляешь один элемент в массив
    // вместо двойки подставляй то, что обработала
    this_array.push_back(2);
    //считывай до того момента пока не будет /
    input_1d_arrays[keyword_name] = this_array;
}

void Reader::close()
{
    datafile.close();
}



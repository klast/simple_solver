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
          if (str.contains ("dimens", Qt::CaseInsensitive))  {
                 s = datafile.readLine();
                 s = s.trimmed();
                 s = s.prepend(" ");
                 s1 = s.section(' ', 1, 1);
                 nx = s1.toDouble();
                 s1 = s.section(' ', 2, 2);
                 ny = s1.toDouble();
             }
     }
   qDebug() << title;
   qDebug() << nx;
   qDebug() << ny;
   qDebug() << "Код сюда дошел";
}

void Reader::close()
{
    datafile.close();
}



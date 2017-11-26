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
    QString tmp, s, s1; float p, pp; int i; p=0;
    while(true)
    {
        tmp = datafile.readLine();
        if (tmp[0] == "/") break;
        else{
            QStringList my_row = tmp.split(' ');
            for(QString item: my_row)
            {
                if (!tmp.contains("*") ){
                    p = item.toFloat();
                    this_array.push_back(p);
                }
                else {
                    QStringList my_row = item.split('*');
                      for(QString item: my_row){
                       s.append(item+" ");}
                       s = s.prepend(" ");
                       s1 = s.section(' ', 1, 1);
                        p = s1.toFloat();
                        s1 = s.section(' ', 2, 2);
                        pp = s1.toFloat();
                        for (i = int(p); i!=0; i--){
                        this_array.push_back(pp);
                        }s.clear();
                       }
                      }
                     }
                    }

    qDebug() << this_array;
    input_1d_arrays[keyword_name] = this_array;
}

void Reader::close()
{
    datafile.close();
}



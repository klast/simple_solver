#include "reader.h"


Reader::Reader()
{
    input_files = QVector<QSharedPointer<QFile>>(6);
}

bool Reader::set_file(filetypes type, QString &_filename)
{
    QSharedPointer<QFile> this_file(new QFile(_filename));
    bool is_opened = this_file->open(QIODevice::ReadOnly);
    if(!is_opened)
    {
        qCritical(logRead()) << "Ошибка открытия файла " << _filename;
        return false;
    }
    else
    {
        qInfo(logRead()) << "Файл " << _filename << "задан";
        input_files.insert(type, this_file);
        return true;
    }
}

void Reader::read()
{
     QString str="";
     QString s, s1;
     current_file = input_files[0].data();
     read_1d_array("wellinfo");
     for(int i = 1; i < input_files.size(); i++)
     {
         if(!input_files[i])
             continue;
         current_file = input_files[i].data();
         while(!current_file->atEnd())
         {
             str = current_file->readLine();
             if(str.contains("swof", Qt::CaseInsensitive))
             {
                 read_1d_array("swof");
             }
             else if(str.contains("rock", Qt::CaseInsensitive))
             {
                 read_1d_array("rock");
             }
             else if(str.contains("pvtw", Qt::CaseInsensitive))
             {
                 read_1d_array("pvtw");
             }
             else if(str.contains("pvdo", Qt::CaseInsensitive))
             {
                 read_1d_array("pvdo");
             }
             else if(str.contains("density", Qt::CaseInsensitive))
             {
                 read_1d_array("density");
             }
             else if(str.contains("tops", Qt::CaseInsensitive))
             {
                 read_1d_array("tops");
             }
             else if(str.contains("pressure", Qt::CaseInsensitive))
             {
                 read_1d_array("pressure");
             }
             else if(str.contains("poro", Qt::CaseInsensitive))
             {
                 read_1d_array("poro");
             }
             else if(str.contains("permx", Qt::CaseInsensitive))
             {
                 read_1d_array("permx");
             }
             else if(str != "\r\n")
             {
                  qInfo(logRead()) << "Необработанная строчка" << str;
             }
         }
     }
     qInfo(logRead()) << "Считывание закончено";
}

void Reader::read_1d_array(QString keyword_name)
{
    QVector<float> this_array;
    QString tmp, s, s1;
    float p, pp;
    int i;
    p=0;
    while(true)
    {
        tmp = current_file->readLine();
        if (tmp[0] == "/" || current_file->atEnd()) break;
        else
        {
            QStringList my_row = tmp.split(' ');
            for(QString item: my_row)
            {
                if (!tmp.contains("*") )
                {
                    p = item.toFloat();
                    this_array.push_back(p);
                }
                else
                {
                    QStringList my_row = item.split('*');
                    for(QString item: my_row)
                    {
                        s.append(item+" ");
                    }
                    s = s.prepend(" ");
                    s1 = s.section(' ', 1, 1);
                    p = s1.toFloat();
                    s1 = s.section(' ', 2, 2);
                    pp = s1.toFloat();
                    for (i = int(p); i!=0; i--)
                    {
                        this_array.push_back(pp);
                    }
                    s.clear();
                }
            }
        }
    }
    qInfo(logRead()) << keyword_name << "= QVector of size ==" << this_array.size();
    input_1d_arrays[keyword_name] = this_array;
}

void Reader::close()
{

}



#include "reader.h"
#include <QRegExp>

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
     qInfo(logRead()) << "НАЧИНАЕМ СЧИТЫВАНИЕ";
     QString str="";
     QString s, s1;
     current_file = input_files[0].data();
     qInfo(logRead()) << "ОТКРЫТ ФАЙЛ" << current_file->fileName();
     read_1d_array("wellinfo");
     for(int i = 1; i < input_files.size(); i++)
     {
         if(!input_files[i])
             continue;
         current_file = input_files[i].data();
         qInfo(logRead()) << "ОТКРЫТ ФАЙЛ" << current_file->fileName();
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
                  qWarning(logRead()) << "необработанная строчка" << str;
             }
         }
     }
     qInfo(logRead()) << "СЧИТЫВАНИЕ ЗАКОНЧЕНО";
}

void Reader::read_1d_array(QString keyword_name)
{
    QVector<double> this_array;
    QString tmp, s, s1;
    double p, pp;
    int i;
    p=0;
    qDebug(logRead()) << "Считываю" << keyword_name;
    while(true)
    {
        if(current_file->atEnd())
            break;
        tmp = current_file->readLine();
        if (tmp[0] == "/")
            break;
        else
        {
            QStringList my_row = tmp.split(QRegExp("\\s"));
            for(QString item: my_row)
            {
                if(item == "")
                    continue;
                if (!item.contains("*") )
                {
                    p = item.toDouble();
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
                    p = s1.toDouble();
                    s1 = s.section(' ', 2, 2);
                    pp = s1.toDouble();
                    for (i = int(p); i!=0; i--)
                    {
                        this_array.push_back(pp);
                    }
                    s.clear();
                }
            }
        }
    }
    qInfo(logRead()) << keyword_name << "- одномерный массив размером " << this_array.size() << "элементов";
    input_1d_arrays[keyword_name] = this_array;
}

void Reader::close()
{

}



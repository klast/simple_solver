#include "model.h"

Model::Model()
{
    nx = 100;
    ny = 100;
    nt = 1000;
}

int Model::simulate()
{
    qDebug() << "Круто!";
    read();
    init();
    return 1;
    //solve();
}

void Model::read()
{
    logFile.reset(new QFile(reader.input_files[0]->fileName().split('.').at(0) + ".log"));
    logFile.data()->open(QFile::WriteOnly | QFile::Text);
    qInstallMessageHandler(Model::messageHandler);
    reader.read();
}

void Model::init()
{
    nx = reader.input_constants["nx"];
    ny = reader.input_constants["ny"];
    solver.init(reader.input_1d_arrays);
}

void Model::solve()
{
    solver.solve();
}

void Model::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(logFile.data());
    out << QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz ");
    switch(type)
    {
        case QtInfoMsg:     out << "| INFO     "; break;
        case QtDebugMsg:    out << "| DEBUG    "; break;
        case QtWarningMsg:  out << "| WARNING  "; break;
        case QtCriticalMsg: out << "| CRITICAL "; break;
        case QtFatalMsg:    out << "| FATAL    "; break;
    }
    out << "| " << context.category <<"| " << msg << endl;
    out.flush();
}


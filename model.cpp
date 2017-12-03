#include "model.h"

Model::Model()
{
    nx = 100;
    ny = 100;
    nt = 15;
}

void Model::simulate()
{
    read();
    init();
    //solve();
}

void Model::read()
{
    logFile.reset(new QFile(reader.datafile.fileName().split('.').at(0) + ".log"));
    logFile.data()->open(QFile::WriteOnly | QFile::Text);
    qInstallMessageHandler(Model::messageHandler);
    reader.read();
}

void Model::init()
{
    nx = reader.input_constants["nx"];
    ny = reader.input_constants["ny"];
    // TODO: вычислить шаг по времени по условию Куранта
}

void Model::solve()
{
    solver.solve();
}

void Model::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(logFile.data());
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ");
    switch(type)
    {
        case QtInfoMsg:     out << "INFO "; break;
        case QtDebugMsg:    out << "DEG "; break;
        case QtWarningMsg:  out << "WRN "; break;
        case QtCriticalMsg: out << "CRT "; break;
        case QtFatalMsg:    out << "FTL "; break;
    }
    out << context.category <<": " << msg << endl;
    out.flush();
}


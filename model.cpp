#include "model.h"
#include <iostream>

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
    solve();
    return 1;
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
    solver.set_dimens(reader.input_constants["nx"], reader.input_constants["ny"]);
    solver.init(reader.input_1d_arrays);
#ifdef HDF5_SOLVE
    solver.hdf5_worker.datafile_name = reader.input_files[0]->fileName();
#endif
}

void Model::solve()
{
    solver.solve();
}

void Model::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    bool is_debug;
//#define QT_DEBUG_PRINT
#ifdef QT_DEBUG_PRINT
    is_debug = true;
#else
    is_debug = false;
#endif
    QTextStream out(logFile.data());
    if(type == QtCriticalMsg || type == QtInfoMsg || (type == QtDebugMsg && is_debug))
    {
        std::cout << QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz ").toStdString();
    }
    if(type!= QtDebugMsg || is_debug || type == QtInfoMsg)
        out << QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz ");
    switch(type)
    {
        case QtInfoMsg:     out << "| INFO     "; break;
        case QtDebugMsg: if(is_debug)   out << "| DEBUG    "; break;
        case QtWarningMsg:  out << "| WARNING  "; break;
        case QtCriticalMsg: out << "| CRITICAL "; break;
        case QtFatalMsg:    out << "| FATAL    "; break;
    }
    if(type!= QtDebugMsg || is_debug || type == QtInfoMsg)
        out << "| " << context.category <<"| " << msg << endl;
    if(type == QtCriticalMsg || type == QtInfoMsg || (type == QtDebugMsg && is_debug))
        std::cout << "| " << context.category <<"| " << msg.toStdString().c_str() << endl;
    out.flush();
}


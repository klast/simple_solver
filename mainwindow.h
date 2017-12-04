#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCharts/QtCharts>
QT_CHARTS_USE_NAMESPACE
using namespace QtCharts;

#include "Model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void create_pvt_features_graph();
    void create_graph(QChart* chart, QString title);

private slots:
    void handleButton(filetypes type, QString &dialog_str);

private:
    Ui::MainWindow *ui;
    //! Считыватель данных из DATA файла
    Model model;
    QVector<QString> filenames;
};

#endif // MAINWINDOW_H

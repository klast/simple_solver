#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include "reader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void handleButton();

private:
    Ui::MainWindow *ui;
    //! Считыватель данных из DATA файла
    Reader reader;
};

#endif // MAINWINDOW_H

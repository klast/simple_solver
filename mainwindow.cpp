#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(handleButton()));
}

void MainWindow::handleButton()
{
    //! Вызов диалогового окна открытия файла
    QString filename = QFileDialog::getOpenFileName(this, tr("Открыть файл модели"), "",
                                                    tr("DATA файл (*.DATA);;Все файлы(*)"));
    //! Окно для вывода сообщения
    QMessageBox msgbox;
    int error_code = 0;
    //! пытаемся открыть файл
    if(model.reader.set_datafile(filename))
    {
        msgbox.setText("Исходный файл модели задан");
    }
    else
    {
        msgbox.setText("Произошла ошибка при открытии файла");
        error_code = 1;
    }
    //! Вызвать окно
    msgbox.exec();
    if(error_code == 1)
        QApplication::quit();

    model.simulate();
    //! завершение работы программы
    QApplication::quit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

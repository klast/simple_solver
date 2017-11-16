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
    //! пытаемся открыть файл
    if(model.open(filename))
    {
        msgbox.setText("Файл открыт");
    }
    else
    {
        msgbox.setText("Произошла шибка при открытии файла");
    }
    //! Вызвать окно
    msgbox.exec();
    //! закрыть файл
    model.reader.close();
    //! завершение работы программы
    QApplication::quit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

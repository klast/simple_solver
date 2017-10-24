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
    QString filename = QFileDialog::getOpenFileName(this, tr("Открыть файл модели"), "",
                                                    tr("DATA файл (*.DATA);;Все файлы(*)"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

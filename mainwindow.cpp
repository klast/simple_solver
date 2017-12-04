#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->init_well_Button->connect(ui->init_well_Button, SIGNAL(clicked(bool)), this, SLOT(handleButton(filetypes::init_well, "txt файл (*.txt);")));
    ui->pushButton->connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(handleButton()));
    filenames.reserve(6);
    create_pvt_features_graph();
}

void MainWindow::handleButton(filetypes type, QString &dialog_str)
{
    //! Вызов диалогового окна открытия файла
    QString filename = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "",
                                                    dialog_str + ";;Все файлы(*)");
    filenames[type] = filename;
}

void MainWindow::create_graph(QChart* chart, QString title)
{
    QLineSeries *series = new QLineSeries();
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(title);
}

void MainWindow::create_pvt_features_graph()
{
    QGridLayout* gridLayout = new QGridLayout(ui->pvt_features);
    QChart* pvt_chart = new QChart();
    QChart* kap_chart = new QChart();
    create_graph(pvt_chart, "PVT свойства");
    create_graph(kap_chart, "Капиллярка");
    QChartView* pvt_view, *kap_view;
    pvt_view = new QChartView(pvt_chart);
    kap_view = new QChartView(kap_chart);
    gridLayout->addWidget(pvt_view, 0, 0);
    gridLayout->addWidget(kap_view, 0, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

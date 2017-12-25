#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->init_well_Button->connect(ui->init_well_Button, SIGNAL(clicked(bool)), this, SLOT(handle_Button()));
    ui->SCAL_Button->connect(ui->SCAL_Button, SIGNAL(clicked(bool)), this, SLOT(handle_Button()));
    ui->PVT_Button->connect(ui->PVT_Button, SIGNAL(clicked(bool)), this, SLOT(handle_Button()));
    ui->GRID_Button->connect(ui->GRID_Button, SIGNAL(clicked(bool)), this, SLOT(handle_Button()));
    ui->INIT_Button->connect(ui->INIT_Button, SIGNAL(clicked(bool)), this, SLOT(handle_Button()));
    ui->GPRO_Button->connect(ui->GPRO_Button, SIGNAL(clicked(bool)), this, SLOT(handle_Button()));
    ui->push_Button->connect(ui->push_Button, SIGNAL(clicked(bool)), this, SLOT(handle_start_Button()));
    ui->push_test_Button->connect(ui->push_test_Button, SIGNAL(clicked(bool)), this, SLOT(handle_start_test_Button()));
    for(int i = 0; i < 6; i++)
    {
        filenames.append("");
    }
    create_pvt_features_graph();
}

void MainWindow::handle_Button()
{
    //! Вызов диалогового окна открытия файла
    QString dialog_name;
    QString sender_name = sender()->objectName();
    if(sender_name == ui->init_well_Button->objectName())
        dialog_name = "txt файл (*.txt)";
    else
        dialog_name = "INC файл (*.INC)";
    QString filename = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "",
                                                     dialog_name + ";;Все файлы(*)");
    if(sender_name == ui->init_well_Button->objectName())
    {
        filenames[filetypes::init_well] = filename;
    }
    else if(sender_name == ui->SCAL_Button->objectName())
    {
        filenames[filetypes::SCAL] = filename;
    }
    else if(sender_name == ui->PVT_Button->objectName())
    {
        filenames[filetypes::PVT] = filename;
    }
    else if(sender_name == ui->GRID_Button->objectName())
    {
        filenames[filetypes::GRID] = filename;
    }
    else if(sender_name == ui->INIT_Button->objectName())
    {
        filenames[filetypes::INIT] = filename;
    }
    else if(sender_name == ui->GPRO_Button->objectName())
    {
        filenames[filetypes::GPRO] = filename;
    }
    qDebug() << sender()->objectName() << filename;

}

void MainWindow::handle_start_Button()
{
    bool is_all_set = true;
    for(int i = 0; i < 6; i++)
    {
        if(filenames[i] == "")
        {
            is_all_set = false;
        }
    }
    QMessageBox msgBox;
    if(is_all_set)
    {
        qDebug("Файлы заданы, начинаем!");
    }
    else
    {
        msgBox.setText("Ошибка! Какой-то из файлов не задан!");
        msgBox.exec();
        QApplication::quit();
        return;
    }
    model.reader.set_file(filetypes::init_well, filenames[filetypes::init_well]);
    model.reader.set_file(filetypes::SCAL, filenames[filetypes::SCAL]);
    model.reader.set_file(filetypes::PVT, filenames[filetypes::PVT]);
    model.reader.set_file(filetypes::GRID, filenames[filetypes::GRID]);
    model.reader.set_file(filetypes::GPRO, filenames[filetypes::GPRO]);
    model.reader.set_file(filetypes::INIT, filenames[filetypes::INIT]);
    if(model.simulate() == 1)
    {
        msgBox.setText("Вроде считывание работает, но это не точно!");
        QApplication::quit();
        return;
    }
}

void MainWindow::handle_start_test_Button()
{
    filenames[filetypes::init_well] = "C:/projects/simple_solver/tests/model4_test/init4.txt";
    filenames[filetypes::SCAL] = "C:/projects/simple_solver/tests/model4_test/MODEL_SCAL.INC";
    filenames[filetypes::PVT] = "C:/projects/simple_solver/tests/model4_test/MODEL_PVT.INC";
    filenames[filetypes::GRID] = "C:/projects/simple_solver/tests/model4_test/MODEL_GRID.INC";
    filenames[filetypes::GPRO] = "C:/projects/simple_solver/tests/model4_test/MODEL_INIT.INC";
    filenames[filetypes::INIT] = "C:/projects/simple_solver/tests/model4_test/MODEL_GPRO.INC";
    model.reader.set_file(filetypes::init_well, filenames[filetypes::init_well]);
    model.reader.set_file(filetypes::SCAL, filenames[filetypes::SCAL]);
    model.reader.set_file(filetypes::PVT, filenames[filetypes::PVT]);
    model.reader.set_file(filetypes::GRID, filenames[filetypes::GRID]);
    model.reader.set_file(filetypes::GPRO, filenames[filetypes::GPRO]);
    model.reader.set_file(filetypes::INIT, filenames[filetypes::INIT]);
    if(model.simulate() == 1)
    {
        QApplication::quit();
        return;
    }
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

#include "graphicsform.h"
#include "ui_graphicsform.h"

GraphicsForm::GraphicsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphicsForm)
{
    ui->setupUi(this);
}

GraphicsForm::~GraphicsForm()
{
    delete ui;
}

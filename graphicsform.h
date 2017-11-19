#ifndef GRAPHICSFORM_H
#define GRAPHICSFORM_H

#include <QWidget>

namespace Ui {
class GraphicsForm;
}

class GraphicsForm : public QWidget
{
    Q_OBJECT

public:
    explicit GraphicsForm(QWidget *parent = 0);
    ~GraphicsForm();

private:
    Ui::GraphicsForm *ui;
};

#endif // GRAPHICSFORM_H

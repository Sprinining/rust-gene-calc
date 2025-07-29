#ifndef GENEINPUTPANEL_H
#define GENEINPUTPANEL_H

#include <QWidget>

namespace Ui {
class GeneInputPanel;
}

class GeneInput : public QWidget
{
    Q_OBJECT

public:
    explicit GeneInput(QWidget *parent = nullptr);
    ~GeneInput();

private:
    Ui::GeneInputPanel *ui;
};

#endif // GENEINPUTPANEL_H

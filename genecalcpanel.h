#ifndef GENECALCPANEL_H
#define GENECALCPANEL_H

#include <QWidget>

namespace Ui {
class GeneCalcPanel;
}

class GeneCalcPanel : public QWidget
{
    Q_OBJECT

public:
    explicit GeneCalcPanel(QWidget *parent = nullptr);
    ~GeneCalcPanel();

private:
    Ui::GeneCalcPanel *ui;
};

#endif // GENECALCPANEL_H

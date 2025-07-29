#ifndef GENEPANEL_H
#define GENEPANEL_H

#include <QWidget>

namespace Ui {
class GenePanel;
}

class GenePanel : public QWidget
{
    Q_OBJECT

public:
    explicit GenePanel(QWidget *parent = nullptr);
    ~GenePanel();

private:
    Ui::GenePanel *ui;
};

#endif // GENEPANEL_H

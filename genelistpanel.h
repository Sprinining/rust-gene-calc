#ifndef GENELISTPANEL_H
#define GENELISTPANEL_H

#include <QWidget>

namespace Ui {
class GeneListPanel;
}

class GeneListPanel : public QWidget
{
    Q_OBJECT

public:
    explicit GeneListPanel(QWidget *parent = nullptr);
    ~GeneListPanel();

private:
    Ui::GeneListPanel *ui;
};

#endif // GENELISTPANEL_H

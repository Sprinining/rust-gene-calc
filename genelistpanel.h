#ifndef GENELISTPANEL_H
#define GENELISTPANEL_H

#include "seeddelegate.h"
#include "seedmodel.h"
#include <QWidget>

namespace Ui {
class GeneListPanel;
}

class GeneListPanel : public QWidget {
    Q_OBJECT

public:
    explicit GeneListPanel(QWidget *parent = nullptr);
    ~GeneListPanel();

private:
    Ui::GeneListPanel *ui;
    SeedModel *model_{nullptr};
    SeedDelegate *delegate_{nullptr};

    void initUI();

public slots:
    void onListViewContextMenu(const QPoint &pos);
    void addSeedItem(const Seed &seed);  // 新增槽函数声明，参数可根据需求调整
};

#endif // GENELISTPANEL_H

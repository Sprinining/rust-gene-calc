#ifndef GENESETTINGSPANEL_H
#define GENESETTINGSPANEL_H

#include <QWidget>
#include <QLabel>
#include "seed.h"  // 你的 Seed 类型定义头文件，确保包含了 AppConsts::GeneType

namespace Ui {
class GeneSettingsPanel;
}

class GeneSettingsPanel : public QWidget {
    Q_OBJECT

public:
    explicit GeneSettingsPanel(QWidget *parent = nullptr);
    ~GeneSettingsPanel();

signals:
    void seedInputFinished(const Seed &seed);

private slots:
    void onGeneButtonClicked();
    void onLineEditTextChanged(const QString &text);
    void onPushButtonInputClicked();

private:
    Ui::GeneSettingsPanel *ui;

    static const QString validGenes;

    QLabel* geneLabels_[6];  // 缓存上面6个标签指针

    void updateLabelsFromInput(const QString &text);
    QColor geneColor(QChar gene);
};

#endif // GENESETTINGSPANEL_H

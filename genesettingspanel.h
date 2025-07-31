#ifndef GENESETTINGSPANEL_H
#define GENESETTINGSPANEL_H

#include "seed.h"
#include <QLabel>
#include <QWidget>

namespace Ui {
class GeneSettingsPanel;
}

class GeneSettingsPanel : public QWidget {
    Q_OBJECT

public:
    explicit GeneSettingsPanel(QWidget *parent = nullptr);
    ~GeneSettingsPanel();

private:
    Ui::GeneSettingsPanel *ui;

    static const QString validGenes; // 有效基因字符集，用于输入校验

    QLabel *geneLabels_[6]; // 缓存指向上方6个圆形标签的指针，提升访问效率

    // 根据输入文本更新上方6个标签显示及样式
    void updateLabelsFromInput(const QString &text);
    // 根据基因字符返回对应的颜色，用于标签背景色设置
    QColor geneColor(QChar gene);
    void initUI();
    void initSignals();

private slots:
    // 处理五个基因按钮点击事件
    void onGeneButtonClicked();
    // 处理编辑框内容变化事件
    void onLineEditTextChanged(const QString &text);
    // 处理“录入”按钮点击事件
    void onPushButtonInputClicked();
    void onCalcButtonClicked();

signals:
    // 当用户完成种子输入并点击“录入”按钮时发射此信号
    // 传递 Seed 类型对象，方便外部接收处理
    void seedInputFinished(const Seed &seed);
    // 点击计算按钮时触发
    void requestSeedCalculation();
};

#endif // GENESETTINGSPANEL_H

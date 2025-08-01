#include "genelistpanel.h"
#include "genecalculator.h"
#include "ui_genelistpanel.h"
#include <QMenu>

GeneListPanel::GeneListPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::GeneListPanel) {
    ui->setupUi(this); // 初始化 UI
    initUI();          // 初始化模型、委托和数据

    // 连接右键菜单信号槽
    connect(ui->listView, &QWidget::customContextMenuRequested, this,
            &GeneListPanel::onListViewContextMenu);
}

// 初始化模型、委托、视图设置和初始数据
void GeneListPanel::initUI() {
    // 创建模型（数据）和委托（渲染）
    model_ = new SeedModel(this);
    delegate_ = new SeedDelegate(this);

    // 设置模型和委托到 QListView 上
    ui->listView->setModel(model_);
    ui->listView->setItemDelegate(delegate_);

    // 设置右键菜单触发策略
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
}

GeneListPanel::~GeneListPanel() { delete ui; }

// 右键菜单槽函数，在 listView 上点击右键时调用
void GeneListPanel::onListViewContextMenu(const QPoint &pos) {
    QModelIndex index = ui->listView->indexAt(pos); // 获取点击位置的索引
    if (!index.isValid())
        return;

    QMenu menu;                                  // 创建右键菜单
    QAction *delAction = menu.addAction("删除"); // 添加“删除”动作
    QAction *selectedAction =
        menu.exec(ui->listView->viewport()->mapToGlobal(pos)); // 弹出菜单

    // 如果点击了“删除”，则删除对应的模型数据行
    if (selectedAction == delAction) {
        model_->removeSeed(index.row());
    }
}

// 公共接口，用于从外部添加新的种子条目
void GeneListPanel::addSeedItem(const Seed &seed) {
    if (model_) {
        model_->appendSeed(seed);
    }
}

void GeneListPanel::onCalculateRequested() {
    if (!model_)
        return;

    // 验证种子数量是否满足计算要求，至少需要4个种子
    if (model_->rowCount() < 4) {
        qDebug() << "至少需要4个种子才能进行计算。";
        emit calculationFinished("至少需要4个种子才能进行计算！");
        return;
    }

    GeneCalculator calculator;

    // 遍历模型中所有种子，添加到计算器
    for (int i = 0; i < model_->rowCount(); ++i) {
        Seed seed = model_->getSeed(i);
        calculator.addSeed(seed);
    }

    // 执行杂交计算
    calculator.calculate();

    // 获取四个父代种子
    const auto &breedingSeeds = calculator.getBreedingSeeds();
    // 获取最优子代种子
    const Seed &offspring = calculator.getOffspringSeed();

    QString result;

    // 遍历父代种子，拼接每个父代的基因字符串
    for (int i = 0; i < breedingSeeds.size(); ++i) {
        QString parentGenes;
        for (auto g : breedingSeeds[i]->genes_) {
            // 将基因枚举转换成字符，并拼接空格分隔
            parentGenes += QChar(GeneCalculator::geneTypeToChar(g));
            parentGenes += ' ';
        }
        // 拼接父代基因信息，换行分隔
        result += QString("Parent %1 genes: %2\n").arg(i + 1).arg(parentGenes);
    }

    // 拼接子代基因字符串
    QString offspringGenes;
    for (auto g : offspring.genes_) {
        offspringGenes += QChar(GeneCalculator::geneTypeToChar(g));
        offspringGenes += ' ';
    }
    // 拼接子代基因信息
    result += QString("Best offspring seed genes: %1\n").arg(offspringGenes);

    qDebug() << result;

    // 通过信号将结果字符串发出去，供 UI 显示
    emit calculationFinished(result);
}

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

    // 添加一个测试种子条目（用于运行时验证）
    Seed newSeed{AppConsts::GeneType::G, AppConsts::GeneType::Y,
                 AppConsts::GeneType::H, AppConsts::GeneType::X,
                 AppConsts::GeneType::W, AppConsts::GeneType::G};
    model_->appendSeed(newSeed);
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

    // 构造一些测试用的种子数据
    QVector<Seed> seeds = {
        Seed({AppConsts::GeneType::G, AppConsts::GeneType::Y,
              AppConsts::GeneType::H, AppConsts::GeneType::X,
              AppConsts::GeneType::W, AppConsts::GeneType::G}),
        Seed({AppConsts::GeneType::Y, AppConsts::GeneType::Y,
              AppConsts::GeneType::H, AppConsts::GeneType::H,
              AppConsts::GeneType::X, AppConsts::GeneType::W}),
        // 更多 Seed 可按需添加
    };

    // 将数据设置进模型
    model_->setSeeds(seeds);
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

    GeneCalculator calculator;

    for (int i = 0; i < model_->rowCount(); ++i) {
        Seed seed = model_->getSeed(i);
        calculator.addSeed(seed);
    }

    calculator.calculate();

    const auto &breedingSeeds = calculator.getBreedingSeeds();
    const Seed &offspring = calculator.getOffspringSeed();

    // 打印输出调试
    for (int i = 0; i < breedingSeeds.size(); ++i) {
        QString parentGenes;
        for (auto g : breedingSeeds[i]->genes_) {
            parentGenes += QChar(GeneCalculator::geneTypeToChar(g));
            parentGenes += ' ';
        }
        qDebug() << QString("Parent %1 genes:").arg(i + 1) << parentGenes;
    }

    QString result;
    for (auto g : offspring.genes_) {
        result += QChar(GeneCalculator::geneTypeToChar(g));
        result += ' ';
    }
    qDebug() << "Best offspring seed genes:" << result;

    // TODO: 你可以在这里更新 UI，比如设置到某个 QLabel 上显示结果
}

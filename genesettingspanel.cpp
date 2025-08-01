#include "genesettingspanel.h"
#include "ui_genesettingspanel.h"
#include <QDebug>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QThread>
#include <qthread.h>

// 有效的基因字符集（大写）：G、Y、H、W、X
const QString GeneSettingsPanel::validGenes = "GYHWX";

GeneSettingsPanel::GeneSettingsPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::GeneSettingsPanel) {
    ui->setupUi(this);

    // 缓存顶部六个 QLabel 指针，分别对应六个圆形标签
    geneLabels_[0] = ui->label_1;
    geneLabels_[1] = ui->label_2;
    geneLabels_[2] = ui->label_3;
    geneLabels_[3] = ui->label_4;
    geneLabels_[4] = ui->label_5;
    geneLabels_[5] = ui->label_6;

    initUI();
    initSignals();
}

GeneSettingsPanel::~GeneSettingsPanel() { delete ui; }

// 当点击任意一个基因按钮时触发
void GeneSettingsPanel::onGeneButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
        return;

    QString geneChar = btn->text().toUpper(); // 取得按钮文字并转为大写
    QString currentText = ui->lineEdit->text().toUpper();

    // 如果已满6个字符则忽略
    if (currentText.length() >= 6)
        return;

    currentText.append(geneChar);
    ui->lineEdit->setText(currentText); // 设置文本，会自动触发 textChanged 槽函数
}

// 当输入框文字发生变化时触发
void GeneSettingsPanel::onLineEditTextChanged(const QString &text) {
    QString upperText = text.toUpper();

    // 如果用户输入了小写，强制转为大写，同时保留光标位置
    if (text != upperText) {
        int cursorPos = ui->lineEdit->cursorPosition();
        ui->lineEdit->blockSignals(true); // 防止递归触发 textChanged
        ui->lineEdit->setText(upperText);
        ui->lineEdit->setCursorPosition(cursorPos);
        ui->lineEdit->blockSignals(false);
    }

    // 更新顶部6个圆形标签
    updateLabelsFromInput(upperText);
}

// 根据输入字符串更新顶部的6个圆形标签
void GeneSettingsPanel::updateLabelsFromInput(const QString &text) {
    for (int i = 0; i < 6; ++i) {
        QLabel *label = geneLabels_[i];
        if (!label)
            continue;

        if (i < text.length()) {
            QChar c = text[i];
            label->setText(c);             // 设置标签文字
            QColor bgColor = geneColor(c); // 获取颜色
            // 设置标签样式（背景色、字体等）
            label->setStyleSheet(QString("background-color: %1;"
                                         "color: white;"
                                         "border-radius: 25px;"
                                         "font-weight: bold;"
                                         "font-size: 20px;"
                                         "qproperty-alignment: AlignCenter;")
                                     .arg(bgColor.name()));
        } else {
            // 未输入则设为灰色问号
            label->setText("?");
            label->setStyleSheet("background-color: gray;"
                                 "color: white;"
                                 "border-radius: 25px;"
                                 "font-weight: bold;"
                                 "font-size: 20px;"
                                 "qproperty-alignment: AlignCenter;");
        }

        label->update(); // 强制刷新
    }
}

// 将基因字符映射为对应的颜色
QColor GeneSettingsPanel::geneColor(QChar gene) {
    switch (gene.toLatin1()) {
    case 'G':
    case 'Y':
    case 'H':
        return QColor(94, 134, 30); // #5e861e
    case 'W':
    case 'X':
        return QColor(155, 68, 51); // #9b4433
    default:
        return QColor(128, 128, 128); // 灰色
    }
}

void GeneSettingsPanel::initUI() {
    // 启用一键清空按钮（右上角 ×）
    ui->lineEdit->setClearButtonEnabled(true);
    // 设置输入框的正则表达式限制：仅允许 GYHWX 字母（大小写都行），最多6位
    QRegularExpression rx("[GYHWXgyhwx]{0,6}");
    ui->lineEdit->setValidator(new QRegularExpressionValidator(rx, this));

    // 初始状态下将标签设为灰色问号
    updateLabelsFromInput("");
}

void GeneSettingsPanel::initSignals() {
    // 连接五个基因按钮的点击信号到统一槽函数
    connect(ui->pushButtonG, &QPushButton::clicked, this,
            &GeneSettingsPanel::onGeneButtonClicked);
    connect(ui->pushButtonY, &QPushButton::clicked, this,
            &GeneSettingsPanel::onGeneButtonClicked);
    connect(ui->pushButtonH, &QPushButton::clicked, this,
            &GeneSettingsPanel::onGeneButtonClicked);
    connect(ui->pushButtonW, &QPushButton::clicked, this,
            &GeneSettingsPanel::onGeneButtonClicked);
    connect(ui->pushButtonX, &QPushButton::clicked, this,
            &GeneSettingsPanel::onGeneButtonClicked);

    // 计算
    connect(ui->pushButtonCalc, &QPushButton::clicked, this,
            &GeneSettingsPanel::onCalcButtonClicked);

    // 监听输入框文字变化，实时更新上方标签
    connect(ui->lineEdit, &QLineEdit::textChanged, this,
            &GeneSettingsPanel::onLineEditTextChanged);

    // “录入”按钮点击事件
    connect(ui->pushButtonInput, &QPushButton::clicked, this,
            &GeneSettingsPanel::onPushButtonInputClicked);
}

void GeneSettingsPanel::displayCalculationResult(const QString &result) {
    ui->textBrowser->setText(result);
}

// 当点击“录入”按钮时触发
void GeneSettingsPanel::onPushButtonInputClicked() {
    QString inputText = ui->lineEdit->text();

    // 必须满6个基因才可录入
    if (inputText.length() != 6) {
        ui->textBrowser->setText(tr("一个种子至少6个基因！"));
        return;
    }
    ui->textBrowser->setText("");

    Seed seed;
    for (int i = 0; i < 6; ++i) {
        QChar ch = inputText[i];
        switch (ch.toUpper().unicode()) {
        case 'G':
            seed.genes_[i] = AppConsts::GeneType::G;
            break;
        case 'Y':
            seed.genes_[i] = AppConsts::GeneType::Y;
            break;
        case 'H':
            seed.genes_[i] = AppConsts::GeneType::H;
            break;
        case 'W':
            seed.genes_[i] = AppConsts::GeneType::W;
            break;
        case 'X':
            seed.genes_[i] = AppConsts::GeneType::X;
            break;
        default:
            return; // 非法字符，直接终止录入
        }
    }

    // 发送信号给外部模块
    emit seedInputFinished(seed);
}

void GeneSettingsPanel::onCalcButtonClicked() {
    emit requestSeedCalculation(); // 通知外部执行计算逻辑
}

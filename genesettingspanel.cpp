#include "genesettingspanel.h"
#include "ui_genesettingspanel.h"
#include <QDebug>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

const QString GeneSettingsPanel::validGenes = "GYHWX";

GeneSettingsPanel::GeneSettingsPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::GeneSettingsPanel) {
    ui->setupUi(this);

    // 缓存指针，避免频繁findChild
    geneLabels_[0] = ui->label_1;
    geneLabels_[1] = ui->label_2;
    geneLabels_[2] = ui->label_3;
    geneLabels_[3] = ui->label_4;
    geneLabels_[4] = ui->label_5;
    geneLabels_[5] = ui->label_6;

    // 限制编辑框输入，只能是 G,Y,H,W,X (大小写都行)，最多6个字符
    QRegularExpression rx("[GYHWXgyhwx]{0,6}");
    ui->lineEdit->setValidator(new QRegularExpressionValidator(rx, this));

    // 连接五个基因按钮点击
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

    // 编辑框内容变化连接
    connect(ui->lineEdit, &QLineEdit::textChanged, this,
            &GeneSettingsPanel::onLineEditTextChanged);

    // 录入按钮点击
    connect(ui->pushButtonInput, &QPushButton::clicked, this,
            &GeneSettingsPanel::onPushButtonInputClicked);

    // 初始化label显示问号灰色
    updateLabelsFromInput("");
}

GeneSettingsPanel::~GeneSettingsPanel() { delete ui; }

void GeneSettingsPanel::onGeneButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
        return;

    QString geneChar = btn->text().toUpper();
    QString currentText = ui->lineEdit->text().toUpper();

    if (currentText.length() >= 6)
        return;

    currentText.append(geneChar);
    ui->lineEdit->setText(currentText); // 会触发 textChanged 自动更新label
}

void GeneSettingsPanel::onLineEditTextChanged(const QString &text) {
    QString upperText = text.toUpper();

    if (text != upperText) {
        int cursorPos = ui->lineEdit->cursorPosition();
        ui->lineEdit->blockSignals(true);
        ui->lineEdit->setText(upperText);
        ui->lineEdit->setCursorPosition(cursorPos);
        ui->lineEdit->blockSignals(false);
        return; // setText后再次触发textChanged会调用updateLabelsFromInput
    }

    updateLabelsFromInput(upperText);
}

void GeneSettingsPanel::updateLabelsFromInput(const QString &text) {
    for (int i = 0; i < 6; ++i) {
        QLabel *label = geneLabels_[i];
        if (!label)
            continue;

        if (i < text.length()) {
            QChar c = text[i];
            label->setText(c);
            QColor bgColor = geneColor(c);
            label->setStyleSheet(QString("background-color: %1;"
                                         "color: white;"
                                         "border-radius: 25px;"
                                         "font-weight: bold;"
                                         "font-size: 20px;"
                                         "qproperty-alignment: AlignCenter;")
                                     .arg(bgColor.name()));
        } else {
            label->setText("?");
            label->setStyleSheet("background-color: gray;"
                                 "color: white;"
                                 "border-radius: 25px;"
                                 "font-weight: bold;"
                                 "font-size: 20px;"
                                 "qproperty-alignment: AlignCenter;");
        }
        label->update();
    }
}

QColor GeneSettingsPanel::geneColor(QChar gene) {
    switch (gene.toLatin1()) {
    case 'G':
    case 'Y':
    case 'H':
        return QColor("#5e861e"); // 绿色系
    case 'W':
    case 'X':
        return QColor("#9b4433"); // 棕红色系
    default:
        return QColor("gray");
    }
}

void GeneSettingsPanel::onPushButtonInputClicked() {
    QString inputText = ui->lineEdit->text();

    if (inputText.length() != 6) {
        // TODO: 这里可以弹窗提醒用户
        return;
    }

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
            return; // 非法字符
        }
    }

    emit seedInputFinished(seed);
}

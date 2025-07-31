#include "seeddelegate.h"

SeedDelegate::SeedDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

// 自定义绘图函数，每个 QListView 的 item 都通过它来绘制
void SeedDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
    painter->save(); // 保存当前画笔状态
    painter->setRenderHint(QPainter::Antialiasing,
                           true); // 开启抗锯齿，边缘更平滑

    // 获取模型中存储的自定义类型 Seed
    QVariant var = index.data(Qt::UserRole);
    if (!var.canConvert<Seed>()) {
        painter->restore(); // 类型不对，跳过绘制
        return;
    }
    Seed seed = var.value<Seed>();

    // 获取当前 item 的区域，以及绘制时用到的一些常量
    QRect rect = option.rect; // 当前绘制区域
    int spacing = 10;         // 圆之间的间隔
    int circleSize = 30;      // 每个圆的直径

    // 绘制序号（左边一小块）
    QString number =
        QString::number(index.row() + 1); // 当前项的序号（从 1 开始）
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 14, QFont::Bold)); // 黑色加粗字体
    QRect numberRect(rect.left(), rect.top(), 30,
                     rect.height()); // 序号区域宽度 30
    painter->drawText(numberRect, Qt::AlignVCenter | Qt::AlignLeft,
                      number); // 居中绘制序号文本

    // 计算圆形起始位置和 Y 中心点
    int startX = rect.left() + 40; // 从左边 40 像素开始绘制圆，留出序号区域
    int centerY = rect.top() + rect.height() / 2; // 纵向居中

    // 画圆之间的连线（5 条线连接 6 个圆心）
    painter->setPen(QPen(Qt::black, 2)); // 设置线宽为 2 的黑色实线
    for (int i = 0; i < 5; ++i) {
        int x1 = startX + i * (circleSize + spacing) +
                 circleSize / 2; // 第 i 个圆的中心 X
        int x2 = startX + (i + 1) * (circleSize + spacing) +
                 circleSize / 2;                     // 第 i+1 个圆的中心 X
        painter->drawLine(x1, centerY, x2, centerY); // 水平画线
    }

    // 绘制 6 个圆形及对应的文字（G/Y/H/W/X）
    for (int i = 0; i < 6; ++i) {
        QRect circleRect(startX + i * (circleSize + spacing), // 左上角 X 坐标
                         centerY - circleSize / 2, // 上移半个圆的高度，使其垂直居中
                         circleSize, circleSize);  // 宽高相同，形成正圆

        AppConsts::GeneType g = seed.genes_[i]; // 当前基因类型
        QColor bgColor = Qt::gray;              // 默认背景色
        QString text;                           // 要显示的字母

        // 根据不同的基因类型设置颜色和文字
        switch (g) {
        case AppConsts::GeneType::G:
            bgColor = QColor(94, 134, 30); // #5e861e
            text = "G";
            break;
        case AppConsts::GeneType::Y:
            bgColor = QColor(94, 134, 30); // #5e861e
            text = "Y";
            break;
        case AppConsts::GeneType::H:
            bgColor = QColor(94, 134, 30); // #5e861e
            text = "H";
            break;
        case AppConsts::GeneType::W:
            bgColor = QColor(155, 68, 51); // #9b4433
            text = "W";
            break;
        case AppConsts::GeneType::X:
            bgColor = QColor(155, 68, 51); // #9b4433
            text = "X";
            break;
        }

        painter->setBrush(bgColor);       // 设置圆的填充颜色
        painter->setPen(Qt::NoPen);       // 不画边框
        painter->drawEllipse(circleRect); // 绘制圆形

        painter->setPen(Qt::white);                           // 设置文字颜色为白色
        painter->drawText(circleRect, Qt::AlignCenter, text); // 在圆内居中绘制字母
    }

    painter->restore(); // 恢复原始画笔状态
}

// 返回每个 item 的建议大小（决定 item 高度）
QSize SeedDelegate::sizeHint(const QStyleOptionViewItem &,
                             const QModelIndex &) const {
    return QSize(300, 50); // 建议宽度 300，高度 50
}

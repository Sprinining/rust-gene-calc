#pragma once

#include "seed.h"
#include <QPainter>
#include <QStyledItemDelegate>

// 自定义委托类，用于绘制 QListView 中每个 item 显示六个圆形基因标签
class SeedDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    // 构造函数，接受父对象指针
    explicit SeedDelegate(QObject *parent = nullptr);

    // 重写 paint 函数，自定义绘制 item
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    // 重写 sizeHint，告诉视图每个 item 的推荐大小
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

#pragma once
#include "seed.h"
#include <QAbstractListModel>

// 自定义模型类，继承自 QAbstractListModel
// 用于在 QListView 中展示多个 Seed（包含 6 个基因信息）
class SeedModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit SeedModel(QObject *parent = nullptr);

    // 设置整个种子列表，替换当前数据
    void setSeeds(const QVector<Seed> &seeds);

    // 返回模型中的行数，即当前种子数量
    // parent 一般无效，因为是列表模型，默认参数表示根索引
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // 返回指定索引下的数据
    // role 指定需要的数据类型，一般用 Qt::UserRole 返回完整的 Seed 对象
    QVariant data(const QModelIndex &index, int role) const override;

    // 删除指定行的种子，成功返回 true，失败返回 false
    bool removeSeed(int row);

    // 向模型末尾添加一个新的种子
    void appendSeed(const Seed &seed);

private:
    // 内部存储的种子列表，每个 Seed 包含 6 个基因
    QVector<Seed> m_seeds_;
};

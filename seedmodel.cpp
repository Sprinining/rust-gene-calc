#include "seedmodel.h"

SeedModel::SeedModel(QObject *parent) : QAbstractListModel{parent} {}

// 设置整个种子列表，替换模型当前数据
void SeedModel::setSeeds(const QVector<Seed> &seeds) {
    beginResetModel(); // 通知视图模型即将重置
    m_seeds_ = seeds;  // 赋值新的种子列表
    endResetModel();   // 通知视图模型重置完成，刷新视图
}

// 返回模型中的行数（种子数量）
int SeedModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_seeds_.size(); // 返回当前持有的种子数量
}

// 返回指定索引对应的数据
// 只处理 UserRole，返回完整的 Seed 对象封装为 QVariant
QVariant SeedModel::data(const QModelIndex &index, int role) const {
    // 检查索引有效性和请求的角色
    if (!index.isValid() || role != Qt::UserRole)
        return {}; // 不符合条件返回空 QVariant

    // 返回该行对应的 Seed 对象，需 Seed 类型注册为 Qt 元类型
    return QVariant::fromValue(m_seeds_[index.row()]);
}

// 删除指定行的种子数据
// 如果行号非法，返回 false；成功删除返回 true
bool SeedModel::removeSeed(int row) {
    if (row < 0 || row >= m_seeds_.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row); // 通知视图将删除指定行
    m_seeds_.removeAt(row);                   // 从内部容器删除
    endRemoveRows();                          // 通知视图删除完成，刷新显示

    return true;
}

// 在模型末尾追加一个新的种子
void SeedModel::appendSeed(const Seed &seed) {
    beginInsertRows(QModelIndex(), m_seeds_.size(),
                    m_seeds_.size()); // 通知视图即将插入
    m_seeds_.push_back(seed);         // 添加数据
    endInsertRows();                  // 通知视图插入完成，刷新显示
}

#ifndef TESTGENECALCULATORWORKER_H
#define TESTGENECALCULATORWORKER_H

#include "seed.h"
#include <QObject>

// TestGeneCalculatorWorker 类用于在独立线程执行基因计算的耗时任务
class TestGeneCalculatorWorker : public QObject {
    Q_OBJECT
public:
    // 构造函数，接收需要生成的随机种子数量和可选的父对象指针
    explicit TestGeneCalculatorWorker(int seedCount, QObject *parent = nullptr);

public slots:
    // 处理函数，执行计算任务，适合放在槽函数中以便于 moveToThread 调用
    void process();

signals:
    // 计算完成信号，传递计算得到的最优方案和最优的种子
    void resultReady(const std::array<std::shared_ptr<Seed>, 4> &breeding_seeds, const Seed &offspring_seed);

    // 处理完成信号，通知线程可以退出
    void finished();

private:
    // 要生成的随机种子数量
    int m_seedCount;

    // 辅助函数：生成一个随机种子
    Seed generateRandomSeed();
};

#endif // TESTGENECALCULATORWORKER_H

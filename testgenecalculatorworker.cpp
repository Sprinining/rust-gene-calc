#include "testgenecalculatorworker.h"
#include "genecalculator.h"
#include <QDebug>
#include <QElapsedTimer>
#include <random>

// 构造函数，初始化种子数量和父对象指针
TestGeneCalculatorWorker::TestGeneCalculatorWorker(int seedCount,
                                                   QObject *parent)
    : QObject(parent), m_seedCount(seedCount) {}

// 处理槽函数，执行基因计算的耗时任务
void TestGeneCalculatorWorker::process() {
    QElapsedTimer timer;
    timer.start();

    GeneCalculator calculator;

    // 根据传入的数量生成随机种子，加入计算器
    for (int i = 0; i < m_seedCount; ++i) {
        Seed s = generateRandomSeed();
        calculator.addSeed(std::move(s));
    }

    // 执行计算，找到最佳杂交结果
    calculator.calculate();

    qint64 elapsedMs = timer.elapsed();
    qDebug() << "Gene calculation took" << elapsedMs << "milliseconds";

    // 发射信号，传递最优子代基因
    emit resultReady(calculator.getBreedingSeeds(), calculator.getOffspringSeed());

    // 计算完成，通知线程可以退出
    emit finished();
}

// 生成一个随机种子，基因随机取值于枚举的0~4范围
Seed TestGeneCalculatorWorker::generateRandomSeed() {
    // 静态随机数生成器，避免频繁初始化
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 4); // 0~4 对应5种基因

    Seed s;
    for (auto &gene : s.genes_)
        gene = static_cast<AppConsts::GeneType>(dist(gen));
    return s;
}

#include "genecalculator.h"
#include "mainwindow.h"
#include "testgenecalculatorworker.h"
#include <QApplication>
#include <QFile>
#include <QThread>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;

    QFile file(":/style/style.qss"); // 如果用资源文件，路径写 ":/xxx.qss"
    if (file.open(QFile::ReadOnly)) {
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    }

    // 注册 Seed 类型，确保 QVariant 能正确识别
    qRegisterMetaType<Seed>("Seed");

    w.show();

    int seedCount = 80; // 设置要生成的随机种子数量

    // 创建一个新的线程，用于执行耗时的基因计算任务
    QThread *thread = new QThread;

    // 创建测试工作对象，传入种子数量参数
    TestGeneCalculatorWorker *worker = new TestGeneCalculatorWorker(seedCount);

    // 将工作对象移动到新线程中执行
    worker->moveToThread(thread);

    // 当线程启动时，调用 worker 的 process 槽函数，开始计算
    QObject::connect(thread, &QThread::started, worker,
                     &TestGeneCalculatorWorker::process);

    // 当 worker 发出 finished 信号，通知线程退出事件循环，结束线程
    QObject::connect(worker, &TestGeneCalculatorWorker::finished, thread,
                     &QThread::quit);

    // 当 worker 完成后，自动销毁 worker 对象，防止内存泄漏
    QObject::connect(worker, &TestGeneCalculatorWorker::finished, worker,
                     &QObject::deleteLater);

    // 当线程结束后，自动销毁线程对象，防止内存泄漏
    QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    // 连接 worker 的结果信号，接收计算出的最优子代种子
    QObject::connect(
        worker, &TestGeneCalculatorWorker::resultReady,
        [](const std::array<std::shared_ptr<Seed>, 4> &breeding_seeds,
                                                           const Seed &offspring) {
        // 打印最优父代种子基因
        for (int i = 0; i < breeding_seeds.size(); ++i) {
            QString parentGenes;
            for (auto g : breeding_seeds[i]->genes_) {
                parentGenes += QChar(GeneCalculator::geneTypeToChar(g));
                parentGenes += ' ';
            }
            qDebug() << QString("Parent %1 genes:").arg(i + 1) << parentGenes;
        }

        // 打印子代基因
        QString result;
        for (auto g : offspring.genes_) {
            result += QChar(GeneCalculator::geneTypeToChar(g));
            result += ' ';
        }
        qDebug() << "Best offspring seed genes:" << result;
        });

    // 启动线程，开始执行计算
    thread->start();

    return a.exec();
}

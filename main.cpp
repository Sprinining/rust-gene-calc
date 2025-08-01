#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;

    QFile file(":/style/style.qss");
    if (file.open(QFile::ReadOnly)) {
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    }

    w.setWindowIcon(QIcon(":/icons/favicon.ico"));

    // 注册 Seed 类型，确保 QVariant 能正确识别
    qRegisterMetaType<Seed>("Seed");

    w.show();

    return a.exec();
}

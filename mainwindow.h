#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "genelistpanel.h"
#include "genesettingspanel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GeneListPanel *gene_list_panel_{nullptr};
    GeneSettingsPanel *gene_settings_panel_{nullptr};

    void initUI();

};
#endif // MAINWINDOW_H

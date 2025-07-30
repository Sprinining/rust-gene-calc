#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initUI();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initUI() {
    gene_list_panel_ = new GeneListPanel(this);
    gene_settings_panel_ = new GeneSettingsPanel(this);
    ui->verticalLayoutGeneList->addWidget(gene_list_panel_);
    ui->verticalLayoutGeneSetting->addWidget(gene_settings_panel_);
}

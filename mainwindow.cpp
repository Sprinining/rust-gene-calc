#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initUI();
    initSingals();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initUI() {
    gene_list_panel_ = new GeneListPanel(this);
    gene_settings_panel_ = new GeneSettingsPanel(this);
    ui->verticalLayoutGeneList->addWidget(gene_list_panel_);
    ui->verticalLayoutGeneSetting->addWidget(gene_settings_panel_);
}

void MainWindow::initSingals() {
    connect(gene_settings_panel_, &GeneSettingsPanel::seedInputFinished,
            gene_list_panel_, &GeneListPanel::addSeedItem);
    connect(gene_settings_panel_, &GeneSettingsPanel::requestSeedCalculation,
            gene_list_panel_, &GeneListPanel::onCalculateRequested);
    connect(gene_list_panel_, &GeneListPanel::calculationFinished,
            gene_settings_panel_, &GeneSettingsPanel::displayCalculationResult);
}

#include "genecalcpanel.h"
#include "ui_genecalcpanel.h"

GeneCalcPanel::GeneCalcPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GeneCalcPanel)
{
    ui->setupUi(this);
}

GeneCalcPanel::~GeneCalcPanel()
{
    delete ui;
}

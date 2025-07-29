#include "genepanel.h"
#include "ui_genepanel.h"

GenePanel::GenePanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GenePanel)
{
    ui->setupUi(this);
}

GenePanel::~GenePanel()
{
    delete ui;
}

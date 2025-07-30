#include "genelistpanel.h"
#include "ui_genelistpanel.h"

GeneListPanel::GeneListPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GeneListPanel)
{
    ui->setupUi(this);
}

GeneListPanel::~GeneListPanel()
{
    delete ui;
}

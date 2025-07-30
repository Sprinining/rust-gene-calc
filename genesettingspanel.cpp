#include "genesettingspanel.h"
#include "ui_genesettingspanel.h"

GeneSettingsPanel::GeneSettingsPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GeneSettingsPanel)
{
    ui->setupUi(this);
}

GeneSettingsPanel::~GeneSettingsPanel()
{
    delete ui;
}

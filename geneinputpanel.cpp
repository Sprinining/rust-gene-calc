#include "geneinputpanel.h"
#include "ui_geneinputpanel.h"

GeneInput::GeneInput(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GeneInputPanel)
{
    ui->setupUi(this);
}

GeneInput::~GeneInput()
{
    delete ui;
}

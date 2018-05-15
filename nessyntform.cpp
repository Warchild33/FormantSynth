#include "nessyntform.h"
#include "ui_nessyntform.h"

NesSyntForm::NesSyntForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NesSyntForm)
{
    ui->setupUi(this);
}

NesSyntForm::~NesSyntForm()
{
    delete ui;
}

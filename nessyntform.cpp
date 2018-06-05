#include "nessyntform.h"
#include "ui_nessyntform.h"
#include "nessynth.h"

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

void NesSyntForm::on_squareButton_clicked()
{
    synt.gen_type = SQUARE;
}

void NesSyntForm::on_triangleButton_clicked()
{
    synt.gen_type = TRIANGLE;
}

void NesSyntForm::on_NoiseBut_clicked()
{
    synt.gen_type = NOISE;
}

void NesSyntForm::on_radioButton_4_clicked()
{
    synt.set_percent(0.125);
}

void NesSyntForm::on_radioButton_5_clicked()
{
    synt.set_percent(0.25);
}

void NesSyntForm::on_radioButton_6_clicked()
{
    synt.set_percent(0.50);
}

void NesSyntForm::on_radioButton_7_clicked()
{
    synt.set_percent(0.75);
}

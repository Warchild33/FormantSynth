#include "fm_dialog.h"
#include "fm.h"
#include "ui_fm_dialog.h"



FM_Dialog::FM_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FM_Dialog)
{
    ui->setupUi(this);
    synt = new FMSynth();
}

FM_Dialog::~FM_Dialog()
{
    delete ui;
}

void FM_Dialog::on_I1_valueChanged(double arg1)
{
    synt->gui_params.I[1] = arg1;
}

void FM_Dialog::on_I2_valueChanged(double arg1)
{
    synt->gui_params.I[2] = arg1;
}


void FM_Dialog::on_test1_clicked()
{
    synt->n_test = 1;
    synt->gui_params.f[1] = ui->f1->value();
    synt->gui_params.f[2] = ui->f2->value();
    synt->gui_params.I[1] = ui->I1->value();
    synt->gui_params.I[2] = ui->I2->value();

}

void FM_Dialog::on_test2_clicked()
{
    synt->n_test = 2;
    ui->I3->setValue(0.6);
    synt->gui_params.I[3] = 0.6;
    ui->I2->setValue(0.7);
    synt->gui_params.I[2] = 0.7;
    ui->I6->setValue(0.7);
    synt->gui_params.I[6] = 0.7;
    ui->I1->setValue(0.99);
    synt->gui_params.I[1] = 0.99;
    ui->I5->setValue(0.85);
    synt->gui_params.I[5] = 0.85;
    ui->f6->setValue(1.51);
    ui->d6->setValue(-1);
    synt->gui_params.f[6] = 1.51;
    synt->gui_params.d[6] = -1;
    ui->f4->setValue(1.50);
    ui->d4->setValue(7);
    synt->gui_params.f[4] = 1.50;
    synt->gui_params.d[4] = 7;
    ui->f5->setValue(3.03);
    ui->d5->setValue(-7);
    synt->gui_params.f[5] = 3.03;
    synt->gui_params.d[5] = -7;



}

void FM_Dialog::on_f1_valueChanged(double arg1)
{
    synt->gui_params.f[1] = ui->f1->value();
}

void FM_Dialog::on_f2_valueChanged(double arg1)
{
    synt->gui_params.f[2] = ui->f2->value();
}

void FM_Dialog::on_f3_valueChanged(double arg1)
{
    synt->gui_params.f[3] = ui->f3->value();
}

void FM_Dialog::on_f4_valueChanged(double arg1)
{
    synt->gui_params.f[4] = ui->f4->value();
}

void FM_Dialog::on_f5_valueChanged(double arg1)
{
    synt->gui_params.f[5] = ui->f5->value();
}

void FM_Dialog::on_f6_valueChanged(double arg1)
{
    synt->gui_params.f[6] = ui->f6->value();
}

void FM_Dialog::on_I5_valueChanged(double arg1)
{
    synt->gui_params.I[5] = arg1;
}

void FM_Dialog::on_I3_valueChanged(double arg1)
{
    synt->gui_params.I[3] = arg1;
}

void FM_Dialog::on_I4_valueChanged(double arg1)
{
    synt->gui_params.I[4] = arg1;
}

void FM_Dialog::on_I6_valueChanged(double arg1)
{
    synt->gui_params.I[6] = arg1;
}

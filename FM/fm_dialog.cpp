#include "fm_dialog.h"
#include "fm.h"
#include "ui_fm_dialog.h"



FM_Dialog::FM_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FM_Dialog)
{
    ui->setupUi(this);
    connect(ui->CopyTo1, SIGNAL(clicked(bool)),this, SLOT(on_CopyTo1_clicked()));
    connect(ui->CopyTo2, SIGNAL(clicked(bool)),this, SLOT(on_CopyTo1_clicked()));
    connect(ui->CopyTo3, SIGNAL(clicked(bool)),this, SLOT(on_CopyTo1_clicked()));
    connect(ui->CopyTo4, SIGNAL(clicked(bool)),this, SLOT(on_CopyTo1_clicked()));
    connect(ui->CopyTo5, SIGNAL(clicked(bool)),this, SLOT(on_CopyTo1_clicked()));
    connect(ui->CopyTo6, SIGNAL(clicked(bool)),this, SLOT(on_CopyTo1_clicked()));
    for(int i=1; i <= 6; i++)
     for(int j=0; j < 4; j++)
     {
         QSlider *slider = this->findChild<QSlider *>("level"+QString::number(i)+QString::number(j));
         connect(slider, SIGNAL(sliderReleased()), this, SLOT(on_rate10_sliderMoved()));
         QSlider *slider2 = this->findChild<QSlider *>("rate"+QString::number(i)+QString::number(j));
         slider2->setMinimum(1);
         connect(slider2, SIGNAL(sliderReleased()), this, SLOT(on_rate10_sliderMoved()));
        // synt->gui_params.rate[i][2] = 4;
     }

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
    synt->gui_params.d[6] = -0.58;
    ui->f4->setValue(1.50);
    ui->d4->setValue(7);
    synt->gui_params.f[4] = 1.50;
    synt->gui_params.d[4] = 3;
    ui->f5->setValue(3.03);
    ui->d5->setValue(-7);
    synt->gui_params.f[5] = 3.03;
    synt->gui_params.d[5] = -3;



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

void FM_Dialog::AssignGUIValues()
{
    synt->gui_params.I[1] = ui->I1->value();
    synt->gui_params.I[2] = ui->I2->value();
    synt->gui_params.I[3] = ui->I3->value();
    synt->gui_params.I[4] = ui->I4->value();
    synt->gui_params.I[5] = ui->I5->value();
    synt->gui_params.I[6] = ui->I6->value();

    synt->gui_params.f[1] = ui->f1->value();
    synt->gui_params.f[2] = ui->f2->value();
    synt->gui_params.f[3] = ui->f3->value();
    synt->gui_params.f[4] = ui->f4->value();
    synt->gui_params.f[5] = ui->f5->value();
    synt->gui_params.f[6] = ui->f6->value();

    synt->gui_params.d[1] = ui->d1->value();
    synt->gui_params.d[2] = ui->d2->value();
    synt->gui_params.d[3] = ui->d3->value();
    synt->gui_params.d[4] = ui->d4->value();
    synt->gui_params.d[5] = ui->d5->value();
    synt->gui_params.d[6] = ui->d6->value();

    //evenlope
    for(int i=1; i <= 6; i++)
     for(int j=0; j < 4; j++)
     {
         QSlider *slider = this->findChild<QSlider *>("level"+QString::number(i)+QString::number(j));
         synt->gui_params.level[i][j+1] = (float)slider->value()/100.;
         QSlider *slider2 = this->findChild<QSlider *>("rate"+QString::number(i)+QString::number(j));
         synt->gui_params.rate[i][j] = (float)slider2->value()/50.;
        // synt->gui_params.rate[i][2] = 4;
     }

}

void FM_Dialog::on_pushButton_3_clicked()
{
    synt->n_test = 3;
    ui->I6->setValue(0.79);
    ui->I5->setValue(0.99);
    ui->I4->setValue(0.89);
    ui->I3->setValue(0.99);
    ui->I2->setValue(0.58);
    ui->I1->setValue(0.99);

    ui->f6->setValue(1);
    ui->f5->setValue(1);
    ui->f4->setValue(1);
    ui->f3->setValue(1);
    ui->f2->setValue(14);
    ui->f1->setValue(1);

    ui->d6->setValue(7);
    ui->d5->setValue(-7);
    ui->d4->setValue(0);
    ui->d3->setValue(0);
    ui->d2->setValue(0);
    ui->d1->setValue(3);
    for(int i=1; i <=6; i++)
    {
        QDoubleSpinBox *w = this->findChild<QDoubleSpinBox *>("d"+QString::number(i));
        connect(w, SIGNAL(valueChanged(double)),this,SLOT(on_d1_valueChanged(double)));
        w = this->findChild<QDoubleSpinBox *>("I"+QString::number(i));
        connect(w, SIGNAL(valueChanged(double)),this,SLOT(on_d1_valueChanged(double)));
        w = this->findChild<QDoubleSpinBox *>("f"+QString::number(i));
        connect(w, SIGNAL(valueChanged(double)),this,SLOT(on_d1_valueChanged(double)));

    }
    //evenlope params 6
    synt->gui_params.level[6][0]=0;
    synt->gui_params.level[6][1]=0.9;     //attack
    synt->gui_params.level[6][2]=0.9;   //decay
    synt->gui_params.level[6][3]=0;   //sustain
    synt->gui_params.level[6][4]=0;   //release
    synt->gui_params.rate[6][0]=0.01;    //attack time
    synt->gui_params.rate[6][1]=0.1;    //decay time
    synt->gui_params.rate[6][2]=0.4;      //sustain time
    synt->gui_params.rate[6][3]=0.2;    //release time

    //evenlope params 2
    synt->gui_params.level[2][0]=0;
    synt->gui_params.level[2][1]=0.9;     //attack
    synt->gui_params.level[2][2]=0.7;   //decay
    synt->gui_params.level[2][3]=0;   //sustain
    synt->gui_params.level[2][4]=0;   //release
    synt->gui_params.rate[2][0]=0.01;    //attack time
    synt->gui_params.rate[2][1]=0.1;    //decay time
    synt->gui_params.rate[2][2]=0.4;      //sustain time
    synt->gui_params.rate[2][3]=0.2;    //release time
    for(int i=1; i <= 6; i++)
     for(int j=0; j < 4; j++)
     {
         QSlider *slider = this->findChild<QSlider *>("level"+QString::number(i)+QString::number(j));
         slider->setValue(synt->gui_params.level[6][j+1]*100);
         QSlider *slider2 = this->findChild<QSlider *>("rate"+QString::number(i)+QString::number(j));
         slider2->setValue(synt->gui_params.rate[6][j]*50);

     }
    ui->algosvg->SvgLoad("./images/algo5.svg");
    ui->algosvg->LoadDom("./images/algo5.svg");
    ui->algosvg->repaint();

    AssignGUIValues();
}

void FM_Dialog::on_rate10_sliderMoved()
{
    QSlider* slider = (QSlider*)sender();
    AssignGUIValues();
    if(slider->objectName().contains("level"))
        ui->label_4->setText(slider->objectName() + " " + QString::number(slider->value()/100.));
    else
        ui->label_4->setText(slider->objectName() + " " + QString::number(slider->value()/50.));

}


void FM_Dialog::on_d1_valueChanged(double arg1)
{
    synt->gui_params.I[1] = ui->I1->value();
    synt->gui_params.I[2] = ui->I2->value();
    synt->gui_params.I[3] = ui->I3->value();
    synt->gui_params.I[4] = ui->I4->value();
    synt->gui_params.I[5] = ui->I5->value();
    synt->gui_params.I[6] = ui->I6->value();

    synt->gui_params.f[1] = ui->f1->value();
    synt->gui_params.f[2] = ui->f2->value();
    synt->gui_params.f[3] = ui->f3->value();
    synt->gui_params.f[4] = ui->f4->value();
    synt->gui_params.f[5] = ui->f5->value();
    synt->gui_params.f[6] = ui->f6->value();

    synt->gui_params.d[1] = ui->d1->value();
    synt->gui_params.d[2] = ui->d2->value();
    synt->gui_params.d[3] = ui->d3->value();
    synt->gui_params.d[4] = ui->d4->value();
    synt->gui_params.d[5] = ui->d5->value();
    synt->gui_params.d[6] = ui->d6->value();
}


void FM_Dialog::on_pushButton_9_clicked()
{

}

void FM_Dialog::on_CopyTo1_clicked()
{
    QPushButton* but =  (QPushButton*)sender();
    QString s = but->objectName();
    s = s.right(s.length()-6);
    int n = s.toInt();
    QComboBox *combo = this->findChild<QComboBox *>("ComboTo"+QString::number(n));

    if(combo->currentIndex()==0)
    {
        for(int i=1; i < 6; i++)
         for(int j=0; j < 4; j++)
         {
             QSlider *slider = this->findChild<QSlider *>("level"+QString::number(i)+QString::number(j));
             slider->setValue(synt->gui_params.level[n][j+1]*100);
             QSlider *slider2 = this->findChild<QSlider *>("rate"+QString::number(i)+QString::number(j));
             slider2->setValue(synt->gui_params.rate[n][j]*50);
         }
    }
     AssignGUIValues();
}

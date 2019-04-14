#include "fm_dialog.h"
#include "fm.h"
#include "ploter.h"
#include "ui_fm_dialog.h"

extern Ploter* p;

FM_Dialog::FM_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FM_Dialog)
{
    ui->setupUi(this);
    disableAsignFromGUI = false;
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
         connect(slider, SIGNAL(valueChanged(int)), this, SLOT(on_sliderMoved(int)) );
         QSlider *slider2 = this->findChild<QSlider *>("rate"+QString::number(i)+QString::number(j));
         slider2->setMinimum(1);
         connect(slider2, SIGNAL(sliderReleased()), this, SLOT(on_rate10_sliderMoved()));
         connect(slider2, SIGNAL(valueChanged(int)), this, SLOT(on_sliderMoved(int)) );
        // synt->gui_params.rate[i][2] = 4;
     }

    for(int i=1; i <=6; i++)
    {
        QDoubleSpinBox *w = this->findChild<QDoubleSpinBox *>("d"+QString::number(i));
        connect(w, SIGNAL(valueChanged(double)),this,SLOT(on_d1_valueChanged(double)));
        w = this->findChild<QDoubleSpinBox *>("I"+QString::number(i));
        connect(w, SIGNAL(valueChanged(double)),this,SLOT(on_d1_valueChanged(double)));
        w = this->findChild<QDoubleSpinBox *>("f"+QString::number(i));
        connect(w, SIGNAL(valueChanged(double)),this,SLOT(on_d1_valueChanged(double)));
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

}

void FM_Dialog::on_test2_clicked()
{

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

void FM_Dialog::AssignToGUI()
{
    disableAsignFromGUI = true;
    for(int i=1; i <= 6; i++)
    {
        QDoubleSpinBox *I = this->findChild<QDoubleSpinBox *>("I"+QString::number(i));
        I->setValue(synt->gui_params.I[i]);
        QDoubleSpinBox *F = this->findChild<QDoubleSpinBox *>("f"+QString::number(i));
        F->setValue(synt->gui_params.f[i]);
        QDoubleSpinBox *d = this->findChild<QDoubleSpinBox *>("d"+QString::number(i));
        d->setValue(synt->gui_params.d[i]);
    }
        //evenlope
        for(int i=1; i <= 6; i++)
         for(int j=0; j < 4; j++)
         {
             QSlider *slider = this->findChild<QSlider *>("level"+QString::number(i)+QString::number(j));
             slider->setValue(synt->gui_params.level[i][j+1]);
             QSlider *slider2 = this->findChild<QSlider *>("rate"+QString::number(i)+QString::number(j));
             slider2->setValue(synt->gui_params.rate[i][j]);
            // synt->gui_params.rate[i][2] = 4;
         }
     ui->algoCombo->setCurrentIndex(synt->gui_params.algo_n-1);
     disableAsignFromGUI = false;

}

void FM_Dialog::AssignGUIValues()
{
    if(disableAsignFromGUI) return;

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
    synt->gui_params.algo_n = ui->algoCombo->currentIndex()+1;

//    //evenlope
    for(int i=1; i <= 6; i++)
     for(int j=0; j < 4; j++)
     {
         QSlider *slider = this->findChild<QSlider *>("level"+QString::number(i)+QString::number(j));
         synt->gui_params.level[i][j+1] = (float)slider->value();
         QSlider *slider2 = this->findChild<QSlider *>("rate"+QString::number(i)+QString::number(j));
         synt->gui_params.rate[i][j] = (float)slider2->value();
        // synt->gui_params.rate[i][2] = 4;
     }

}

void FM_Dialog::on_pushButton_3_clicked()
{

}

void FM_Dialog::Show_Envelope(int n_op)
{
    int levels[4];
    int rates[4];
    for(int i=0; i < 4; i++)
    {
        levels[i] = synt->gui_params.level[n_op][i+1];
        rates[i] = synt->gui_params.rate[n_op][i];
    }
    EnvelopeFM ev(levels, rates);

    p->clearvals(0);
    for(int n=0; n < 2*48000; n++)
    {
        p->setXY(0,(float)n/48000.,ev.render());
    }
    p->update_data();
    p->set_autoscale(true);

}

void FM_Dialog::on_sliderMoved(int value)
{
    QSlider* slider = (QSlider*)sender();
    QString s = slider->objectName();
    QString n,i;
    if(s.contains("rate"))
    {
      n = s.right(s.length()-4).left(1);
      i = s.right(s.length()-4).right(1);
      synt->gui_params.rate[n.toInt()][i.toInt()] = value;
    }
    if(s.contains("level"))
    {
        n = s.right(s.length()-5).left(1);
        i = s.right(s.length()-5).right(1);
        synt->gui_params.level[n.toInt()][i.toInt()+1] = value;
    }



    Show_Envelope(n.toInt());

    QString templ = "<html><head/><body><p><span style=\" font-size:16pt;\">%1</span></p></body></html>";

    if(slider->objectName().contains("level"))
    {
        templ = templ.arg("L " + QString::number(slider->value()));
        ui->label_4->setText(templ);
    }
    else
    {
        templ = templ.arg("R " + QString::number(slider->value()));
        ui->label_4->setText(templ);
    }

}

void FM_Dialog::on_rate10_sliderMoved()
{
    QSlider* slider = (QSlider*)sender();
    QString s = slider->objectName();
    if(s.contains("rate"))
      s = s.right(s.length()-4).left(1);
    if(s.contains("level"))
        s = s.right(s.length()-5).left(1);
    Show_Envelope(s.toInt());

    AssignGUIValues();
    QString templ = "<html><head/><body><p><span style=\" font-size:16pt;\">%1</span></p></body></html>";

    if(slider->objectName().contains("level"))
    {
        templ = templ.arg("L " + QString::number(slider->value()));
        ui->label_4->setText(templ);
    }
    else
    {
        templ = templ.arg("R " + QString::number(slider->value()));
        ui->label_4->setText(templ);
    }

}


void FM_Dialog::on_d1_valueChanged(double arg1)
{
    if(disableAsignFromGUI) return;
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
             slider->setValue(synt->gui_params.level[n][j+1]);
             QSlider *slider2 = this->findChild<QSlider *>("rate"+QString::number(i)+QString::number(j));
             slider2->setValue(synt->gui_params.rate[n][j]);
         }
    }
     AssignGUIValues();
}

void FM_Dialog::on_test1_2_clicked()
{



}

double rate2time(double rate)
{
    return 1 - rate/100.;
}

void FM_Dialog::initEnvelope(int op, int r1, int r2, int r3, int r4, int l1, int l2, int l3, int l4)
{
    synt->gui_params.level[op][1]=l1;   //attack
    synt->gui_params.level[op][2]=l2;   //decay
    synt->gui_params.level[op][3]=l3;   //sustain
    synt->gui_params.level[op][4]=l4;   //release
    synt->gui_params.rate[op][0]=r1;    //attack RATE
    synt->gui_params.rate[op][1]=r2;    //decay RATE
    synt->gui_params.rate[op][2]=r3;    //sustain RATE
    synt->gui_params.rate[op][3]=r4;    //release RATE

}

void FM_Dialog::initSliders()
{
    for(int i=1; i <= 6; i++)
     for(int j=0; j < 4; j++)
     {
         QSlider *slider = this->findChild<QSlider *>("level"+QString::number(i)+QString::number(j));
         slider->setValue(synt->gui_params.level[i][j+1]);
         QSlider *slider2 = this->findChild<QSlider *>("rate"+QString::number(i)+QString::number(j));
         slider2->setValue(synt->gui_params.rate[i][j]);


     }
}

void FM_Dialog::initFreqs(double f1, double f2, double f3, double f4, double f5, double f6)
{
    ui->f6->setValue(f6);
    ui->f5->setValue(f5);
    ui->f4->setValue(f4);
    ui->f3->setValue(f3);
    ui->f2->setValue(f2);
    ui->f1->setValue(f1);
}

void FM_Dialog::initLevels(double I1, double I2, double I3, double I4, double I5, double I6)
{
    ui->I6->setValue(I6);
    ui->I5->setValue(I5);
    ui->I4->setValue(I4);
    ui->I3->setValue(I3);
    ui->I2->setValue(I2);
    ui->I1->setValue(I1);
}

void FM_Dialog::initDetune(double d1, double d2, double d3, double d4, double d5, double d6)
{
    ui->d6->setValue(d6);
    ui->d5->setValue(d5);
    ui->d4->setValue(d4);
    ui->d3->setValue(d3);
    ui->d2->setValue(d2);
    ui->d1->setValue(d1);
}

void FM_Dialog::on_comboBox_activated(const QString &arg1)
{
    if( arg1 == "bass1" )
    {
        synt->n_test = 4;
        initLevels(0.99,0.80,0.99,0.93,0.62,0.85);
        initFreqs(0.5,0.5,0.5,5.0,0.5,9);
        initDetune(0,0,0,0,0,0);
        initEnvelope(6,94,56,26,55,93,28,0,0);
        initEnvelope(5,99,0,0,0,99,0,0,0);
        initEnvelope(4,90,40,7,55,90,32,2,0);
        initEnvelope(3,88,98,32,30,79,65,0,0);
        initEnvelope(2,99,20,0,0,99,0,0,0);
        initEnvelope(1,95,62,10,58,99,95,32,0);

        //copy to all
        ui->algoCombo->setCurrentIndex(16-1);

        initSliders();

        ui->algosvg->SvgLoad("./images/algo16.svg");
        ui->algosvg->LoadDom("./images/algo16.svg");
        ui->algosvg->repaint();

        AssignGUIValues();
    }

    if( arg1 == "bass2" )
    {
        synt->n_test = 4;
        initLevels(0.99,0.80,0.68,0.99,0.75,0.87);
        initFreqs(0.51,0.52,1.0,0.5,1.01,0.5);
        initDetune(0,0,0,0,0,1);
        initEnvelope(6,25,50,24,56,96,97,0,0);
        initEnvelope(5,99,51,0,0,99,74,0,0);
        initEnvelope(4,80,39,28,53,89,57,0,0);
        initEnvelope(3,73,25,32,30,97,78,0,0);
        initEnvelope(2,28,37,42,50,99,0,0,0);
        initEnvelope(1,75,37,18,63,99,70,0,0);

        //copy to all
        ui->algoCombo->setCurrentIndex(17-1);

        initSliders();

        ui->algosvg->SvgLoad("./images/algo17.svg");
        ui->algosvg->LoadDom("./images/algo17.svg");
        ui->algosvg->repaint();

        AssignGUIValues();

    }

    if( arg1 == "orchestra" )
    {
        synt->n_test = 4;
        initFreqs(1,1,2,2,2,2);
        initLevels(0.99,0.83,0.96,0.72,0.80,0.82);
        initDetune(0,-6,6,0,0,0);
        initEnvelope(6,72,76,10,32,99,92,0,0);
        initEnvelope(5,76,73,10,55,99,94,0,0);
        initEnvelope(4,56,74,10,45,98,98,36,0);
        initEnvelope(3,54,15,10,47,99,92,0,0);
        initEnvelope(2,56,43,32,61,99,93,90,0);
        initEnvelope(1,80,56,10,45,98,98,36,0);
        ui->algoCombo->setCurrentIndex(2-1);

        initSliders();

        ui->algosvg->SvgLoad("./images/algo2.svg");
        ui->algosvg->LoadDom("./images/algo2.svg");
        ui->algosvg->repaint();

        AssignGUIValues();

    }

    if( arg1 == "E.organ1" )
    {
        synt->n_test = 4;
        initFreqs(0.5,1.01,1.5,0.5,1.0,3.0);
        initLevels(0.94,0.94,0.94,0.94,0.94,0.94);
        initDetune(-2,-6,4,5,2,0);
        initEnvelope(1,99,80,22,90,99,99,99,0);
        initEnvelope(2,99,20,22,90,99,99,97,0);
        initEnvelope(3,99,80,54,82,99,99,99,0);
        initEnvelope(4,99,80,22,90,99,99,99,0);
        initEnvelope(5,99,80,22,90,99,99,99,0);
        initEnvelope(6,99,54,22,90,99,0,0,0);

        ui->algoCombo->setCurrentIndex(32-1);

        initSliders();

        ui->algosvg->SvgLoad("./images/algo32.svg");
        ui->algosvg->LoadDom("./images/algo32.svg");
        ui->algosvg->repaint();

        AssignGUIValues();
    }

    if( arg1 == "E.piano" )
    {
        synt->n_test = 4;
        initLevels(0.79,0.99,0.89,0.99,0.58,0.99);
        initFreqs(1,14,1,1,1,1);
        initDetune(3,0,0,0,-7,7);
        initEnvelope(6,95,29,10,10,99,95,0,0);
        initEnvelope(5,95,20,10,10,99,95,0,0);
        initEnvelope(4,95,29,10,10,90,95,0,0);
        initEnvelope(3,95,20,10,10,99,97,0,0);
        initEnvelope(2,97,50,10,10,90,75,0,0);
        initEnvelope(1,96,25,10,10,99,76,0,0);

        ui->algoCombo->setCurrentIndex(5-1);

        initSliders();

        ui->algosvg->SvgLoad("./images/algo5.svg");
        ui->algosvg->LoadDom("./images/algo5.svg");
        ui->algosvg->repaint();

        AssignGUIValues();

    }


    if( arg1 == "piano1" )
    {
        initLevels(0.86,0.85,0.97,0.95,0.87,0.94);
        initFreqs(1,1,7,1,1,0.5);
        initDetune(-4,4,-3,3,-2,2);
        initEnvelope(1,90,30,28,43,99,95,0,0);
        initEnvelope(2,98,26,6,32,91,90,0,0);
        initEnvelope(3,94,80,19,12,83,67,0,0);
        initEnvelope(4,90,64,28,45,99,97,0,0);
        initEnvelope(5,98,20,6,2,91,90,0,0);
        initEnvelope(6,80,73,15,10,99,19,0,0);

        ui->algoCombo->setCurrentIndex(3-1);

        initSliders();

        ui->algosvg->SvgLoad("./images/algo3.svg");
        ui->algosvg->LoadDom("./images/algo3.svg");
        ui->algosvg->repaint();

        AssignGUIValues();


    }

    if( arg1 == "piano2" )
    {

        initLevels(0.86,0.85,0.97,0.95,0.87,0.94);
        initFreqs(1,1,7,1,1,0.5);
        initDetune(-4,4,-3,3,-2,2);
        initEnvelope(1,90,30,28,43,99,95,0,0);
        initEnvelope(2,98,26,6,32,91,90,0,0);
        initEnvelope(3,94,80,19,12,83,67,0,0);
        initEnvelope(4,90,64,28,45,99,97,0,0);
        initEnvelope(5,98,20,6,2,91,90,0,0);
        initEnvelope(6,80,73,15,10,99,19,0,0);

        ui->algoCombo->setCurrentIndex(1-1);

        initSliders();

        ui->algosvg->SvgLoad("./images/algo3.svg");
        ui->algosvg->LoadDom("./images/algo3.svg");
        ui->algosvg->repaint();

        AssignGUIValues();


    }
}

void FM_Dialog::on_rate11_sliderMoved(int position)
{

}

void FM_Dialog::on_OSC_clicked(bool checked)
{
    synt->bShowOSC =checked;
}

void FM_Dialog::on_algoCombo_currentIndexChanged(int index)
{

}

void FM_Dialog::on_algoCombo_currentIndexChanged(const QString &arg1)
{
    ui->algosvg->SvgLoad("./images/algo"+arg1+".svg");
    ui->algosvg->LoadDom("./images/algo"+arg1+".svg");
    ui->algosvg->repaint();

    synt->gui_params.algo_n = ui->algoCombo->currentIndex()+1;

}

void FM_Dialog::on_comboBox_2_currentIndexChanged(int index)
{
    synt->n_op_osc = index;
}

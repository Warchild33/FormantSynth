#include <QProcess>
#include "happybirsday.h"
#include "formantsyntform.h"
#include "ui_formantsyntform.h"

FormantSyntForm::FormantSyntForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormantSyntForm)
{
    ui->setupUi(this);
}

FormantSyntForm::~FormantSyntForm()
{
    delete ui;
}

void FormantSyntForm::on_hbSongButton_clicked()
{
    synt.alsa->close();
    Happybirsday hb_song;
    hb_song.set_synth(synt);
    std::vector<Notestruct> notes = hb_song.parse_hb_notes("./midi_data/happy_birsday.txt");
    hb_song.generate_song(notes);
    Play("./wave/hb_song.wav");

}
void FormantSyntForm::Play(QString fn)
{
    QProcess process1;
    QProcess process2;
    process1.start(QString("bash -c \"mplayer %1\" ").arg(fn));
    if (!process1.waitForFinished())
             return ;
}

void FormantSyntForm::set_params()
{
    synt.F1 = ui->F1->value();
    synt.F2 = ui->F2->value();
    synt.F3 = ui->F3->value();
    synt.BW = ui->BW->value();
    synt.Ncascade = ui->Ncascade->value();
}

void FormantSyntForm::on_Abutton_clicked()
{
    ui->F1->setValue(660);
    ui->F2->setValue(1700);
    ui->F3->setValue(2400);
    ui->BW->setValue(0.0066);
    ui->Ncascade->setValue(5);
    synt.cur_vowel = 'A';
    set_params();
    //synt.play_note(32,2,1);
    //on_genButton_clicked();
    //Play();
}

void FormantSyntForm::on_Ibutton_clicked()
{
    ui->F1->setValue(270);
    ui->F2->setValue(2300);
    ui->F3->setValue(3000);
    ui->BW->setValue(0.0033);
    ui->Ncascade->setValue(10);
    synt.cur_vowel = 'I';
    //on_genButton_clicked();
    //Play();

}

void FormantSyntForm::on_Ebutton_clicked()
{
    ui->F1->setValue(400);
    ui->F2->setValue(2000);
    ui->F3->setValue(2550);
    ui->BW->setValue(0.0066);
    ui->Ncascade->setValue(5);
    synt.cur_vowel = 'E';
    //on_genButton_clicked();
    //Play();
}


void FormantSyntForm::on_Ubut_clicked()
{
    synt.cur_vowel = 'U';
}

void FormantSyntForm::on_Ncascade_valueChanged(double arg1)
{
    synt.Ncascade = arg1;
}

void FormantSyntForm::on_F1_valueChanged(double arg1)
{
    set_params();
}

void FormantSyntForm::on_F2_valueChanged(double arg1)
{
    set_params();
}

void FormantSyntForm::on_F3_valueChanged(double arg1)
{
    set_params();
}

void FormantSyntForm::on_BW_valueChanged(double arg1)
{
    set_params();
}

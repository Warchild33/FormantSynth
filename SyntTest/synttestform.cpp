#include <QSettings>
#include "synttestform.h"
#include "ui_nessyntform.h"
#include "testsynth.h"
#include "print.h"
#include "fft.h"
#include "common.h"
#include "wave_in.h"

static QSettings settings("./settings/settings.ini", QSettings::IniFormat);

SyntTestForm::SyntTestForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NesSyntForm)
{
    ui->setupUi(this);
    map_to_prn(this);    
    synt = new TestSynth();
    isPlaying = false;
}

void SyntTestForm::print(QString s)
{
    ui->console->appendPlainText(s);
}

SyntTestForm::~SyntTestForm()
{
    delete ui;
}

void SyntTestForm::on_squareButton_clicked()
{
    synt->gen_type = SQUARE;
}

void SyntTestForm::on_triangleButton_clicked()
{
    synt->gen_type = TRIANGLE;
}

void SyntTestForm::on_NoiseBut_clicked()
{
    synt->gen_type = NOISE;
}

void SyntTestForm::on_radioButton_4_clicked()
{
    synt->set_percent(0.125);
}

void SyntTestForm::on_radioButton_5_clicked()
{
    synt->set_percent(0.25);
}

void SyntTestForm::on_radioButton_6_clicked()
{
    synt->set_percent(0.50);
}

void SyntTestForm::on_radioButton_7_clicked()
{
    synt->set_percent(0.75);
}

void SyntTestForm::on_squareButton_2_clicked()
{
    synt->gen_type = NOISE2;
}

void SyntTestForm::on_pushButton_clicked()
{

}

void SyntTestForm::on_squareButton_3_clicked()
{
    synt->gen_type = GUITAR;
}

void SyntTestForm::on_hbSongButton_clicked()
{
    //synt.alsa->close();
    if( !isPlaying)
    {
        hb_song = new Happybirsday();
        hb_song->set_synth(synt);
        std::vector<Notestruct>* notes = new std::vector<Notestruct>();
        *notes = hb_song->parse_hb_notes("./midi_data/happy_birsday.txt");
        settings.setValue("multiple_voice",1);
        hb_song->Play(notes, ui->progressBar);
        ui->hbSongButton->setText("Stop");
    }
    else
    {
        ui->hbSongButton->setText("Happy birsday song");
        hb_song->Stop();
        delete hb_song;
    }
    isPlaying = !isPlaying;
}

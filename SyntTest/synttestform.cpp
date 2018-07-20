#include <QSettings>
#include "synttestform.h"
#include "ui_nessyntform.h"
#include "testsynth.h"
#include "print.h"
#include "fft.h"
#include "common.h"
#include "wave_in.h"
#include "freqtable.h"

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

std::vector<int> gen_cyclic_group(int i)
{
    std::vector<int> g;

    g.push_back(i);
    g.push_back(i);

    int j=1;
    int val=1;
    while(val!=0)
    {
        val = (g[j-1] + g[j]) % 6 ;
        g.push_back( val );
        j++;
    }
    return g;
}

void generate_music(std::vector<Notestruct>& notes)
{
    const int N_NOTES = 50;
    const int N_GROUPS = 50;

    FreqTable  freq_table;
    MidiNote c4 = freq_table.getNoteByName("C4");
    char startNote = c4.note;

    float dt[] = {1, 1./2, 1./4, 1./8, 1./16};
    float t = 0;
    for(int i=0; i < N_GROUPS; i++)
    {
        std::vector<int> g = gen_cyclic_group(rand()%7);
        for(auto n=g.begin(); n!=g.end(); n++)
        {
            Notestruct note;
            note.note = startNote + *n;
            note.t_start = t;
            note.t_end = t + dt[rand()%4]*0.1;
            t = note.t_end + dt[rand()%4]*0.1;
            notes.push_back(note);
        }
    }
}

void SyntTestForm::on_musicGeneratorBN_clicked()
{
    //synt.alsa->close();
    if( !isPlaying)
    {
        hb_song = new Happybirsday();
        hb_song->set_synth(synt);
        std::vector<Notestruct>* notes = new std::vector<Notestruct>();
        generate_music(*notes);
        settings.setValue("multiple_voice",1);
        hb_song->Play(notes, ui->progressBar);
        ui->musicGeneratorBN->setText("Stop");
    }
    else
    {
        ui->musicGeneratorBN->setText("Music generator");
        hb_song->Stop();
        delete hb_song;
    }
    isPlaying = !isPlaying;

}

void parseFrac(QString str, int& numerator, int& denominator)
{
    QStringList parts = str.split("/");
    numerator = parts[0].toDouble();
    denominator = parts[1].toDouble();
}

void SyntTestForm::on_ratio_textChanged()
{
    int numerator, denominator;
    parseFrac(ui->ratio->toPlainText(), numerator, denominator);
    synt->ratio = (double)numerator / (double)denominator;
}

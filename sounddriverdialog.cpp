#include "sounddriverdialog.h"
#include "ui_sounddriverdialog.h"
#include <QDebug>
#include <QSettings>
#include <QProcess>
#include <QFile>
#include <QTextStream>


QSettings settings("./settings/settings.ini", QSettings::IniFormat);

SoundDriverDialog::SoundDriverDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SoundDriverDialog)
{
    ui->setupUi(this);

//    ui->comboBox->addItem("hw:0,0");
//    ui->comboBox->addItem("hw:0,0");
//    ui->comboBox->addItem("hw:1,0");
//    ui->comboBox->addItem("hw:2,0");
//    ui->comboBox->addItem("hw:0,1");
//    ui->comboBox->addItem("hw:1,1");
//    ui->comboBox->addItem("hw:1,2");
    listdev();
    ui->comboBox->addItem("plug:dmix");
    ui->comboBox->addItem("pulseaudio");

    //ui->comboBox->setCurrentText(settings.value("alsa_device").toString());
    ui->comboBox->setItemText(0,settings.value("alsa_device").toString());
    alsa = new AlsaDriver();
    alsa->open((char*)ui->comboBox->currentText().toStdString().c_str(),true);
}

void SoundDriverDialog::listdev()
{
    QProcess process1;
    process1.start(QString("bash -c \"cat /proc/asound/devices > /tmp/listdev.000\""));
    if (!process1.waitForFinished())
             return ;

    QFile   f("/tmp/listdev.000");
    if( f.open( QFile::ReadOnly | QFile::Text) )
    {
         QString line;
         QTextStream stream(&f);
         ui->comboBox->clear();
         ui->comboBox->addItem("hw:0,0");
         while( !stream.atEnd() )
         {
             QString line = stream.readLine();
             if( line.contains("playback") )
             {
                 QRegExp rx("(\\d+)[ -]+(\\d+)");
                 if(rx.indexIn(line)!=-1)
                 {
                     QStringList list = rx.capturedTexts();
                     int card = rx.cap(1).toInt();
                     int device = rx.cap(2).toInt();
                     ui->comboBox->addItem(QString("hw:%1,%2").
                                           arg(card).arg(device));
                 }

             }
             //ui->comboBox->addItem();
         }
    }

}

SoundDriverDialog::~SoundDriverDialog()
{
    delete ui;
}

void SoundDriverDialog::on_testButton_clicked()
{  
    short buf[128];
    alsa->out_pcm(buf, 128);
}

void SoundDriverDialog::on_comboBox_activated(const QString &arg1)
{
    //alsa->close();
    if(ui->comboBox->currentText()=="pulseaudio")
    {
        settings.setValue("use_driver","pulse");
    }
    else
    {
        settings.setValue("use_driver","alsa");
        alsa->open((char*)ui->comboBox->currentText().toStdString().c_str(), true);
        settings.setValue("alsa_device", arg1);
    }

}

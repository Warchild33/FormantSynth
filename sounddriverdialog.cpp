#include "sounddriverdialog.h"
#include "ui_sounddriverdialog.h"
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

    ui->comboBox->addItem("hw:0,0");
    ui->comboBox->addItem("hw:1,0");
    ui->comboBox->addItem("hw:2,0");    
    listdev();
    ui->comboBox->setCurrentText(settings.value("alsa_device").toString());
    alsa = new AlsaDriver();
    alsa->open((char*)ui->comboBox->currentText().toStdString().c_str());
}

void SoundDriverDialog::listdev()
{
    QProcess process1;
    process1.start(QString("bash -c \"aplay -L | grep hw > /tmp/listdev.000\""));
    if (!process1.waitForFinished())
             return ;

    QFile   f("/tmp/listdev.000");
    if( f.open( QFile::ReadOnly | QFile::Text) )
    {
         QString line;
         QTextStream stream(&f);
         while( !stream.atEnd() )
             ui->comboBox->addItem(stream.readLine());
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
    alsa->open((char*)ui->comboBox->currentText().toStdString().c_str());
    settings.setValue("alsa_device", arg1);
}

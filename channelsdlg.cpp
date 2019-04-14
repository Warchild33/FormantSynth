#include <QDir>
#include <QComboBox>
#include "channelsdlg.h"
#include "ui_channelsdlg.h"

ChannelsDlg::ChannelsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChannelsDlg)
{
    ui->setupUi(this);
    QDir patches_dir("./patches");
    if (patches_dir.exists())
    {
        QFileInfoList list = patches_dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
                 QFileInfo fileInfo = list.at(i);
                 //fileInfo.fileName()
                 for(int j=0; j < 10; j++)
                 {
                    QComboBox* combo = this->findChild<QComboBox*>("comboBox"+QString::number(j));
                    if(fileInfo.fileName().size() > 2 )
                      combo->addItem(fileInfo.fileName());
                 }
             }
    }

}

int  ChannelsDlg::getEnabledChannels()
{
    int word=0;
    word|=(ui->checkBox_1->isChecked() << 0);
    word|=(ui->checkBox_2->isChecked() << 1);
    word|=(ui->checkBox_3->isChecked() << 2);
    word|=(ui->checkBox_4->isChecked() << 3);
    word|=(ui->checkBox_5->isChecked() << 4);
    word|=(ui->checkBox_6->isChecked() << 5);
    word|=(ui->checkBox_7->isChecked() << 6);
    word|=(ui->checkBox_8->isChecked() << 7);
    word|=(ui->checkBox_9->isChecked() << 8);
    word|=(ui->checkBox_10->isChecked() << 9);
    return word;
}

QString ChannelsDlg::getPatch(int i)
{
    QComboBox* combo = this->findChild<QComboBox*>("comboBox"+QString::number(i));
    return combo->currentText();
}

ChannelsDlg::~ChannelsDlg()
{
    delete ui;
}

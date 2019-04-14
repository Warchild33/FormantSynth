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

QString ChannelsDlg::getPatch(int i)
{
    QComboBox* combo = this->findChild<QComboBox*>("comboBox"+QString::number(i));
    return combo->currentText();
}

ChannelsDlg::~ChannelsDlg()
{
    delete ui;
}

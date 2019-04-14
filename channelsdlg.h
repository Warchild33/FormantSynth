#ifndef CHANNELSDLG_H
#define CHANNELSDLG_H

#include <QDialog>

namespace Ui {
class ChannelsDlg;
}

class ChannelsDlg : public QDialog
{
    Q_OBJECT

public:
     Ui::ChannelsDlg *ui;
    explicit ChannelsDlg(QWidget *parent = 0);
    QString getPatch(int i);
    int     getEnabledChannels();
    ~ChannelsDlg();

};

#endif // CHANNELSDLG_H

#ifndef SOUNDDRIVERDIALOG_H
#define SOUNDDRIVERDIALOG_H

#include <QDialog>

#include "alsadriver.h"

namespace Ui {
class SoundDriverDialog;
}

class SoundDriverDialog : public QDialog
{
    Q_OBJECT

public:
    AlsaDriver* alsa;
    explicit SoundDriverDialog(QWidget *parent = 0);
    ~SoundDriverDialog();
    void listdev();

private slots:
    void on_testButton_clicked();

    void on_comboBox_activated(const QString &arg1);

private:
    Ui::SoundDriverDialog *ui;
};

#endif // SOUNDDRIVERDIALOG_H

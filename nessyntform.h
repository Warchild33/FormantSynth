#ifndef NESSYNTFORM_H
#define NESSYNTFORM_H

#include <QDialog>
#include "nessynth.h"

namespace Ui {
class NesSyntForm;
}

class NesSyntForm : public QDialog
{
    Q_OBJECT

public:
    explicit NesSyntForm(QWidget *parent = 0);
    ~NesSyntForm();
    NesSynth synt;

private slots:

    void on_squareButton_clicked();

    void on_triangleButton_clicked();

    void on_NoiseBut_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_6_clicked();

    void on_radioButton_7_clicked();

private:
    Ui::NesSyntForm *ui;
};

#endif // NESSYNTFORM_H

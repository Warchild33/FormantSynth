#ifndef FORMANTSYNTFORM_H
#define FORMANTSYNTFORM_H

#include <QDialog>
#include "formantsynt.h"

namespace Ui {
class FormantSyntForm;
}

class FormantSyntForm : public QDialog
{
    Q_OBJECT

public:
    explicit FormantSyntForm(QWidget *parent = 0);
    ~FormantSyntForm();
    FormantSynt synt;
    void Play(QString fn);
    void set_params();

private slots:
    void on_hbSongButton_clicked();

    void on_Abutton_clicked();

    void on_Ibutton_clicked();

    void on_Ebutton_clicked();

    void on_Ubut_clicked();

    void on_Ncascade_valueChanged(double arg1);

    void on_F1_valueChanged(double arg1);

    void on_F2_valueChanged(double arg1);

    void on_F3_valueChanged(double arg1);

    void on_BW_valueChanged(double arg1);

private:
    Ui::FormantSyntForm *ui;
};

#endif // FORMANTSYNTFORM_H

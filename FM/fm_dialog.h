#ifndef FM_DIALOG_H
#define FM_DIALOG_H

#include <QDialog>
#
#include "fm.h"

namespace Ui {
class FM_Dialog;
}

class FM_Dialog : public QDialog
{
    Q_OBJECT
public:
    FMSynth* synt;
    Ui::FM_Dialog *ui;
    explicit FM_Dialog(QWidget *parent = 0);
    ~FM_Dialog();

private slots:
    void AssignGUIValues();
    void on_I1_valueChanged(double arg1);

    void on_I2_valueChanged(double arg1);


    void on_test1_clicked();

    void on_test2_clicked();

    void on_f1_valueChanged(double arg1);

    void on_f2_valueChanged(double arg1);

    void on_f3_valueChanged(double arg1);

    void on_f4_valueChanged(double arg1);

    void on_f5_valueChanged(double arg1);

    void on_f6_valueChanged(double arg1);

    void on_I5_valueChanged(double arg1);

    void on_I3_valueChanged(double arg1);

    void on_I4_valueChanged(double arg1);

    void on_I6_valueChanged(double arg1);

    void on_pushButton_3_clicked();

    void on_rate10_sliderMoved();


    void on_d1_valueChanged(double arg1);


    void on_pushButton_9_clicked();

    void on_CopyTo1_clicked();

private:

};

#endif // FM_DIALOG_H

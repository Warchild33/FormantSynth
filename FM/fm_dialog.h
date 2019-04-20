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
    bool disableAsignFromGUI;
    FMSynth* synt;
    Ui::FM_Dialog *ui;
    explicit FM_Dialog(QWidget *parent = 0);
    void Show_Envelope(int n_op);
    ~FM_Dialog();

    void initSliders();
    void AssignToGUI();

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

    void on_test1_2_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_rate11_sliderMoved(int position);

    void initEnvelope(int op, int r1, int r2, int r3, int r4, int l1, int l2, int l3, int l4);
    void initFreqs(double f1, double f2, double f3, double f4, double f5, double f6);
    void initLevels(double I1, double I2, double I3, double I4, double I5, double I6);
    void initDetune(double d1, double d2, double d3, double d4, double d5, double d6);

    void on_OSC_clicked(bool checked);

    void on_algoCombo_currentIndexChanged(int index);

    void on_algoCombo_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(int index);
    void on_sliderMoved(int);

    void on_pichMod_sliderMoved(int position);

private:

};

#endif // FM_DIALOG_H

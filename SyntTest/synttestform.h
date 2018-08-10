#ifndef NESSYNTFORM_H
#define NESSYNTFORM_H

#include <QDialog>
#include "testsynth.h"
#include "happybirsday.h"

namespace Ui {
class NesSyntForm;
}

class SyntTestForm : public QDialog
{
    Q_OBJECT

public:
    explicit SyntTestForm(QWidget *parent = 0);
    ~SyntTestForm();
    TestSynth*    synt;
    Happybirsday* hb_song;
    Ui::NesSyntForm *ui;
    bool isPlaying;
private slots:

    void print(QString s);

    void on_squareButton_clicked();

    void on_triangleButton_clicked();

    void on_NoiseBut_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_6_clicked();

    void on_radioButton_7_clicked();

    void on_squareButton_2_clicked();

    void on_pushButton_clicked();

    void on_squareButton_3_clicked();

    void on_hbSongButton_clicked();

    void on_musicGeneratorBN_clicked();

    void on_ratio_textChanged();

    void on_squareButton_4_clicked();

private:

};

#endif // NESSYNTFORM_H

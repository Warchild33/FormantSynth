#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "syntezer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool isPlaying;
    explicit MainWindow(QWidget *parent = 0);
    void Play(QString fn="pulse_filt.wav");
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent* event);
    ~MainWindow();
signals:
    void keyPressSig(int);
    void keyReleaseSig(int);
public slots:
    void print(const QString& str );

private slots:
    void on_sound_Settings();
    void on_save_patch();
    void on_load_patch();

    void play_test_song();


    void on_tabWidget_currentChanged(QWidget *arg1);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    Syntezer* active_synth;

};

#endif // MAINWINDOW_H

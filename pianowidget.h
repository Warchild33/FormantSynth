#ifndef PIANOWIDGET_H
#define PIANOWIDGET_H

#include <QWidget>
#include <map>
#include <QPoint>
#include "key2notetable.h"

struct PianoKey
{
    QPoint pixel;
    bool   on;
    PianoKey()
        : on(false)
    {}
    PianoKey(QPoint pixel)
        : pixel(pixel), on(false)
    {
    }

};

class PianoWidget : public QWidget
{
    Q_OBJECT
public:
    QImage background;
    std::map<int, PianoKey> piano_keys;
    explicit PianoWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void create_table();

    void drawKeys(QPainter& painter);

signals:
    void sigMouseKey(int key);

public slots:
    void on_key_press(int key_code);
    void on_key_release(int key_code);

};

#endif // PIANOWIDGET_H

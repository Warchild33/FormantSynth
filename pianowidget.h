#ifndef PIANOWIDGET_H
#define PIANOWIDGET_H

#include <QWidget>
#include <map>
#include <QPoint>
#include "key2notetable.h"
#include "svgwidget.h"

struct PianoKey
{
    QPoint pixel;
    bool   on;
    QString name;
    PianoKey()
        : on(false)
    {}
    PianoKey(QPoint pixel)
        : pixel(pixel), on(false)
    {
    }
    PianoKey(QPoint pixel, QString name)
        : pixel(pixel), on(false), name(name)
    {
    }

};

class PianoWidget : public SvgWidget
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

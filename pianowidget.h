#ifndef PIANOWIDGET_H
#define PIANOWIDGET_H

#include <QWidget>
#include <map>
#include <QPoint>
#include "key2notetable.h"
#include "svgwidget.h"

struct PianoKey
{
    QPointF pixel;
    int    code;
    bool   on;
    QString name;
    QRectF  rect;
    QRectF  rect2;
    QRectF  rect3;
    PianoKey()
        : on(false)
    {}
    PianoKey(QPoint pixel)
        : pixel(pixel), on(false)
    {
    }
    PianoKey(QPoint pixel, QString name, QRectF rect)
        : pixel(pixel), on(false), name(name), rect(rect)
    {
        //pixel = rect.center();
    }
    PianoKey(QString name, QRectF rect, QRectF rect2=QRectF(), QRectF rect3=QRectF())
        : on(false), name(name), rect(rect), rect2(rect2), rect3(rect3)
    {
        pixel = rect.center();
    }

};

class PianoWidget : public SvgWidget
{
    Q_OBJECT
public:
    QImage background;
    std::map<int, PianoKey> piano_keys;
    std::map<QString, PianoKey> note2key;
    explicit PianoWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void create_table();

    void drawKeys(QPainter& painter);
    void drawKey(PianoKey key, bool down);

signals:
    void sigMouseKeyDown(int key);
    void sigMouseKeyRelease(int key);

public slots:
    void on_key_press(int key_code);
    void on_key_release(int key_code);
    void on_key_show(QString key_name);
    void on_key_hide(QString key_name);

};

#endif // PIANOWIDGET_H

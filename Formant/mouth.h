#ifndef MOUTH_H
#define MOUTH_H

#include <QPainterPath>
#include <QObject>
#include <QTimerEvent>
#include <QPainter>
#include "svgwidget.h"

class Mouth : public QObject
{
    Q_OBJECT
public:
    Mouth(QString id, SvgWidget* svg);
    void timerEvent(QTimerEvent *);
    void draw();
    int          stroke_width;
    QPainterPath path;
    QPainterPath path2;
    QColor       stroke_color;
    QColor       fill_color;
    int          frames;
    SvgWidget*   my_svg;
    bool         bAnimate;

signals:
    
public slots:
    
};



#endif // MOUTH_H

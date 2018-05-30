#ifndef FORMANTSYNTHSVG_H
#define FORMANTSYNTHSVG_H

#include <QWidget>
#include "svgwidget.h"

class Slider
{
public:
    Slider(){};
    Slider(QRectF r, QString name)
        :rect(r), name(name)
    {
        activeColor = Qt::red;
        defaultColor = QColor(99,168,109);
    }
    QRectF   rect;
    QString name;
    QColor  activeColor;
    QColor  defaultColor;
    double   findNearestPointOnCircle(QPointF mouse, QPointF center, float R);
    double  value;
};

class FormantSynthSvg : public SvgWidget
{
    Q_OBJECT
public:

    explicit FormantSynthSvg(QWidget *parent = nullptr);
    void     paintEvent(QPaintEvent* event);
    void     mousePressEvent(QMouseEvent* event);
    void     mouseMoveEvent(QMouseEvent* event);


    Slider  freqSliders[10];
    int minRotateAngle;
    int maxRotateAngle;

signals:

public slots:
};

#endif // FORMANTSYNTHSVG_H

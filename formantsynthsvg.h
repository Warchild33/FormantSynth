#ifndef FORMANTSYNTHSVG_H
#define FORMANTSYNTHSVG_H

#include <QWidget>
#include "svgwidget.h"

class FormantSynthSvg : public SvgWidget
{
    Q_OBJECT
public:
    explicit FormantSynthSvg(QWidget *parent = nullptr);
    void     paintEvent(QPaintEvent* event);

signals:

public slots:
};

#endif // FORMANTSYNTHSVG_H

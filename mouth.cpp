#include <QTransform>
#include "mouth.h"

int parse_int_val(QString str)
{
    QRegExp rx("(\\d+)");
    rx.indexIn(str);
    return rx.cap(1).toInt();
}

Mouth::Mouth(QString id, SvgWidget* svg)
    :my_svg(svg)
{
    startTimer(50);
    path = svg->getPath(id);
    QString style = svg->getAttr(id, "style");
    stroke_width = parse_int_val(svg->getSubAttr(style, "stroke-width"))*2;
    stroke_color = QColor(svg->getSubAttr(style,"stroke"));
    fill_color = Qt::black;
    bAnimate = false;
    startTimer(50);
}

void Mouth::timerEvent(QTimerEvent *)
{
    QTransform t;
    if( bAnimate )
    {
        float sx = float(frames%9)/10.+0.1;
        QPointF c = path.boundingRect().center();
        t.translate(c.x(),
                    c.y());
        t.scale(sx, sx);
        t.translate(-c.x(),
                    -c.y());

        path2 = t.map(path);
    }
    QTransform t2 = my_svg->getViewBoxTransform();
    path2 = t2.map(path2);
    frames++;
}

void Mouth::draw()
{
    QPainter painter(my_svg);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidth(stroke_width);
    painter.setPen(pen);
    painter.drawPath(path2);
    //painter.fillPath(path2,fill_color);
}

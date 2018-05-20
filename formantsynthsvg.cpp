#include <QTransform>
#include <QtMath>
#include "formantsynthsvg.h"

FormantSynthSvg::FormantSynthSvg(QWidget *parent) : SvgWidget(parent)
{
    minRotateAngle = -270;
    maxRotateAngle = 0;
    SvgLoad("./images/formant_synth.svg");
    LoadDom("./images/formant_synth.svg");
    QPointF center = findPathCenter("F9");
    rotateNode("F9", center, -270);
    setStroke("F9", Qt::red);
    LoadRenderDOM();
    SaveDom("./images/formant_synth_dom.svg");

    //Init Sliders
    for(int i=1; i < 10; i++)
    {
        QString name = QString("F%1").arg(i);
        QRectF rect = getPath(name).boundingRect();
        float w = rect.width();
        rect.setX(rect.center().x() - w );
        rect.setY(rect.center().y() - w );
        rect.setWidth(w*2);
        rect.setHeight(w*2);
        freqSliders[i] = Slider(rect, name);
    }
    setFocus();

}

QPointF  FormantSynthSvg::mapToViewbox(QPoint p, QRectF viewbox)
{
    QPointF pt(p.x(), p.y());
    float sx = (float)viewbox.width() / (float)geometry().width();
    float sy = (float)viewbox.height() / (float)geometry().height();
    QTransform t;
    t = t.scale(sx, sy);
    pt = pt * t;
    return pt;
}

void  FormantSynthSvg::mousePressEvent(QMouseEvent* event)
{



}

double findNearestPointOnCircle(QPointF mouse, QPointF center, float R)
{
    double  X,Y,d, minD=100000000;
    double  min_angle;
    for(double angle=0; angle < 360; angle+=1)
    {
        X = center.x() + R * cos( qDegreesToRadians(angle) );
        Y = center.y() + R * sin( qDegreesToRadians(angle) );
        d = (mouse.x() - X)*(mouse.x() - X) + (mouse.y() - Y)*(mouse.y() - Y);
        if(minD!=qMin(d, minD))
            min_angle = angle;
        minD = qMin(d, minD);
    }
    return 270 - min_angle;
}

void  FormantSynthSvg::mouseMoveEvent(QMouseEvent* event)
{
    float angle;
    for(int i=0; i < 10; i++)
    {
        QPoint pos = event->pos();
        pos.setY(height() - pos.y());
        //fprintf(stderr,"mouse %d %d\n", pos.x(), pos.y());
        QPointF p = mapToViewbox(pos, viewbox);
        angle = findNearestPointOnCircle(p, freqSliders[i].rect.center(),
                                            freqSliders[i].rect.width());
        //fprintf(stderr,"angle %f\n", angle);

        if( freqSliders[i].rect.contains(p) )
        {
            setStroke(freqSliders[i].name, freqSliders[i].activeColor);
            rotateNode(freqSliders[i].name, freqSliders[i].rect.center(), angle);
        }
        else
            setStroke(freqSliders[i].name, freqSliders[i].defaultColor);
    }
    LoadRenderDOM();
    repaint();
}

void FormantSynthSvg::paintEvent(QPaintEvent* event)
{
    /*
    painter.fillRect(this->rect(),Qt::red);
    painter.drawPixmap(this->rect(),QPixmap::fromImage(background));
    */
    SvgWidget::paintEvent(event);
}

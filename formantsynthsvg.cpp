#include <QTransform>
//#include <QtMath>
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

void  FormantSynthSvg::mousePressEvent(QMouseEvent* event)
{



}

double Slider::findNearestPointOnCircle(QPointF mouse, QPointF center, float R)
{
    double  X,Y,d, minD=100000000;
    double  min_angle;
    double  degree2rad = M_PI / 180.;
    for(double angle=310; angle < 310+360; angle+=1)
    {
        X = center.x() + R * cos( degree2rad * angle );
        Y = center.y() + R * sin( degree2rad * angle );
        d = (mouse.x() - X)*(mouse.x() - X) + (mouse.y() - Y)*(mouse.y() - Y);
        if(minD!=qMin(d, minD))
            min_angle = angle;
        minD = qMin(d, minD);
    }
    value = ((670 - min_angle) / 360) * 10000;
    return 270 - min_angle + 55;
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
        angle = freqSliders[i].findNearestPointOnCircle(p, freqSliders[i].rect.center(),
                                                           freqSliders[i].rect.width());

        if( freqSliders[i].rect.contains(p) )
        {
            fprintf(stderr,"value %f\n", freqSliders[i].value);
            setStroke(freqSliders[i].name, freqSliders[i].activeColor);
            rotateNode(freqSliders[i].name, freqSliders[i].rect.center(), angle);
            setText(freqSliders[i].name + "g",
                    QString::number(int(freqSliders[i].value)) + " Hz");
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

#include <QTransform>
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
        freqSliders[i] = Slider(getPath(name).boundingRect(), name);
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

void  FormantSynthSvg::mouseMoveEvent(QMouseEvent* event)
{
    for(int i=0; i < 10; i++)
    {
        QPoint pos = event->pos();
        pos.setY(height() - pos.y());
        //fprintf(stderr,"mouse %d %d\n", pos.x(), pos.y());
        QPointF p = mapToViewbox(pos, viewbox);
        if( freqSliders[i].rect.contains(p) )
            setStroke(freqSliders[i].name, freqSliders[i].activeColor);
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

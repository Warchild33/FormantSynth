#include "formantsynthsvg.h"

FormantSynthSvg::FormantSynthSvg(QWidget *parent) : SvgWidget(parent)
{
    SvgLoad("./images/formant_synth.svg");
    LoadDom("./images/formant_synth.svg");
}

void FormantSynthSvg::paintEvent(QPaintEvent* event)
{
    /*
    painter.fillRect(this->rect(),Qt::red);
    painter.drawPixmap(this->rect(),QPixmap::fromImage(background));
    */
    SvgWidget::paintEvent(event);
}

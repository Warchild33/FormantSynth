#include <QPainter>
#include <QMouseEvent>
#include <QImage>
#include <math.h>
#include "pianowidget.h"

PianoWidget::PianoWidget(QWidget *parent) : SvgWidget(parent)
{
    //background = QImage("./images/piano_keys_and_notes.png");
    SvgLoad("./images/keyboard.svg");
    LoadDom("./images/keyboard.svg");
    create_table();
}

void PianoWidget::paintEvent(QPaintEvent* event)
{
    /*
    painter.fillRect(this->rect(),Qt::red);
    painter.drawPixmap(this->rect(),QPixmap::fromImage(background));
    */
    QPainter painter(this);
    drawKeys(painter);


    SvgWidget::paintEvent(event);
}

void PianoWidget::drawKeys(QPainter& painter)
{
    for(auto key=piano_keys.begin(); key!=piano_keys.end(); key++)
    {
        QPoint pixel = (*key).second.pixel;
        if( (*key).second.on )
        {
            painter.setPen(Qt::red);
            painter.drawEllipse(pixel,5,5);
            setFill((*key).second.name,Qt::red, 0.94);
            LoadRenderDOM();
        }
        else
        {
            setFill((*key).second.name,Qt::white, 0.94);
            LoadRenderDOM();
        }
    }
}

void PianoWidget::on_key_press(int key_code)
{
    piano_keys[key_code].on = true;
    repaint();
}

void PianoWidget::on_key_release(int key_code)
{
   piano_keys[key_code].on = false;
   repaint();
}

void PianoWidget::create_table()
{
    //C4
    piano_keys['Q'] = PianoKey(QPoint(40,140),"C4");    piano_keys['W'] = PianoKey(QPoint(75,140),"D4");
    piano_keys['E'] = PianoKey(QPoint(115,140),"E4");    piano_keys['R'] = PianoKey(QPoint(150,140),"F4");
    piano_keys['T'] = PianoKey(QPoint(185,140),"G4");    piano_keys['Y'] = PianoKey(QPoint(225,140),"A4");
    piano_keys['U'] = PianoKey(QPoint(260,140),"B4");   piano_keys['I'] = PianoKey(QPoint(285,140),"G4");
    /*
    //C5
    piano_keys['O'] = PianoKey(QPoint(335,140);    piano_keys['P'] = PianoKey(QPoint(365,140);
    piano_keys['['] = PianoKey(QPoint(407,140);    piano_keys[']'] = PianoKey(QPoint(439,140);
    piano_keys['A'] = PianoKey(QPoint(480,140);    piano_keys['S'] = PianoKey(QPoint(520,140);
    piano_keys['D'] = PianoKey(QPoint(550,140);    piano_keys['F'] = PianoKey(QPoint(580,140);

    piano_keys['1'] = PianoKey(QPoint(58,73);     piano_keys['2'] = PianoKey(QPoint(100,77);
    piano_keys['3'] = PianoKey(QPoint(170,72);    piano_keys['4'] = PianoKey(QPoint(205,77);
    piano_keys['5'] = PianoKey(QPoint(243,75);    piano_keys['6'] = PianoKey(QPoint(315,77);
    piano_keys['7'] = PianoKey(QPoint(354,71);    piano_keys['8'] = PianoKey(QPoint(421,79);
    */


//    //C6
//    key2pixel['F'] = 64;    key2pixel['G'] = 65;
//    key2pixel['H'] = 67;    key2pixel['J'] = 69;

}


void PianoWidget::mousePressEvent(QMouseEvent* event)
{
    fprintf(stderr,"pixel %d %d\n", event->pos().x(), event->pos().y());
    for(auto key=piano_keys.begin(); key!=piano_keys.end(); key++)
       (*key).second.on = false;
    for(auto key=piano_keys.begin(); key!=piano_keys.end(); key++)
    {
        QPoint pixel = (*key).second.pixel;
        QPoint diff = event->pos() - pixel;
        float r = sqrt(diff.x()*diff.x() + diff.y()*diff.y());
        //fprintf(stderr,"pixel r = %f\n", r);
        if( r < 20 )
        {
            (*key).second.on = true;
            emit sigMouseKey((*key).first);
        }

    }
    repaint();
    // start position 40 120
}

void PianoWidget::mouseReleaseEvent(QMouseEvent* event)
{
    for(auto key=piano_keys.begin(); key!=piano_keys.end(); key++)
       (*key).second.on = false;
     repaint();
}

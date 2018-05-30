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
    SvgWidget::paintEvent(event);

    QPainter painter(this);
    drawKeys(painter);


}

void PianoWidget::drawKeys(QPainter& painter)
{
    for(auto key=piano_keys.begin(); key!=piano_keys.end(); key++)
    {
        QPoint pixel = (*key).second.pixel;
        pixel.setY(pixel.y()+100);
        if( (*key).second.on )
        {
            QRectF rect = mapToViewbox((*key).second.rect, viewbox);
            painter.fillRect(rect,qRgba(255,0,0,40));
            QRectF rect2 = mapToViewbox((*key).second.rect2, viewbox);
            painter.fillRect(rect2,Qt::black);
            QRectF rect3 = mapToViewbox((*key).second.rect3, viewbox);
            painter.fillRect(rect3,Qt::black);

        }
    }
}

void PianoWidget::on_key_press(int key_code)
{
    if( piano_keys.find(key_code)==piano_keys.end() )
        return;
    piano_keys[key_code].on = true;
    repaint();
}

void PianoWidget::on_key_release(int key_code)
{
   if( piano_keys.find(key_code)==piano_keys.end() )
       return;
   piano_keys[key_code].on = false;
   repaint();
}

void PianoWidget::create_table()
{
    //C4
    piano_keys['Q'] = PianoKey("C4", getRect("C4"),getRect("Db4"));
    piano_keys['W'] = PianoKey("D4", getRect("D4"),getRect("Eb4"),getRect("Db4"));
    piano_keys['E'] = PianoKey("E4", getRect("E4"),getRect("Eb4"));
    piano_keys['R'] = PianoKey("F4", getRect("F4"),getRect("Gb4"));
    piano_keys['T'] = PianoKey("G4", getRect("G4"),getRect("Gb4"),getRect("Ab4"));
    piano_keys['Y'] = PianoKey("A4", getRect("A4"),getRect("Ab4"),getRect("Bb4"));
    piano_keys['U'] = PianoKey("B4", getRect("B4"),getRect("Bb4"));
    piano_keys['I'] = PianoKey("C5", getRect("C5"));
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

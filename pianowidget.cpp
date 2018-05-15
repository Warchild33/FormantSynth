#include <QPainter>
#include <QMouseEvent>
#include <QImage>
#include <math.h>
#include "pianowidget.h"

PianoWidget::PianoWidget(QWidget *parent) : QWidget(parent)
{
    background = QImage("./images/piano_keys_and_notes.png");
    create_table();
}

void PianoWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(),Qt::red);
    painter.drawPixmap(this->rect(),QPixmap::fromImage(background));
    drawKeys(painter);
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
    piano_keys['Q'] = QPoint(40,140);    piano_keys['W'] = QPoint(75,140);
    piano_keys['E'] = QPoint(115,140);    piano_keys['R'] = QPoint(150,140);
    piano_keys['T'] = QPoint(185,140);    piano_keys['Y'] = QPoint(225,140);
    piano_keys['U'] = QPoint(260,140);    piano_keys['I'] = QPoint(285,140);
    //C5
    piano_keys['O'] = QPoint(335,140);    piano_keys['P'] = QPoint(365,140);
    piano_keys['['] = QPoint(407,140);    piano_keys[']'] = QPoint(439,140);
    piano_keys['A'] = QPoint(480,140);    piano_keys['S'] = QPoint(520,140);
    piano_keys['D'] = QPoint(550,140);    piano_keys['F'] = QPoint(580,140);

    piano_keys['1'] = QPoint(58,73);     piano_keys['2'] = QPoint(100,77);
    piano_keys['3'] = QPoint(170,72);    piano_keys['4'] = QPoint(205,77);
    piano_keys['5'] = QPoint(243,75);    piano_keys['6'] = QPoint(315,77);
    piano_keys['7'] = QPoint(354,71);    piano_keys['8'] = QPoint(421,79);


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

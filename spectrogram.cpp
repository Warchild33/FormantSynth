#include <QLabel>
#include <QResizeEvent>
#include <QApplication>
#include <QPainter>
#include <QDoubleSpinBox>
#include <QDesktopWidget>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "spectrogram.h"
#include "stft.h"

using namespace cv;

double mag(complex_double d);
//void Mat2Image(QImage& img, Mat& src, int nChannels);
void IplImage2qImage(QImage& img, IplImage* src);

Spectrogram::Spectrogram(complex_double** X2, double stime, double fmax, int rows, int cols)
    :mX2(X2),mstime(stime),mfmax(fmax),mrows(rows),mcols(cols)
{
    porog = new QDoubleSpinBox(this);
    porog->setMaximum(100000);
    porog->setValue(255);
    porog->setMinimum(0);
    porog->show();
    connect(porog,SIGNAL(valueChanged(double)),this,SLOT(porogChanged(double)));
    QRect rec = QApplication::desktop()->screenGeometry();
    int d_height = rec.height();
    int d_width = rec.width();
    sg_width = 600;
    sg_height = 300;
    setGeometry(d_width/2 - sg_width/2,d_height/2-sg_height/2,sg_width,sg_height);
    show();
    //Build(X2,stime,fmax,rows,cols);
}

void Spectrogram::Build(complex_double** X2, double stime, double fmax, int rows, int cols, double porog)
{

    //qDebug("r=%d, g=%d b=%d",hsv(100,12,12).red(),hsv(100,12,12).green(),hsv(100,12,12).blue());
    //return;

    //spectrogram

    Mat src(sg_height, sg_width, CV_8U);
    double BinTimePix = (double)(sg_width-1) / cols;
    double FreqSize = (double)(sg_height-1) / rows;
    int x,y;
    for(int i=0; i < rows; i++)
    for(int j=0; j < cols; j++)
    {
        x = int((double)BinTimePix * j);
        y = sg_height - int((double)FreqSize*i) - 1;
        unsigned int col = unsigned int(mag(X2[i][j])*porog);
        if(col>255) col = 255;
        QColor c;
        //c.setHsv(col,50,50,255);
        c.setRgb(2,col,222,255);
        //image.setPixel(x,y,col);
        //*src.ptr(y,x) = col;
        circle(src, Point(x,y), 5, col, -1);
        //qDebug("x=%d y=%d",x,y);
    }

    Mat gauss;
    medianBlur(src, gauss, 7);
    QImage image(sg_width, sg_height, QImage::Format_RGB32);
    image.fill(Qt::black);
    IplImage out(gauss);
    IplImage2qImage(image, &out);
    //imwrite("spectrogram.bmp", src);




    //x axis
    QPainter p(&image);
    int nt = sg_width / 100;
    double cenat = stime / nt;
    p.setPen( Qt::white );
    p.drawLine(QPoint(0,sg_height-10),QPoint(sg_width,sg_height-10));
    for(int i=1; i < nt; i++)
    {
      p.drawText(QPoint(i*100-20, sg_height-20),QString::number(i*cenat));
      p.drawLine(QPoint(i*100, sg_height-15),QPoint(i*100, sg_height-5));
    }


    //y axis
    nt = sg_height / 100;
    cenat = fmax / nt;
    p.drawLine(QPoint(5,sg_height-10),QPoint(5,0));
    for(int i=1; i < nt; i++)
    {
      p.drawText(QPoint(15,sg_height - i*100+5),QString::number(i*cenat));
      p.drawLine(QPoint(0,sg_height - i*100),QPoint(10,sg_height - i*100));
    }


    setPixmap(QPixmap::fromImage(image));
}

void Spectrogram::resizeEvent(QResizeEvent* e)
{
    //qDebug("resize");
    sg_width = width();
    sg_height = height();
    Build(mX2,mstime,mfmax,mrows,mcols,porog->value());
}

void Spectrogram::porogChanged(double p)
{
    //qDebug("%f",porog);
    Build(mX2,mstime,mfmax,mrows,mcols,porog->value());
}


void IplImage2qImage(QImage& img, IplImage* src)
{
    int cnt=0;
    char* pix = src->imageData;

    if( src->nChannels == 3)
    {
        for(int k=0; k<src->width * src->height; k++ )
        {
               int i = cnt % img.width();
               int j = cnt / img.width();
               uint col = ((pix[2]<<16) + (pix[1]<<8) + pix[0]);
               img.setPixel(i, j, col);
               pix+=src->nChannels;
               cnt++;
        }
    }

    if( src->nChannels == 1)
    {
        for(int k=0; k<src->width * src->height; k++ )
        {
               int i = cnt % img.width();
               int j = cnt / img.width();
               uchar col = pix[0];
               img.setPixel(i, j, col);
               pix+=src->nChannels;
               cnt++;
        }
    }

}


#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_series_data.h>
#include <QMutex>
//#include <math.h>
#include "ploter.h"
extern "C" void rprintf(char* fmt,...);
complex_float bss[1200][16], bsr[1200][16];

QMutex datamutex;

Ploter* Ploter::instance;


Ploter*   Ploter::getInstance()
{
    return instance;
}

Ploter::Ploter(QWidget* parent)
//    :QwtPlot(parent)
{
    instance = this;
    set_mode(IMAG);
    plot = new QwtPlot(parent);
    //plot->setAxisScale(QwtPlot::xBottom, 0, 1000, 0);
    //plot->setAxisScale(QwtPlot::yLeft, -5000, 5000, 0);
    plot->setAxisAutoScale(QwtPlot::xBottom, true);
    plot->setAxisAutoScale(QwtPlot::yLeft, true);
    // названия графика и осей
    //plot->setTitle();
    plot->setFont(QFont("Arial", 8, QFont::Bold));
    plot->setAxisTitle(QwtPlot::xBottom, "");
    plot->setAxisFont(QwtPlot::xBottom, QFont("Arial", 8, QFont::Bold));
    plot->setAxisFont(QwtPlot::yLeft, QFont("Arial", 8, QFont::Bold));
    plot->setAutoReplot(true);
    //plot->set
    //plot->setAxisTitle(QwtPlot::yLeft, tr("Значения"));

    //plot->setAutoLegend(TRUE);

    pal = new QColor[16];
    // палитра графика    
    pal[0] = Qt::red;
    pal[1] = Qt::blue;
    pal[2] = Qt::green;
    pal[3] = Qt::magenta;
    pal[4] = Qt::lightGray;
    pal[5] = Qt::yellow;
    pal[6] = QColor(188,143,143);
    pal[7] = Qt::darkMagenta;
    pal[8] = Qt::darkCyan;
    pal[9] = Qt::darkGreen;
    pal[10] = Qt::gray;
    pal[11] = Qt::darkYellow;
    pal[12] = Qt::darkGray;
    pal[13] = Qt::blue;
    pal[14] = Qt::black;
    pal[15] = Qt::darkMagenta;
    
    // добавить 16 кривых    

    for(int i=0; i<16; i++)
    {
        //if((i!=4) && (i!=15))
        //  pal[i] = pal[i].lighter(150);
        //d_data[i] = new SignalData();
        d_curves[i]= new QwtPlotCurve(QString("Curve %1").arg(i));
        d_curves[i]->setStyle(QwtPlotCurve::Steps);
        d_curves[i]->setPen(QPen(pal[i]));


        //d_curves[i]->setData((QwtSeriesData<QPointF>*)d_data[i]);


        d_curves[i]->attach(plot);

        //d_curve->setData( new CurveData() );
        //curve[i] = plot->//insertCurve(QString("Curve %1").arg(i));
        // вкл. интерполяцию
        //plot->setCurveStyle(curve[i], QwtPlotCurve::Spline, QwtPlotCurve::Auto );
        //задать цвета
        //plot->setCurvePen(curve[i], QPen(pal[i]));

    }

    // вставить маркеры 
    //long mY = plot->insertLineMarker("y = 0", QwtPlot::yLeft);
    //plot->setMarkerYPos(mY, 0.0);
    //long mX = plot->insertLineMarker("x = 2 pi", QwtPlot::xBottom);
    //plot->setMarkerXPos(mX, 6.284);
    d_curves[1]->setStyle(QwtPlotCurve::Dots);


}


// вкл интерполяцию
void Ploter::set_interpolation(bool flag)
{
    interpolate = flag;
    const QwtPlotItemList& itmList = plot->itemList();
    if(flag)
    {
          for ( QwtPlotItemIterator it = itmList.begin();
               it != itmList.end(); ++it )
           {
               if ( (*it)->rtti() == QwtPlotItem::Rtti_PlotCurve )
               {
                   QwtPlotCurve *c = (QwtPlotCurve*)(*it);
                   c->setStyle(QwtPlotCurve::Steps);   // вкл. интерполяцию

               }
           }

    }
    else
    {
        for ( QwtPlotItemIterator it = itmList.begin();
             it != itmList.end(); ++it )
         {
             if ( (*it)->rtti() == QwtPlotItem::Rtti_PlotCurve )
             {
                 QwtPlotCurve *c = (QwtPlotCurve*)(*it);
                 c->setStyle(QwtPlotCurve::Lines);   // выкл. интерполяцию

             }
         }
    }
    plot->replot();
}

// автомасштаюирование
void Ploter::set_autoscale(bool flag)
{
    autoscale = flag;
    if(flag)
    {
        plot->setAxisAutoScale(QwtPlot::xBottom);
        plot->setAxisAutoScale(QwtPlot::yLeft);
    }
    else
    {
        plot->setAxisScale(QwtPlot::xBottom, 0, 1000, 0);
        plot->setAxisScale(QwtPlot::yLeft, -5000, 5000, 0);
    }
    plot->replot();
}

void Ploter::setXY(int n, double px, double py)
{
   if(n>16) return;
   if(n<0) return;   
   vx[n].append(px);
   vy[n].append(py);
}


void Ploter::setXYonly(int n, double px, double py)
{
   if(n>16) return;
   if(n<0) return;
   vx[n].append(px);
   vy[n].append(py);
}

void Ploter::clearvals(int n)
{
    if(n>16) return;
    if(n<0) return;
     vx[n].clear();
     vy[n].clear();
    //d_data[n]->clearStaleValues(0);
	d_curves[n]->setSamples(vx[n],vy[n]);
}


// генерит случайный сигнал
void Ploter::rand_test()
{
    //for(int n=1; n<10; n++)
    {
        clearvals(0);
        for(int i=0; i<1000; i++)
        {
           x[i] = i;
           y1[i] = ((double)5 * rand()) / RAND_MAX;
           setXY(0, x[i], y1[i]);
        }
        //plot->setCurveData(curve[n], x, y1, 1000);
        //d_curves[0]->setRawSamples();
    }
    //d_curves[0]->setSamples(x,y1,1000);
    //d_curves[1]->setSamples(x,y1,1000);
    plot->replot();
}

//увеличивает уровень сигнала 
void Ploter::magnitude(double level)
{
    for(int i=0; i<1000; i++)
    {
       x[i] = i;
       y1[i] = bss[i][1].re * level;
       y2[i] = cos((double)i);
    }
    //plot->setCurveData(curve[1], x, y1, 1000);
    plot->replot();    
    printf("%f\n", level);
}

//устанавливает название
void Ploter::title(char* name)
{
    plot->setTitle(name);
}




// Отображение комплексного сигнала
// mode - режим отображения (re, im, abs)
// n - количество отсчетов
void Ploter::out_complex(complex_float** s_in, int Ndd, int j)
{
    int n;

    //set_curves_count(1);

    if(Ndd > MAX_POINTS)
    {
        printf("Ploter::out_complex: too many discrets!\n");
        return;
    }

    switch(mode)
    {
        case IMAG:
            for(n = 0; n < Ndd; n++)
            {
                 x[n] = n;    
                 y1[n] = s_in[n][j].im;

            }
            //plot->setCurveData(curve[0], x, y1, Ndd);
        break;

       case REAL:
            for(n = 0; n < Ndd; n++)
            {
                 x[n] = n;
                 y1[n] = s_in[n][j].re;

            }
            //plot->setCurveData(curve[0], x, y1, Ndd);
            break;



    }
    plot->replot();

}


void Ploter::out_complex2(complex_float*** s_in, int Nl, int Ndd, int j)
{
    int n, l;

    //set_curves_count(16);

    switch(mode)
    {
        case IMAG:
        for( l = 0; l < Nl; l++)    
        {
            for( n = 0; n < Ndd; n++ )
            {
               x[n] = n;
               y1[n] = s_in[l][n][j].im;
            }
            //plot->setCurveData(curve[l], x, y1, 1000);
        }
        break;

       case REAL:
            for( l = 0; l < Nl; l++)    
            {
                for( n = 0; n < Ndd; n++ )
                {
                   x[n] = n;
                   y1[n] = s_in[l][n][j].re;
                }
              //  plot->setCurveData(curve[l], x, y1, 1000);
            }
       break;



    }
    plot->replot();

}
void Ploter::update_data()
{
    for(int n=0; n<16; n++)
      d_curves[n]->setSamples(vx[n],vy[n]);

}

void Ploter::set_curves_count(int N)
{
    N=N;
  /*  plot->removeCurves();
    for(int i = 0; i < N; i++)
    {
        curve[i] = plot->insertCurve(QString("Curve %1").arg(i));
        plot->setCurvePen(curve[i], QPen(pal[i]));
    }
    */
}

void Ploter::reset()
{
    for(int n=0; n<16; n++)
        if( (vx[n].size()>0) && (vy[n].size()>0))
        {
            vx[n].clear();
            vy[n].clear();
            d_curves[n]->setSamples(vx[n],vy[n]);
        }
    /*plot->removeCurves();
    for(int i = 0; i < 16; i++)
    {
        curve[i] = plot->insertCurve(QString("Curve %1").arg(i));
        plot->setCurvePen(curve[i], QPen(pal[i]));
    }
    */
}



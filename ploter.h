#ifndef PLOTER_H
#define PLOTER_H

#include <qframe.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_series_data.h>

typedef struct complex_float
{
    float re;
    float im;
}complex_float;



class Ploter //: public QwtPlot
{

public:    
    int mode;
    bool autoscale;
    bool interpolate;
    QwtPlot* plot;

    enum Mode { IMAG,  REAL,  ABS };

    QColor* pal;
    #define MAX_POINTS 2000
    double x[MAX_POINTS], y1[MAX_POINTS], y2[MAX_POINTS];        // x and y values    
    long curve[16];
    QwtPlotCurve* d_curves[16];
    //SignalData* d_data[16];
    QVector< double > vx[16];
    QVector< double > vy[16];

    static Ploter*         instance;
    static Ploter*   getInstance();

    Ploter(QWidget* parent);

    void setXY(int n, double px, double py);
    void setXYonly(int n, double px, double py);
    void clearvals(int n);
    void set_mode(int m) { mode = m; }
    void set_curves_count(int N);
    void set_interpolation(bool flag);
    void set_autoscale(bool flag);
    void rand_test();
    void update_data();
    void reset();
    void magnitude(double level);
    void title(char* name);
    void out_complex(complex_float** s_in, int Ndd, int j);
    void out_complex2(complex_float*** s_in, int Ndd, int Nl, int j);
};


#endif

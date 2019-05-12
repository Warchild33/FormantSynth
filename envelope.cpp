#include "envelope.h"
#include <math.h>
#define M_PI 3.14159265

Pnt spline_kochaken(Pnt P1, Pnt P2, Pnt P3, Pnt P4, float t,
           double tension, double bias, double continuity)
{
    Pnt R1, R2;
    float tt = tension; // tension pts[i]->kp.x
    float b = bias; // bias  pts[i]->kp.y
    float c = continuity; // continuity	pts[i]->kp.z

    R1.x = ((1-tt)*(1+b)*(1+c)/2) * (P2.x - P1.x) + ((1-tt)*(1-b)*(1-c)/2) * (P3.x - P2.x);
    R1.z = ((1-tt)*(1+b)*(1+c)/2) * (P2.z - P1.z) + ((1-tt)*(1-b)*(1-c)/2) * (P3.z - P2.z);

    R2.x = ((1-tt)*(1+b)*(1-c)/2) * (P3.x - P2.x) + ((1-tt)*(1-b)*(1+c)/2) * (P4.x - P3.x);
    R2.z = ((1-tt)*(1+b)*(1-c)/2) * (P3.z - P2.z) + ((1-tt)*(1-b)*(1+c)/2) * (P4.z - P3.z);

    Pnt sp;
    sp.x = P1.x * (2*t*t*t-3*t*t+1) + P2.x * (-2*t*t*t+3*t*t) + R1.x * (t*t*t-2*t*t+t) + R2.x * (t*t*t-t*t);
    sp.z = P1.z * (2*t*t*t-3*t*t+1) + P2.z * (-2*t*t*t+3*t*t) + R1.z * (t*t*t-2*t*t+t) + R2.z * (t*t*t-t*t);

    return sp;
}
//Cardinal quadratic B-spline with knot vector (0, 0, 0, 1, 2, 3, 3, 3) and control points (0, 0, 1, 0, 0), and its first derivative
/*
    Evaluates S(x).
    Args
    ----
    k: index of knot interval that contains x
    x: position
    t: array of knot positions, needs to be padded as described above
    c: array of control points
    p: degree of B-spline
*/
double deBoor(int k, double x, double* t, double* c, int p)
{
    double d[3];

    for(int j=0; j < p+1; j++)
        d[j] = c[j + k - p];

    for(int r=1; r < p+1; r++)
       for(int j=p; j > r-1; j--)
        {
            double alpha = (x - t[j+k-p]) / (t[j+1+k-r] - t[j+k-p]);
            d[j] = (1.0 - alpha) * d[j-1] + alpha * d[j];
        }
    return d[p];
}

void envelope_Bspline(double* x, double Amax, double SampleRate, double time)
{
    int N = floor(time*SampleRate);
    double t[] = {0, 0, 0, 1, 2, 3, 3, 3};
    double c[] = {0, 0, 1, 0, 0};

    t[4] = N;    t[5] = N+1;
    t[6] = N+1;  t[7] = N+1;

    x[0] = 0;
    for(int i=1; i < N+1; i++ )
    {
        double A = Amax * deBoor(3, i, t, c, 2);
        x[i] = x[i] * A;
    }
}

void envelope_cos(double* x, double Amax, double SampleRate, double time)
{
    int N = floor(time*SampleRate);
    for(int i=0; i < N; i++ )
    {
        double A = Amax * cos(M_PI * i / N + M_PI/2 );
        x[i] = x[i] * A;
    }
}


void normalize(double level, double* x, double Amax, double SampleRate, double time)
{
    int N = floor(time*SampleRate);
    for(int i=0; i < N; i++ )
        x[i] = (x[i]/Amax)*level;
}





#ifndef ENVELOPE_H
#define ENVELOPE_H

class Pnt
{
public:
    Pnt(){x=0;y=0;z=0;}
    Pnt(const Pnt& B) { x = B.x; y = B.y; z = B.z; }
    Pnt(float _x, float _y, float _z) :
    x(_x), y(_y), z(_z) {}
    Pnt* operator->(){return this;}
    Pnt operator*(float s){return Pnt(x*s,y*s,z*s);}
    Pnt operator*(Pnt& B){return Pnt(B.x*x,B.y*y,B.z*z);}
    //Pnt operator*(double B){return Pnt(B*x,B*y,B*z);}
    Pnt operator+(Pnt p){return Pnt(x+p.x,y+p.y,z+p.z);}
    Pnt operator-(Pnt p){return Pnt(x-p.x,y-p.y,z-p.z);}
    bool operator==(Pnt p){return (x == p.x) && (y == p.y) && (z == p.z); }

    bool inRangeOfPnt(Pnt B, float R)
    {
        return( (x+R) > B.x )&&( (y+R) > B.y ) && ((x-R) < B.x )&&( (y-R) < B.y );
    }

    bool isNull()
    {
        return x==0 && y==0 && z==0;
    }


    float x,y,z;
};

Pnt spline_kochaken(Pnt P1, Pnt P2, Pnt P3, Pnt P4, float t,
           double tension, double bias, double continuity);

double deBoor(int k, double x, double* t, double* c, int p);
void envelope_cos(double* x, double Amax, double SampleRate, double time);
void envelope_Bspline(double* x, double Amax, double SampleRate, double time);
void normalize(double level, double* x, double Amax, double SampleRate, double time);

#endif // ENVELOPE_H

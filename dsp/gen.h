#ifndef GEN_H
#define GEN_H
    double triangle(double f_oc=800, double SampleRate=48000, double t=0, double phase0=0);
    double* pulse_gen1(double f_oc=800, double SampleRate=48000, double time=2, int* N=0);
    double* pulse_nes(double f_oc=800, double percent=0.75, double SampleRate=48000, double time=2, int* N=0);
    double* noise_nes(double f_oc=800, double percent=0.75, double SampleRate=48000, double time=2, int* N=0);
    double* noise_nes1(double f_oc=800, double percent=0.75, double SampleRate=48000, double time=2, int* N=0);
    double* tri_nes(double f_oc=800, double percent=0.75, double SampleRate=48000, double time=2, int* N=0);
    double* silence(double SampleRate=48000, double time=2, int* N=0);
    double* test_fm(double f_oc=800, double ratio=0.75, double SampleRate=48000, double time=2, int* N=0);
#endif // GEN_H

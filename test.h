#ifndef TEST_H
#define TEST_H

class Test
{
public:
    Test();
    void startup_tests();
    void test_spline_kochaken();
    void test_deBoor();
    void test_nes_gen();
    void test_gen_voice(double f_ot, double F1, double F2, double F3, double BW, int Ncascade);
};

#endif // TEST_H

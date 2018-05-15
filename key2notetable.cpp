#include "key2notetable.h"

Key2NoteTable::Key2NoteTable()
{
    create();
}

void Key2NoteTable::create()
{
//    for(char i=0; i<127; i++)
//    {
//        key2note[i] = i;
//        key2note[2*i] = i;
//    }
    //C4
    key2note['Q'] = 36;    key2note['W'] = 38;
    key2note['E'] = 40;    key2note['R'] = 41;
    key2note['T'] = 43;    key2note['Y'] = 45;
    key2note['U'] = 47;    key2note['I'] = 48;
    //C5
    key2note['O'] = 50;    key2note['P'] = 52;
    key2note['['] = 53;    key2note[']'] = 55;
    key2note['A'] = 57;    key2note['S'] = 59;
    key2note['D'] = 60;    key2note['F'] = 62;
    //C6
    key2note['G'] = 64;    key2note['H'] = 65;
    key2note['J'] = 67;    key2note['K'] = 69;
    key2note['L'] = 71;    key2note[';'] = 72;
    key2note['\''] = 74;    key2note['\\'] = 76;
    //key2note['\''] = 77;    key2note['\\'] = 79;

    // black keys
    key2note['1'] = 37;    key2note['2'] = 39;
    key2note['3'] = 42;    key2note['4'] = 44;
    key2note['5'] = 46;    key2note['6'] = 49;
    key2note['7'] = 51;    key2note['8'] = 54;
    key2note['9'] = 58;    key2note['0'] = 61;

}

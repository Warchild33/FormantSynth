#ifndef KEY2NOTETABLE_H
#define KEY2NOTETABLE_H

#include <map>

class Key2NoteTable
{
public:
    Key2NoteTable();
    std::map<int, char>  key2note;
    void create();
    char operator[](int key) { return key2note[key]; }
};

#endif // KEY2NOTETABLE_H

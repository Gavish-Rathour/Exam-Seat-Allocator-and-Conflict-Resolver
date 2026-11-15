#ifndef PERSON_H
#define PERSON_H
#include <iostream>
#include <string>
using namespace std;

class Person
{
private:
    string name;
public:
    Person();
    Person(string n);
    virtual void display();
    string getName();
    void setName(string n);
    virtual ~Person();
};
#endif

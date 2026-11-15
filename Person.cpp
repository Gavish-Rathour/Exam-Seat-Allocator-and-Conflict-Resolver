#include "Person.h"
Person::Person() { name = ""; }
Person::Person(string n) { name = n; }
void Person::display() { cout << "Name: " << name << endl; }
string Person::getName() { return name; }
void Person::setName(string n) { name = n; }
Person::~Person() {}

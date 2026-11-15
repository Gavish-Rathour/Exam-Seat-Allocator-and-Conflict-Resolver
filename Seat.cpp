#include "Seat.h"
Seat::Seat() { row = 0; column = 0; student = nullptr; }
Seat::Seat(int r, int c) { row = r; column = c; student = nullptr; }
int Seat::getRow() { return row; }
int Seat::getColumn() { return column; }
Student* Seat::getStudent() { return student; }
void Seat::assignStudent(Student *s) { student = s; }
void Seat::clearSeat() { student = nullptr; }

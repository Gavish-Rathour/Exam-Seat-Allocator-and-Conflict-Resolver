#ifndef SEAT_H
#define SEAT_H
#include "../StudentManagement/Student.h"
class Seat
{
private:
    int row, column;
    Student *student;
public:
    Seat();
    Seat(int r, int c);
    int getRow();
    int getColumn();
    Student* getStudent();
    void assignStudent(Student *s);
    void clearSeat();
};
#endif

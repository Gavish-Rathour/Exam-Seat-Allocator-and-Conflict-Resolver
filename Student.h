#ifndef STUDENT_H
#define STUDENT_H
#include "Person.h"

class Student : public Person
{
private:
    string rollNo;
    int id;
    string section;
    string subject;
    string course;
public:
    Student();
    Student(string n, string roll, int studentId, string sec, string sub, string c);
    void display() override;
    string getRollNo();
    int getId();
    string getSection();
    string getSubject();
    string getCourse();
    void setRollNo(string r);
    void setId(int value);
    void setSection(string sec);
    void setSubject(string sub);
    void setCourse(string c);
};
#endif

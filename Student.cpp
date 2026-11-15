#include "Student.h"
Student::Student() : Person()
{
    rollNo = ""; id = 0; section = ""; subject = ""; course = "";
}
Student::Student(string n, string roll, int studentId, string sec, string sub, string c)
    : Person(n)
{
    rollNo = roll; id = studentId; section = sec; subject = sub; course = c;
}
void Student::display()
{
    cout << "Name: " << getName() << " | Roll No: " << rollNo
         << " | ID: " << id << " | Section: " << section
         << " | Subject: " << subject << " | Course: " << course << endl;
}
string Student::getRollNo() { return rollNo; }
int Student::getId() { return id; }
string Student::getSection() { return section; }
string Student::getSubject() { return subject; }
string Student::getCourse() { return course; }
void Student::setRollNo(string r) { rollNo = r; }
void Student::setId(int value) { id = value; }
void Student::setSection(string sec) { section = sec; }
void Student::setSubject(string sub) { subject = sub; }
void Student::setCourse(string c) { course = c; }

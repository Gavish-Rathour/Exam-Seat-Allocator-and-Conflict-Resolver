#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H
#include "Student.h"
#include "Config.h"

class StudentManager
{
private:
    Student students[MAX_STUDENTS];
    int count;
public:
    StudentManager();
    bool loadFromFile(string fileName);
    int getCount();
    Student* getStudents();
    void displayAll();
    int findStudent(string rollNo);
    int findStudentByName(string name);
    void addStudent();
    void updateStudent();
    void deleteStudent();
};
#endif

#include "StudentManager.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>

static bool isValidCourseSubject(string course, string subject)
{
    const string btech[] = {
        "Data Structures and Algorithms", "Database Management Systems", "Operating Systems",
        "Computer Networks", "Object Oriented Programming", "Software Engineering",
        "Computer Organization and Architecture", "Theory of Computation",
        "Design and Analysis of Algorithms", "Artificial Intelligence",
        "Mechanics", "Drawing", "Math", "Physics", "Chemistry", "English",
        "Programming", "Electronics"
    };
    const string bba[] = {
        "Principles of Management", "Financial Accounting", "Business Economics",
        "Marketing Management", "Human Resource Management", "Business Communication",
        "Business Statistics", "Organizational Behaviour", "Business Law",
        "Entrepreneurship Development"
    };
    const string mba[] = {
        "Financial Management", "Marketing Management", "Human Resource Management",
        "Operations Management", "Business Analytics", "Strategic Management",
        "Managerial Economics", "Organizational Behaviour", "Business Research Methods",
        "Entrepreneurship and Innovation"
    };
    const string mtech[] = {
        "Advanced Data Structures and Algorithms", "Advanced Operating Systems",
        "Machine Learning", "Deep Learning", "Advanced Database Systems",
        "Cloud Computing", "Distributed Systems", "Artificial Intelligence",
        "Computer Vision", "Research Methodology"
    };
    if (course == "BTech" || course == "BTech CSE")
        for (int i = 0; i < 18; i++) if (btech[i] == subject) return true;
    if (course == "BBA")
        for (int i = 0; i < 10; i++) if (bba[i] == subject) return true;
    if (course == "MBA")
        for (int i = 0; i < 10; i++) if (mba[i] == subject) return true;
    if (course == "MTech" || course == "MTech CSE")
        for (int i = 0; i < 10; i++) if (mtech[i] == subject) return true;
    return false;
}

StudentManager::StudentManager() { count = 0; }

bool StudentManager::loadFromFile(string fileName)
{
    ifstream inputFile(fileName.c_str());
    if (!inputFile) return false;
    count = 0;
    string line;
    int lineNumber = 0, invalid = 0;
    while (getline(inputFile, line))
    {
        lineNumber++;
        if (line.empty()) continue;
        string fields[6];
        stringstream ss(line);
        bool valid = true;
        for (int i = 0; i < 6; i++)
        {
            if (!getline(ss, fields[i], '|')) { valid = false; break; }
        }
        string extra;
        if (getline(ss, extra, '|')) valid = false;
        for (int i = 0; i < 6; i++) if (fields[i].empty()) valid = false;
        if (!valid)
        {
            cout << "Invalid student record at line " << lineNumber << ". Expected 6 fields.\n";
            invalid++; continue;
        }
        bool duplicate = false;
        for (int i = 0; i < count; i++)
            if (students[i].getRollNo() == fields[1] || students[i].getId() == atoi(fields[2].c_str())) duplicate = true;
        char *endPtr = nullptr;
        long idValue = strtol(fields[2].c_str(), &endPtr, 10);
        if (endPtr == fields[2].c_str() || *endPtr != '\0' || idValue < 0) valid = false;
        if (!isValidCourseSubject(fields[5], fields[4])) valid = false;
        if (duplicate || !valid)
        {
            cout << "Invalid/duplicate roll number or ID at student file line " << lineNumber << ".\n";
            invalid++; continue;
        }
        if (count >= MAX_STUDENTS)
        {
            cout << "Maximum supported student capacity (" << MAX_STUDENTS << ") reached. Remaining records not loaded.\n";
            break;
        }
        students[count] = Student(fields[0], fields[1], (int)idValue, fields[3], fields[4], fields[5]);
        count++;
    }
    cout << "Student records loaded: " << count << " | Invalid records: " << invalid << endl;
    return true;
}

int StudentManager::getCount() { return count; }
Student* StudentManager::getStudents() { return students; }

void StudentManager::displayAll()
{
    if (count == 0) { cout << "No student records available.\n"; return; }
    for (int i = 0; i < count; i++) students[i].display();
    cout << "Total students: " << count << endl;
}

int StudentManager::findStudent(string rollNo)
{
    for (int i = 0; i < count; i++) if (students[i].getRollNo() == rollNo) return i;
    return -1;
}

int StudentManager::findStudentByName(string name)
{
    for (int i = 0; i < count; i++) if (students[i].getName() == name) return i;
    return -1;
}

void StudentManager::addStudent()
{
    if (count >= MAX_STUDENTS) { cout << "Student capacity is full.\n"; return; }
    string name, roll, section, subject, course;
    int id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter name: "; getline(cin, name);
    cout << "Enter roll number: "; getline(cin, roll);
    if (findStudent(roll) != -1) { cout << "Roll number already exists.\n"; return; }
    cout << "Enter numeric ID: "; if (!(cin >> id)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid ID.\n"; return; }
    for (int i = 0; i < count; i++) if (students[i].getId() == id) { cout << "This ID already exists.\n"; return; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter section: "; getline(cin, section);
    cout << "Enter subject: "; getline(cin, subject);
    cout << "Enter course: "; getline(cin, course);
    if (name.empty() || roll.empty() || section.empty() || subject.empty() || course.empty()) { cout << "Fields cannot be empty.\n"; return; }
    if (!isValidCourseSubject(course, subject)) { cout << "Invalid course-subject combination. Check README for supported subjects.\n"; return; }
    students[count++] = Student(name, roll, id, section, subject, course);
    cout << "Student added in current session only. Input file was not changed.\n";
}

void StudentManager::updateStudent()
{
    string roll;
    cout << "Enter roll number to update: "; cin >> roll;
    int index = findStudent(roll);
    if (index == -1) { cout << "Student not found.\n"; return; }
    string name, section, subject, course; int id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "New name: "; getline(cin, name);
    cout << "New numeric ID: "; if (!(cin >> id)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid ID.\n"; return; }
    for (int i = 0; i < count; i++) if (i != index && students[i].getId() == id) { cout << "This ID already exists.\n"; return; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "New section: "; getline(cin, section);
    cout << "New subject: "; getline(cin, subject);
    cout << "New course: "; getline(cin, course);
    if (name.empty() || section.empty() || subject.empty() || course.empty()) { cout << "Fields cannot be empty.\n"; return; }
    if (!isValidCourseSubject(course, subject)) { cout << "Invalid course-subject combination. Check README for supported subjects.\n"; return; }
    students[index].setName(name); students[index].setId(id); students[index].setSection(section);
    students[index].setSubject(subject); students[index].setCourse(course);
    cout << "Student updated in current session only.\n";
}

void StudentManager::deleteStudent()
{
    string roll; cout << "Enter roll number to delete: "; cin >> roll;
    int index = findStudent(roll);
    if (index == -1) { cout << "Student not found.\n"; return; }
    for (int i = index; i < count - 1; i++) students[i] = students[i + 1];
    count--; cout << "Student deleted from current session only.\n";
}

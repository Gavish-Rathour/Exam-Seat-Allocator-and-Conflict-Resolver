#include <iostream>
#include <limits>
#include <string>
#include "../StudentManagement/StudentManager.h"
#include "../RoomManagement/RoomManager.h"
#include "../SeatAllocation/Allocator.h"
using namespace std;

void studentMenu(StudentManager &students, bool &allocationDone)
{
    int choice;
    do
    {
        cout << "\n========== STUDENT MANAGEMENT ==========\n";
        cout << "1. Display students\n2. Add student\n3. Update student\n4. Delete student\n0. Back to main menu\nChoice: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Enter a number.\n"; continue; }
        if (choice == 1) students.displayAll();
        else if (choice == 2) { students.addStudent(); allocationDone = false; }
        else if (choice == 3) { students.updateStudent(); allocationDone = false; }
        else if (choice == 4) { students.deleteStudent(); allocationDone = false; }
        else if (choice != 0) cout << "Invalid choice.\n";
    } while (choice != 0);
}

void searchMenu(StudentManager &students, Allocator &allocator, RoomManager &rooms, bool allocationDone)
{
    int choice;
    cout << "\n1. Search by roll number\n2. Search by name\n3. Search by course\n4. Search by subject\nChoice: ";
    if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid input.\n"; return; }
    Student *data = students.getStudents();
    if (choice == 1)
    {
        string roll; cout << "Enter roll number: "; cin >> roll;
        int index = students.findStudent(roll);
        if (index == -1) { cout << "Student not found.\n"; return; }
        data[index].display();
        if (allocationDone)
        {
            int shift, row, col; string roomId;
            if (allocator.getLocationForStudent(index, rooms, shift, roomId, row, col))
                cout << "Shift: " << shift << " | Room: " << roomId << " | Row: " << row << " | Column: " << col << endl;
            else cout << "Student is unallocated.\n";
        }
        else cout << "Allocate seats to see the location.\n";
    }
    else if (choice == 2)
    {
        string name; cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Enter full name: "; getline(cin, name);
        bool found = false;
        for (int i = 0; i < students.getCount(); i++) if (data[i].getName().find(name) != string::npos)
        {
            found = true; data[i].display();
            if (allocationDone) { int sh,r,c; string room; if (allocator.getLocationForStudent(i,rooms,sh,room,r,c)) cout << "Shift "<<sh<<" | Room "<<room<<" | Row "<<r<<" | Column "<<c<<"\n"; else cout << "Unallocated.\n"; }
        }
        if (!found) cout << "No matching student found.\n";
    }
    else if (choice == 3 || choice == 4)
    {
        string value; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << (choice == 3 ? "Enter course: " : "Enter subject: "); getline(cin, value);
        if (allocationDone) allocator.searchGroup(value, choice == 3, students, rooms);
        else
        {
            bool found = false;
            for (int i = 0; i < students.getCount(); i++)
            {
                string field = choice == 3 ? data[i].getCourse() : data[i].getSubject();
                if (field == value) { data[i].display(); found = true; }
            }
            if (!found) cout << "No matching students found.\n";
            cout << "Seat locations will appear after allocation.\n";
        }
    }
    else cout << "Invalid choice.\n";
}


void askGroupSeating(StudentManager &students, Allocator &allocator, RoomManager &rooms)
{
    char choice;
    cout << "\nDo you want to view room-wise seating of a Subject or Course? (Y/N): ";
    cin >> choice;
    if (choice != 'Y' && choice != 'y') return;

    int type;
    cout << "1. Subject-wise\n2. Course-wise\n0. Back\nChoice: ";
    if (!(cin >> type)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid input.\n"; return; }
    if (type == 0) return;
    if (type != 1 && type != 2) { cout << "Invalid choice.\n"; return; }

    Student *data = students.getStudents();
    cout << "Available " << (type == 1 ? "subjects" : "courses") << ":\n";
    for (int i = 0; i < students.getCount(); i++)
    {
        string value = type == 1 ? data[i].getSubject() : data[i].getCourse();
        bool alreadyShown = false;
        for (int j = 0; j < i; j++)
        {
            string previous = type == 1 ? data[j].getSubject() : data[j].getCourse();
            if (previous == value) alreadyShown = true;
        }
        if (!alreadyShown) cout << "- " << value << "\n";
    }
    string value;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter exact " << (type == 1 ? "subject" : "course") << " name: ";
    getline(cin, value);
    allocator.displayGroupSeating(value, type == 2, students, rooms);
}

int main()
{
    StudentManager students;
    RoomManager rooms;
    Allocator allocator;
    bool allocationDone = false;

    cout << "========== EXAM SEAT ALLOCATOR V2 ==========\n";
    if (!students.loadFromFile("Database/student_records.txt"))
        cout << "ERROR: Database/student_records.txt not found.\n";
    if (!rooms.loadFromFile("Database/room_config.txt"))
        cout << "ERROR: Database/room_config.txt not found.\n";
    cout << "\n========== INPUT SUMMARY ==========\n";
    cout << "Students loaded: " << students.getCount() << endl;
    cout << "Rooms loaded: " << rooms.getRoomCount() << endl;
    cout << "Seats per shift: " << rooms.getTotalSeats() << endl;
    cout << "Select option 3 to start allocation and check the actual result.\n";

    int choice;
    while (true)
    {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Student Management\n2. Display Room Details\n3. Allocate Examination Seats\n4. Display Seating Arrangement\n5. Search Student\n6. Save/Export Results\n0. Exit\nChoice: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Please enter a valid number.\n"; continue; }
        if (choice == 1) studentMenu(students, allocationDone);
        else if (choice == 2) rooms.displayAllRooms();
        else if (choice == 3)
        {
            allocator.allocate(students, rooms);
            allocationDone = true;
            allocator.saveReport(students, rooms, "Results/seating_output.txt");
            cout << "\nDo you want to display the seating arrangement now? (Y/N): ";
            char show; cin >> show;
            if (show == 'Y' || show == 'y') { allocator.displayAllShifts(students, rooms); askGroupSeating(students, allocator, rooms); }
            allocator.displayUnallocated(students);
            cout << "\nDo you want to update student records? (Y/N): ";
            char update; cin >> update;
            if (update == 'Y' || update == 'y') { allocationDone = false; cout << "Returning to main menu. Choose Student Management to edit records.\n"; }
        }
        else if (choice == 4)
        {
            if (!allocationDone) cout << "Please allocate seats first.\n";
            else { allocator.displayAllShifts(students, rooms); askGroupSeating(students, allocator, rooms); }
        }
        else if (choice == 5) searchMenu(students, allocator, rooms, allocationDone);
        else if (choice == 6)
        {
            if (!allocationDone) cout << "Please allocate seats before exporting the report.\n";
            else allocator.saveReport(students, rooms, "Results/seating_output.txt");
        }
        else if (choice == 0) { cout << "Program ended.\n"; break; }
        else cout << "Invalid choice. Try again.\n";
    }
    return 0;
}

#include "Allocator.h"
#include <iostream>
#include <fstream>
#include <limits>

Allocator::Allocator()
{
    allocationCount = 0; allocatedCount = 0; unallocatedCount = 0; totalShifts = 0;
}

bool Allocator::hasConflict(Student *candidate, Room *room, int row, int column)
{
    
    if (column > 0)
    {
        Student *left = room->getSeat(row, column - 1)->getStudent();
        if (left && (left->getSubject() == candidate->getSubject() || left->getSection() == candidate->getSection())) return true;
    }
    if (row > 0)
    {
        Student *upper = room->getSeat(row - 1, column)->getStudent();
        if (upper && (upper->getSubject() == candidate->getSubject() || upper->getSection() == candidate->getSection())) return true;
    }
    return false;
}

bool Allocator::placeStudent(int studentIndex, Student *students, RoomManager &roomManager, int shift)
{
    Room *rooms = roomManager.getRooms();
    for (int r = 0; r < roomManager.getRoomCount(); r++)
        for (int row = 0; row < rooms[r].getRows(); row++)
            for (int col = 0; col < rooms[r].getColumns(); col++)
                if (rooms[r].getSeat(row, col)->getStudent() == nullptr && !hasConflict(&students[studentIndex], &rooms[r], row, col))
                {
                    rooms[r].getSeat(row, col)->assignStudent(&students[studentIndex]);
                    allocations[allocationCount].studentIndex = studentIndex;
                    allocations[allocationCount].shift = shift;
                    allocations[allocationCount].roomIndex = r;
                    allocations[allocationCount].row = row;
                    allocations[allocationCount].column = col;
                    allocationCount++;
                    return true;
                }
    return false;
}

void Allocator::allocate(StudentManager &studentManager, RoomManager &roomManager)
{
    allocationCount = 0; allocatedCount = 0; unallocatedCount = 0; totalShifts = 0;
    int count = studentManager.getCount();
    if (count == 0 || roomManager.getRoomCount() == 0) { cout << "Students or rooms are not available.\n"; return; }
    Student *students = studentManager.getStudents();
    bool assigned[MAX_STUDENTS] = {false};
    cout << "\nStarting seat allocation...\n";

    int remaining = count;
    bool repeatedNoProgress = false;
    while (remaining > 0 && totalShifts < 20)
    {
        totalShifts++;
        roomManager.clearAllRooms();
        int before = remaining;
        for (int i = 0; i < count; i++)
        {
            if (!assigned[i] && placeStudent(i, students, roomManager, totalShifts))
            {
                assigned[i] = true; remaining--; allocatedCount++;
            }
        }
        cout << "\nShift " << totalShifts << ": allocated " << (before - remaining) << " student(s).\n";
        if (remaining == 0) break;
        if (totalShifts >= 20) break;
        if (before == remaining)
        {
            cout << "No additional student could be placed in this shift because of the current seating constraints.\n";
            if (repeatedNoProgress)
            {
                cout << "A further identical shift also made no progress, so allocation has stopped.\n";
                break;
            }
            repeatedNoProgress = true;
        }
        else repeatedNoProgress = false;
        char choice;
        cout << "All students could not be accommodated. Would you like to add an additional shift? (Y/N): ";
        cin >> choice;
        if (choice != 'Y' && choice != 'y') break;
    }
    unallocatedCount = count - allocatedCount;
    cout << "\nAllocation summary: " << allocatedCount << " allocated, " << unallocatedCount << " unallocated across " << totalShifts << " shift(s).\n";
}

void Allocator::displayShift(int shift, StudentManager &studentManager, RoomManager &roomManager)
{
    roomManager.clearAllRooms();
    Student *students = studentManager.getStudents();
    for (int i = 0; i < allocationCount; i++) if (allocations[i].shift == shift)
    {
        Room *room = roomManager.getRoomByIndex(allocations[i].roomIndex);
        if (room) room->getSeat(allocations[i].row, allocations[i].column)->assignStudent(&students[allocations[i].studentIndex]);
    }
    cout << "\n********** SHIFT " << shift << " **********\n";
    roomManager.displayAllRooms();
}

void Allocator::displayAllShifts(StudentManager &students, RoomManager &rooms)
{
    if (allocationCount == 0) { cout << "No allocation available. Allocate seats first.\n"; return; }
    for (int s = 1; s <= totalShifts; s++) displayShift(s, students, rooms);
    rooms.clearAllRooms();
}

void Allocator::displayUnallocated(StudentManager &studentManager)
{
    bool found = false; Student *students = studentManager.getStudents();
    cout << "\nUNALLOCATED STUDENTS\n";
    for (int i = 0; i < studentManager.getCount(); i++)
    {
        bool allocated = false;
        for (int j = 0; j < allocationCount; j++) if (allocations[j].studentIndex == i) allocated = true;
        if (!allocated) { students[i].display(); found = true; }
    }
    if (!found) cout << "None. All students have been allocated.\n";
}

bool Allocator::getLocationForStudent(int studentIndex, RoomManager &roomManager, int &shift, string &roomId, int &row, int &column)
{
    for (int i = 0; i < allocationCount; i++) if (allocations[i].studentIndex == studentIndex)
    {
        Room *room = roomManager.getRoomByIndex(allocations[i].roomIndex);
        shift = allocations[i].shift; roomId = room->getRoomId();
        row = allocations[i].row + 1; column = allocations[i].column + 1; return true;
    }
    return false;
}

void Allocator::searchGroup(string value, bool searchCourse, StudentManager &studentManager, RoomManager &roomManager)
{
    Student *students = studentManager.getStudents(); bool found = false;
    for (int i = 0; i < studentManager.getCount(); i++)
    {
        string field = searchCourse ? students[i].getCourse() : students[i].getSubject();
        if (field == value)
        {
            students[i].display(); found = true;
            bool location = false;
            for (int j = 0; j < allocationCount; j++) if (allocations[j].studentIndex == i)
            {
                Room *room = roomManager.getRoomByIndex(allocations[j].roomIndex);
                cout << "Shift " << allocations[j].shift << " | Room " << room->getRoomId()
                     << " | Row " << allocations[j].row + 1 << " | Column " << allocations[j].column + 1 << endl;
                location = true; break;
            }
            if (!location) cout << "Seat not allocated.\n";
        }
    }
    if (!found) cout << "No matching students found.\n";
}

void Allocator::displayGroupSeating(string value, bool searchCourse, StudentManager &studentManager, RoomManager &roomManager)
{
    Student *students = studentManager.getStudents();
    bool matchingStudentFound = false;
    for (int i = 0; i < studentManager.getCount(); i++)
    {
        string field = searchCourse ? students[i].getCourse() : students[i].getSubject();
        if (field == value) matchingStudentFound = true;
    }
    if (!matchingStudentFound)
    {
        cout << "No students found for " << (searchCourse ? "course: " : "subject: ") << value << endl;
        return;
    }

    // Show every room in every shift. Non-matching seats are deliberately printed as EMPTY.
    for (int shift = 1; shift <= totalShifts; shift++)
    {
        cout << "\n========== " << (searchCourse ? "COURSE: " : "SUBJECT: ")
             << value << " | SHIFT " << shift << " ==========\n";
        for (int r = 0; r < roomManager.getRoomCount(); r++)
        {
            Room *room = roomManager.getRoomByIndex(r);
            cout << "\nROOM " << room->getRoomId() << " (" << room->getRows()
                 << " rows x " << room->getColumns() << " columns)\n";
            cout << "       ";
            for (int col = 0; col < room->getColumns(); col++) cout << "| C" << col + 1 << "            ";
            cout << "|\n";
            for (int row = 0; row < room->getRows(); row++)
            {
                cout << "R" << row + 1 << "     ";
                for (int col = 0; col < room->getColumns(); col++)
                {
                    string seatValue = "EMPTY";
                    for (int a = 0; a < allocationCount; a++)
                    {
                        if (allocations[a].shift == shift && allocations[a].roomIndex == r &&
                            allocations[a].row == row && allocations[a].column == col)
                        {
                            Student &student = students[allocations[a].studentIndex];
                            string field = searchCourse ? student.getCourse() : student.getSubject();
                            if (field == value) seatValue = student.getRollNo();
                            break;
                        }
                    }
                    cout << "| " << seatValue;
                    int padding = 13 - (int)seatValue.length();
                    for (int k = 0; k < padding; k++) cout << ' ';
                }
                cout << "|\n";
            }
        }
    }
    cout << "\nNote: EMPTY means the seat is vacant for this selected "
         << (searchCourse ? "course" : "subject") << ".\n";
}

void Allocator::saveReport(StudentManager &studentManager, RoomManager &roomManager, string fileName)
{
    ofstream out(fileName.c_str());
    if (!out) { cout << "Could not open output file: " << fileName << endl; return; }
    out << "EXAM SEAT ALLOCATION REPORT\nTotal students: " << studentManager.getCount()
        << "\nTotal rooms per shift: " << roomManager.getRoomCount()
        << "\nSeats per shift: " << roomManager.getTotalSeats()
        << "\nAllocated students: " << allocatedCount
        << "\nUnallocated students: " << unallocatedCount
        << "\nTotal shifts: " << totalShifts << "\n";
    Student *students = studentManager.getStudents();
    for (int shift = 1; shift <= totalShifts; shift++)
    {
        out << "\n========== SHIFT " << shift << " ==========\n";
        for (int r = 0; r < roomManager.getRoomCount(); r++)
        {
            Room *room = roomManager.getRoomByIndex(r);
            out << "\nROOM " << room->getRoomId() << " (" << room->getRows() << "x" << room->getColumns() << ")\n";
            for (int row = 0; row < room->getRows(); row++)
            {
                for (int col = 0; col < room->getColumns(); col++)
                {
                    string value = "----";
                    for (int a = 0; a < allocationCount; a++)
                        if (allocations[a].shift == shift && allocations[a].roomIndex == r && allocations[a].row == row && allocations[a].column == col)
                            value = students[allocations[a].studentIndex].getRollNo();
                    out << value << (col + 1 == room->getColumns() ? "" : "\t");
                }
                out << "\n";
            }
        }
    }
    out << "\nUNALLOCATED STUDENTS\n";
    for (int i = 0; i < studentManager.getCount(); i++)
    {
        bool found = false;
        for (int a = 0; a < allocationCount; a++) if (allocations[a].studentIndex == i) found = true;
        if (!found) out << students[i].getRollNo() << " | " << students[i].getName() << "\n";
    }
    cout << "Report saved to " << fileName << endl;
}

int Allocator::getAllocatedCount() { return allocatedCount; }
int Allocator::getUnallocatedCount() { return unallocatedCount; }
int Allocator::getTotalShifts() { return totalShifts; }

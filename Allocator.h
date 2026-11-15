#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include "../StudentManagement/StudentManager.h"
#include "../RoomManagement/RoomManager.h"

struct Allocation
{
    int studentIndex;
    int shift;
    int roomIndex;
    int row;
    int column;
};

class Allocator
{
private:
    Allocation allocations[MAX_ALLOCATIONS];
    int allocationCount;
    int allocatedCount;
    int unallocatedCount;
    int totalShifts;
    bool hasConflict(Student *candidate, Room *room, int row, int column);
    bool placeStudent(int studentIndex, Student *students, RoomManager &rooms, int shift);
public:
    Allocator();
    void allocate(StudentManager &students, RoomManager &rooms);
    void displayShift(int shift, StudentManager &students, RoomManager &rooms);
    void displayAllShifts(StudentManager &students, RoomManager &rooms);
    void displayUnallocated(StudentManager &students);
    void saveReport(StudentManager &students, RoomManager &rooms, string fileName);
    bool getLocationForStudent(int studentIndex, RoomManager &rooms, int &shift, string &roomId, int &row, int &column);
    void searchGroup(string value, bool searchCourse, StudentManager &students, RoomManager &rooms);
    void displayGroupSeating(string value, bool searchCourse, StudentManager &students, RoomManager &rooms);
    int getAllocatedCount();
    int getUnallocatedCount();
    int getTotalShifts();
};
#endif

#ifndef ROOM_H
#define ROOM_H
#include "Seat.h"
#include "../StudentManagement/Config.h"
#include <string>
using namespace std;
class Room
{
private:
    string roomId;
    int rows, columns, totalSeats;
    Seat seats[MAX_ROOM_ROWS][MAX_ROOM_COLS];
public:
    Room();
    Room(string id, int r, int c, int total);
    string getRoomId();
    int getRows();
    int getColumns();
    int getTotalSeats();
    Seat* getSeat(int row, int column);
    void clearRoom();
    void display();
};
#endif

#include "Room.h"
#include <iostream>
#include <iomanip>
Room::Room() { roomId = ""; rows = 0; columns = 0; totalSeats = 0; }
Room::Room(string id, int r, int c, int total)
{
    roomId = id; rows = r; columns = c; totalSeats = total;
    for (int i = 0; i < MAX_ROOM_ROWS; i++)
        for (int j = 0; j < MAX_ROOM_COLS; j++) seats[i][j] = Seat(i + 1, j + 1);
}
string Room::getRoomId() { return roomId; }
int Room::getRows() { return rows; }
int Room::getColumns() { return columns; }
int Room::getTotalSeats() { return totalSeats; }
Seat* Room::getSeat(int row, int column)
{
    if (row < 0 || row >= rows || column < 0 || column >= columns) return nullptr;
    return &seats[row][column];
}
void Room::clearRoom()
{
    for (int r = 0; r < rows; r++) for (int c = 0; c < columns; c++) seats[r][c].clearSeat();
}
void Room::display()
{
    cout << "\n========== ROOM " << roomId << " ==========\n";
    cout << "Rows: " << rows << " | Columns: " << columns << " | Seats: " << totalSeats << "\n";
    cout << setw(6) << "Row";
    for (int c = 0; c < columns; c++) cout << setw(12) << c + 1;
    cout << endl;
    for (int r = 0; r < rows; r++)
    {
        cout << setw(6) << r + 1;
        for (int c = 0; c < columns; c++)
        {
            Student *s = seats[r][c].getStudent();
            if (s) cout << setw(12) << s->getRollNo(); else cout << setw(12) << "----";
        }
        cout << endl;
    }
}

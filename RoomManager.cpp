#include "RoomManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
RoomManager::RoomManager() { roomCount = 0; totalSeats = 0; }
bool RoomManager::loadFromFile(string fileName)
{
    ifstream input(fileName.c_str());
    if (!input) return false;
    roomCount = 0; totalSeats = 0;
    string line; int lineNo = 0, invalid = 0;
    while (getline(input, line))
    {
        lineNo++; if (line.empty()) continue;
        string fields[4]; stringstream ss(line); bool valid = true;
        for (int i = 0; i < 4; i++) if (!getline(ss, fields[i], '|')) { valid = false; break; }
        string extra; if (getline(ss, extra, '|')) valid = false;
        for (int i = 0; i < 4; i++) if (fields[i].empty()) valid = false;
        char *end1 = nullptr, *end2 = nullptr, *end3 = nullptr;
        long rows = strtol(fields[1].c_str(), &end1, 10);
        long cols = strtol(fields[2].c_str(), &end2, 10);
        long seats = strtol(fields[3].c_str(), &end3, 10);
        if (end1 == fields[1].c_str() || *end1 || end2 == fields[2].c_str() || *end2 || end3 == fields[3].c_str() || *end3) valid = false;
        if (rows <= 0 || cols <= 0 || rows > MAX_ROOM_ROWS || cols > MAX_ROOM_COLS || seats != rows * cols) valid = false;
        for (int i = 0; i < roomCount; i++) if (rooms[i].getRoomId() == fields[0]) valid = false;
        if (!valid || roomCount >= MAX_ROOMS)
        {
            cout << "Invalid room record at line " << lineNo << ". Check fields, unique ID, dimensions and capacity.\n";
            invalid++; continue;
        }
        rooms[roomCount++] = Room(fields[0], (int)rows, (int)cols, (int)seats);
        totalSeats += (int)seats;
    }
    cout << "Room records loaded: " << roomCount << " | Invalid records: " << invalid << endl;
    return true;
}
int RoomManager::getRoomCount() { return roomCount; }
int RoomManager::getTotalSeats() { return totalSeats; }
Room* RoomManager::getRooms() { return rooms; }
Room* RoomManager::getRoomByIndex(int index) { if (index < 0 || index >= roomCount) return nullptr; return &rooms[index]; }
Room* RoomManager::getRoomById(string id) { for (int i=0;i<roomCount;i++) if (rooms[i].getRoomId()==id) return &rooms[i]; return nullptr; }
void RoomManager::clearAllRooms() { for (int i=0;i<roomCount;i++) rooms[i].clearRoom(); }
void RoomManager::displayAllRooms() { for (int i=0;i<roomCount;i++) rooms[i].display(); }

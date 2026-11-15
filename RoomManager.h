#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H
#include "Room.h"
class RoomManager
{
private:
    Room rooms[MAX_ROOMS];
    int roomCount;
    int totalSeats;
public:
    RoomManager();
    bool loadFromFile(string fileName);
    int getRoomCount();
    int getTotalSeats();
    Room* getRooms();
    Room* getRoomByIndex(int index);
    Room* getRoomById(string roomId);
    void clearAllRooms();
    void displayAllRooms();
};
#endif

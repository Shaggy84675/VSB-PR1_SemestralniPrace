#pragma once

#include <vector>
#include <string>

using namespace std;

struct Reservation;

struct Room {
	int id;
	short floor;
	int room_number;
	int seat_capacity;
	int reservation_price;
};

enum class Filter { FLOOR, PRICE, SEATS };

bool FillRoomsStructure(string &path, vector <struct Room> &data);
void PrintRoomsTable(vector <struct Room> &data);
bool AddNewRoom(string &rooms_path, vector <struct Room> &rooms_data);
bool RemoveRoom(string &rooms_path, string &reservations_path, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);

bool RoomIdExists(int id, vector <struct Room> &data);
bool RoomExists(int room, vector <struct Room> &data);
int FindRoomIndex(int room, vector <struct Room> &data);
bool CheckRoomsIntegrity(string &path, vector <struct Room> &data);
bool SaveRoomsStructure(string &path, vector <struct Room> &data);
int FindRoomID(int room, vector <struct Room> &data);
bool IsRoomFree(int room_id, vector <struct Reservation> &reservation_data);
bool IsRoomFree(int room_number, short day, short month, short year, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);
bool PrintFreeRooms(vector <struct Room> &rooms_data, vector <struct Reservation> &reservation_data);
bool PrintFreeRooms(short floor, vector <struct Room> &rooms_data, vector <struct Reservation> &reservation_data, Filter filter = Filter::FLOOR);

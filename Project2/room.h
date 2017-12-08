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

#define ROOM_ID_MIN_LENGTH			1
#define ROOM_ID_MAX_LENGTH			999999
#define ROOM_FLOOR_MIN_LENGTH		-999
#define ROOM_FLOOR_MAX_LENGTH		999
#define ROOM_ROOMNUM_MIN_LENGTH		0
#define ROOM_ROOMNUM_MAX_LENGTH		99999999
#define ROOM_SEATS_MIN_LENGTH		1
#define ROOM_SEATS_MAX_LENGTH		1000000
#define ROOM_PRICE_MIN_LENGTH		0
#define ROOM_PRICE_MAX_LENGTH		INT_MAX - 1



bool FillRoomsStructure(string &path, vector <Room> &data);
void PrintRoomsTable(vector <Room> &data);
bool AddNewRoom(string &rooms_path, vector <Room> &rooms_data);
bool RemoveRoom(string &rooms_path, string &reservations_path, vector <Room> &rooms_data, vector <Reservation> &reservations_data);

bool RoomIdExists(int id, vector <Room> &data);
bool RoomExists(int room, vector <Room> &data);
int FindRoomIndex(int room, vector <Room> &data);
bool CheckRoomsIntegrity(string &path, vector <Room> &data);
bool SaveRoomsStructure(string &path, vector <Room> &data);
int FindRoomID(int room, vector <Room> &data);
bool IsRoomFree(int room_id, vector <Reservation> &reservation_data);
bool IsRoomFree(int room_number, short day, short month, short year, vector <Room> &rooms_data, vector <Reservation> &reservations_data);
vector<Room> getFreeRooms(vector <Room> &rooms_data, vector <Reservation> &reservation_data);
vector<Room> getRoomsOnFloor(short floor, vector <Room> &data);
vector<Room> getRoomsOnPrice(int price, vector <Room> &data);
vector<Room> getRoomsOnSeats(int seats, vector <Room> &data);
void PrintRooms(vector <Room> &data);
bool selectFreeRooms(vector <Room> &rooms_data, vector <Reservation> &reservation_data);
vector <Room> getRoomsOnDate(short day, short month, short year, vector <Room> &rooms_data, vector <Reservation> &reservation_data);
Room ParserRoom(string row);
bool IsRoomValid(Room room);

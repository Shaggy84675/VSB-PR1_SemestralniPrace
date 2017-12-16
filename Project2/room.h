///
/// @brief Hlavickovy soubor, ktery obsahuje deklarace pro praci s mistnostmi
/// @author Patrik Leifert
/// @file room.h
/// @date Prosinec 2017
///

#pragma once

#include <vector>
#include <string>

using namespace std;

struct Reservation;

///
/// @struct Room
/// @brief Struktura obsahujici seznam mistnosti
///

struct Room {
	int id;					///< Unikatni ID pro identifikaci mistnosti
	short floor;			///< Patro mistnosti
	int room_number;		///< Cislo mistnosti
	int seat_capacity;		///< Kapacita sedadel mistnosti
	int reservation_price;	///< Cena za den rezervace mistnosti
};

#define ROOM_ID_MIN_VALUE			1
#define ROOM_ID_MAX_VALUE			999999
#define ROOM_FLOOR_MIN_VALUE		-999
#define ROOM_FLOOR_MAX_VALUE		999
#define ROOM_ROOMNUM_MIN_VALUE		0
#define ROOM_ROOMNUM_MAX_VALUE		99999999
#define ROOM_SEATS_MIN_VALUE		1
#define ROOM_SEATS_MAX_VALUE		1000000
#define ROOM_PRICE_MIN_VALUE		0
#define ROOM_PRICE_MAX_VALUE		INT_MAX - 1

#define SUBMENU_FILTERONDATE		'a'
#define SUBMENU_FILTERONPRICE		'b'
#define SUBMENU_FILTERONSEATS		'c'
#define SUBMENU_FILTEROVERSEATS		'd'
#define SUBMENU_FILTERONFLOOR		'e'
#define SUBMENU_FILTER				'f'
#define SUBMENU_LEAVE				'q'
#define SUBMENU_FILTERCLOSEST		'g'



bool FillRoomsStructure(string &path, vector <Room> &rooms);
void PrintRoomsTable(vector <Room> &rooms);
bool AddNewRoom(string &rooms_path, vector <Room> &rooms);
bool RemoveRoom(string &rooms_path, string &reservations_path, vector <Room> &rooms, vector <Reservation> &reservations);

bool RoomIdExists(int id, vector <Room> &rooms);
bool RoomExists(int room, vector <Room> &rooms);
int FindRoomIndex(int room, vector <Room> &rooms);
bool CheckRoomsIntegrity(string &path, vector <Room> &rooms);
bool SaveRoomsStructure(string &path, vector <Room> &rooms);
int FindRoomID(int room, vector <Room> &rooms);
bool IsRoomFree(int room_id, vector <Reservation> &reservations);
bool IsRoomFree(int room_id, short day, short month, short year, vector <Reservation> &reservations);
vector<Room> getFreeRooms(vector <Room> &rooms, vector <Reservation> &reservations);
vector<Room> getRoomsOnFloor(short floor, vector <Room> &rooms);
vector<Room> getRoomsOnPrice(int price, vector <Room> &rooms);
vector<Room> getRoomsOnSeats(int seats, vector <Room> &rooms);
vector<Room> getRoomsOverSeats(int seats, vector <Room> &rooms);
void PrintRooms(vector <Room> &rooms);
bool SubMenuSelection(vector <Room> &rooms, vector <Reservation> &reservations);
vector <Room> getRoomsOnDate(short day, short month, short year, vector <Room> &rooms, vector <Reservation> &reservations);
Room ParserRoom(string row);
bool IsRoomValid(Room room);
bool RoomComparator(Room room_old, Room room_new);

#pragma once

#include <string>
#include <vector>

using namespace std;

struct Room;

struct Reservation {
	int id;
	short day;
	short month;
	short year;
};

void PrintReservationsTable(vector <struct Reservation> &data);
void PrintReservationsTable(vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);
bool FillReservationsStructure(string &path, vector <struct Reservation> &data);
bool CheckReservationsIntegrity(string &path, vector <struct Reservation> &data);
bool SaveReservationsStructure(string &path, vector <struct Reservation> &data);
bool PrintFreeRooms(vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);
bool BookRoom(string &path, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);
bool IsRoomFree(int room_number, short day, short month, short year, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);
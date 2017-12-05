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
bool MakeReservation(string &path, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);
int FindReservationIndex(int id, short day, short month, short year, vector <struct Reservation> &data);
bool CancelReservation(string &path, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);
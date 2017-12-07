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

#define RESERVATION_ID_MIN_LENGTH		1
#define RESERVATION_ID_MAX_LENGTH		999999
#define RESERVATION_DAY_MIN_LENGTH		0
#define RESERVATION_DAY_MAX_LENGTH		31
#define RESERVATION_MONTH_MIN_LENGTH	0
#define RESERVATION_MONTH_MAX_LENGTH	12
#define RESERVATION_YEAR_MIN_LENGTH		2000
#define RESERVATION_YEAR_MAX_LENGTH		9999


void PrintReservationsTable(vector <struct Reservation> &data);
void PrintReservationsTable(vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);
bool FillReservationsStructure(string &path, vector <struct Reservation> &data);
bool CheckReservationsIntegrity(string &path, vector <struct Reservation> &data);
bool SaveReservationsStructure(string &path, vector <struct Reservation> &data);
bool MakeReservation(string &path, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);
int FindReservationIndex(int id, short day, short month, short year, vector <struct Reservation> &data);
bool CancelReservation(string &path, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data);
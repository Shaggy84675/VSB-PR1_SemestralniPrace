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

#define RESERVATION_ID_MIN_VALUE		1
#define RESERVATION_ID_MAX_VALUE		999999
#define RESERVATION_DAY_MIN_VALUE		0
#define RESERVATION_DAY_MAX_VALUE		31
#define RESERVATION_MONTH_MIN_VALUE		0
#define RESERVATION_MONTH_MAX_VALUE		12
#define RESERVATION_YEAR_MIN_VALUE		2000
#define RESERVATION_YEAR_MAX_VALUE		9999


void PrintReservations(vector <Reservation> &reservations);
void PrintReservationsTable(vector <Room> &rooms, vector <Reservation> &reservations);
bool FillReservationsStructure(string &path, vector <Reservation> &reservations);
bool CheckReservationsIntegrity(string &path, vector <Reservation> &reservations);
bool SaveReservationsStructure(string &path, vector <Reservation> &reservations);
bool MakeReservation(string &path, vector <Room> &rooms, vector <Reservation> &reservations);
int FindReservationIndex(int id, short day, short month, short year, vector <Reservation> &reservations);
bool CancelReservation(string &path, vector <Room> &rooms, vector <Reservation> &reservations);
bool IsReservationValid(Reservation reservation);
bool AppendRecordToReservationFile(string &path, Reservation &reservation);
Reservation ParserReservation(string row);
Reservation GetReservationDate(Reservation &reservation);
void GetTableRoomHeader();
void GetTableRoomSeparator(const int size);

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


void PrintReservations(vector <Reservation> &data);
void PrintReservationsTable(vector <Room> &rooms_data, vector <Reservation> &reservations_data);
bool FillReservationsStructure(string &path, vector <Reservation> &data);
bool CheckReservationsIntegrity(string &path, vector <Reservation> &data);
bool SaveReservationsStructure(string &path, vector <Reservation> &data);
bool MakeReservation(string &path, vector <Room> &rooms_data, vector <Reservation> &reservations_data);
int FindReservationIndex(int id, short day, short month, short year, vector <Reservation> &data);
bool CancelReservation(string &path, vector <Room> &rooms_data, vector <Reservation> &reservations_data);
bool IsReservationValid(Reservation reservation);
bool AppendRecordToFile(string &path, Reservation &reservation);
Reservation ParserReservation(string row);
Reservation GetReservationDate(Reservation &reservation);
#include "reservation.h"
#include "messages.h"
#include "room.h"
#include "helper.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdio>


bool IsRoomFree(int room_number, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data)
{
	if (reservations_data[FindRoomIndex(room_number, rooms_data)].day == 0 ||
		reservations_data[FindRoomIndex(room_number, rooms_data)].month == 0 ||
		reservations_data[FindRoomIndex(room_number, rooms_data)].year == 0)
	{
		return true;
	}
	return false;
}

bool BookRoom(vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data)
{
	int room_index;
	int room_number;
	string fulldate;
	short date;
	short month;
	short year;


	cout << BOOKROOM_INP_ROOMNUM;
	GET_INPUT(room_number, ROOM_INP_ERR(INT_MIN, INT_MAX), BOOKROOM_INP_ROOMNUM);

	room_index = FindRoomIndex(room_number, rooms_data);

	if (room_index == -1)
	{
		return false;
	}

	if (!IsRoomFree(room_number, rooms_data, reservations_data))
	{
		cout << BOOKROOM_NOT_AVAILABLE;
		return false;
	}

	while (true)
	{
		cout << BOOKROOM_INP_DATE;

		cin.ignore();
		getline(cin, fulldate);

		sscanf_s(fulldate.c_str(), "%hd.%hd.%hd", &date, &month, &year);

		if ((date > 31 || date < 0) || (month > 12 || month < 0) || (year < 2000 || year > 9999))
		{
			cout << BOOKROOM_INP_DATE_INVALID << endl;
			continue;
		}
		break;
	}

	return true;
}

bool PrintFreeRooms(vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data)
{
	cout << "+" << setw(55) << setfill('-') << "+" << endl;
	cout << "| Mistnost | Patro | Kapacita sedadel | Cena rezervace |" << endl;
	cout << left << setw(11) << setfill('-') << "+"
		<< left << setw(8) << setfill('-') << "+"
		<< left << setw(19) << setfill('-') << "+"
		<< left << setw(17) << setfill('-') << "+"
		<< left << "+" << endl;

	for (unsigned int i = 0; i < rooms_data.size(); i++)
	{
		stringstream date;
		stringstream curr;

		if (reservations_data[i].day == 0 || reservations_data[i].month == 0 || reservations_data[i].year == 0)
		{
			curr << rooms_data[i].reservation_price << " " << CURRENCY;
			cout << left << "| " << setw(9) << setfill(' ') << rooms_data[i].room_number
				<< left << "| " << setw(6) << setfill(' ') << rooms_data[i].floor
				<< left << "| " << setw(17) << setfill(' ') << rooms_data[i].seat_capacity
				<< left << "| " << setw(15) << setfill(' ') << curr.str() << "|" << endl;
		}
	}

	cout << "+" << setw(55) << setfill('-') << right << "+" << endl;

	return true;
}

void PrintReservationsTable(vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data)
{
	cout << "+" << setw(76) << setfill('-') << "+" << endl;
	cout << "| Mistnost | Patro | Kapacita sedadel | Cena rezervace | Rezervovana na den |" << endl;
	cout << left << setw(11) << setfill('-') << "+"
		<< left << setw(8) << setfill('-') << "+"
		<< left << setw(19) << setfill('-') << "+"
		<< left << setw(17) << setfill('-') << "+"
		<< left << setw(21) << setfill('-') << "+"
		<< left << "+" << endl;

	for (unsigned int i = 0; i < rooms_data.size(); i++)
	{
		stringstream date;
		stringstream curr;

		date << reservations_data[i].day << ".";
		curr << rooms_data[i].reservation_price << " " << CURRENCY;
		cout << left << "| " << setw(9) << setfill(' ') << rooms_data[i].room_number
			<< left << "| " << setw(6) << setfill(' ') << rooms_data[i].floor
			<< left << "| " << setw(17) << setfill(' ') << rooms_data[i].seat_capacity
			<< left << "| " << setw(15) << setfill(' ') << curr.str();
		if (date.str() == "0" || reservations_data[i].month == 0 || reservations_data[i].year == 0)
		{
			cout << left << "| " << setw(18) << setfill(' ') << " - " << " |" << endl;
		}
		else
		{
			cout << left << "| " << setw(3) << setfill(' ') << date.str()
				<< left << " " << setw(8) << setfill(' ') << GetMonthName(reservations_data[i].month)
				<< left << "  " << setw(4) << setfill(' ') << reservations_data[i].year << " |" << endl;
		}

	}

	cout << "+" << setw(76) << setfill('-') << right << "+" << endl;
}

bool SaveReservationsStructure(string &path, vector <struct Reservation> &data)
{
	fstream file;
	string s;

	file.open(path, fstream::in | fstream::out | fstream::app);

	if (!file.is_open())
	{
		cout << ERROR_FILE_NOT_FOUND(path) << endl;
		return false;
	}

	file >> s;
	file.close();
	file.open(path, ios::out);
	file << s << endl;

	for (unsigned int i = 0; i < data.size(); i++)
	{
		file << data[i].id << ";"
			<< setw(2) << setfill('0') << data[i].day << "."
			<< setw(2) << setfill('0') << data[i].month << "."
			<< setw(4) << setfill('0') << data[i].year << endl;
	}

	file.close();

	return true;
}

bool CheckReservationsIntegrity(string &path, vector <struct Reservation> &data)
{
	ifstream file;
	bool passed = true;
	string s;
	unsigned int file_records = 0;

	file.open(path);

	if (!file.is_open())
	{
		cout << ERROR_FILE_NOT_FOUND(path) << endl;
		return false;
	}

	getline(file, s);
	while (getline(file, s))
	{
		istringstream line(s);
		while (getline(line, s, '\n'))
		{
			file_records++;
		}
	}

	if (data.size() == file_records)
	{
		unsigned int position = 0;
		file.clear();
		file.seekg(0, ios::beg);
		getline(file, s);

		while (getline(file, s))
		{
			istringstream line(s);

			getline(line, s, ';');
			if (atoi(s.c_str()) != data[position].id)
			{
				cout << (passed ? CHECKROOMINTEGRITY_ERROR : "");
				cout << CHECKRESERVATIONINTEGRITY_DETAIL(position, "id");
				cout << endl;
				passed = false;
			}

			getline(line, s, '.');
			if (atoi(s.c_str()) != data[position].day)
			{
				cout << (passed ? CHECKROOMINTEGRITY_ERROR : "");
				cout << CHECKRESERVATIONINTEGRITY_DETAIL(position, "datum");
				cout << endl;
				passed = false;
			}
			
			getline(line, s, '.');
			if (atoi(s.c_str()) != data[position].month)
			{
				cout << (passed ? CHECKROOMINTEGRITY_ERROR : "");
				cout << CHECKRESERVATIONINTEGRITY_DETAIL(position, "datum");
				cout << endl;
				passed = false;
			}

			getline(line, s, '\n');
			if (atoi(s.c_str()) != data[position].year)
			{
				cout << (passed ? CHECKROOMINTEGRITY_ERROR : "");
				cout << CHECKRESERVATIONINTEGRITY_DETAIL(position, "datum");
				cout << endl;
				passed = false;
			}
			position++;
		}
		file.close();
	}
	else
	{
		passed = false;
		file.close();
	}


	if (!passed)
	{
		cout << CHECKROOMINTEGRITY_ADD;
		if (YesNoCheck())
		{
			data.clear();
			FillReservationsStructure(path, data);
			cout << CHECKROOMINTEGRITY_SUCCESS << endl;
			return true;
		}
		else
		{
			SaveReservationsStructure(path, data);
			return false;
		}
	}
	return false;
}



void PrintReservationsTable(vector <struct Reservation> &data)
{
	cout << "+" << setw(27) << setfill('-') << "+" << endl;
	cout << "|   ID   | Datum rezervace |" << endl;
	cout << left << setw(9) << setfill('-') << "+"
		<< left << setw(18) << setfill('-') << "+"
		<< left << "+" << endl;

	for (unsigned int i = 0; i < data.size(); i++)
	{
		stringstream date;
		date << data[i].day << "." << data[i].month << "." << data[i].year;
		cout << left << "| " << setw(7) << setfill(' ') << data[i].id
			<< left << "| " << setw(16) << setfill(' ') << date.str() << "|" << endl;
	}
	cout << "+" << setw(27) << setfill('-') << right << "+" << endl;
}

bool FillReservationsStructure(string &path, vector <struct Reservation> &data)
{
	ifstream reservations_file;
	Reservation reservation;
	string s;

	int id;
	short day, month, year;

	reservations_file.open(path);

	if (!reservations_file.is_open())
	{
		cout << ERROR_FILE_NOT_FOUND(path) << endl;
		return 0;
	}

	getline(reservations_file, s);
	while (getline(reservations_file, s))
	{
		istringstream line(s);
		if (s.find('"') != string::npos)
			return false;

		getline(line, s, ';');
		id = atoi(s.c_str());
		if (id < 1 || id > 999999)
			return false;
		reservation.id = id;

		getline(line, s, '.');
		day = atoi(s.c_str());
		if (day < 0 || day > 31 || s == "")
			return false;
		reservation.day = day;

		getline(line, s, '.');
		month = atoi(s.c_str());
		if (month < 0 || month > 12 || s == "")
			return false;
		reservation.month = month;

		getline(line, s, '.');
		year = atoi(s.c_str());
		if (year < 0 || year > 9999 || s == "")
			return false;
		reservation.year = year;		

		data.push_back(reservation);
	}
	reservations_file.close();
	return true;
}

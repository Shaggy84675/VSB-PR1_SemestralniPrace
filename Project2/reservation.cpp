///
/// @author Patrik Leifert
/// @file reservation.cpp
///

#include "reservation.h"
#include "message.h"
#include "room.h"
#include "helper.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdio>


int FindReservationIndex(int id, short day, short month, short year, vector <struct Reservation> &data)
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i].id == id &&
			data[i].day == day &&
			data[i].month == month &&
			data[i].year == year)
		{
			return i;
		}
	}
	return -1;
}

bool CancelReservation(string &path, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data)
{
	Room room;
	Reservation reservation;
	string fulldate;

	cout << CANCELRESERVATION_INP_ROOMNUM;
	GET_INPUT(room.room_number, ROOM_INP_ERR(INT_MIN, INT_MAX), CANCELRESERVATION_INP_ROOMNUM);

	reservation.id = FindRoomID(room.room_number, rooms_data);

	if (reservation.id == -1)
	{
		cout << ROOM_NOT_FOUND_ERR << endl;
		return false;
	}

	while (true)
	{
		cout << CANCELRESERVATION_INP_DATE << endl;

		cin.ignore();
		getline(cin, fulldate);

		sscanf_s(fulldate.c_str(), "%hd.%hd.%hd", &reservation.day, &reservation.month, &reservation.year);

		if ((reservation.day > RESERVATION_DAY_MAX_LENGTH || reservation.day < RESERVATION_DAY_MIN_LENGTH) ||
			(reservation.month > RESERVATION_MONTH_MAX_LENGTH || reservation.month < RESERVATION_MONTH_MIN_LENGTH) ||
			(reservation.year < RESERVATION_YEAR_MIN_LENGTH || reservation.year > RESERVATION_YEAR_MAX_LENGTH))
		{
			cout << INP_DATE_INVALID << endl;
			continue;
		}
		break;
	}

	if (IsRoomFree(reservation.id, reservation.day, reservation.month, reservation.year, rooms_data, reservations_data))
	{
		cout << CANCELRESERVATION_NOT_RESERVED;
		return false;
	}

	cout << CANCELRESERVATION_INP_CONFIRM(room.room_number);

	if (YesNoCheck())
	{
		if (!CheckReservationsIntegrity(path, reservations_data))
		{
			return false;
		}

		int index = FindReservationIndex(reservation.id, reservation.day, reservation.month, reservation.year, reservations_data);

		if (index == -1)
		{
			return false;
		}

		reservations_data.erase(reservations_data.begin() + index);

		if (!SaveReservationsStructure(path, reservations_data))
		{
			return false;
		}
	}
	else
	{
		cout << CANCELRESERVATION_CANCELED << endl;
		return false;
	}
	return true;
}

bool MakeReservation(string &path, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data)
{
	Room room;
	Reservation reservation;
	string fulldate;


	cout << CREATERESERVATION_INP_ROOMNUM;
	GET_INPUT(room.room_number, ROOM_INP_ERR(INT_MIN, INT_MAX), CREATERESERVATION_INP_ROOMNUM);

	reservation.id = FindRoomID(room.room_number, rooms_data);

	if (reservation.id == -1)
	{
		cout << ROOM_NOT_FOUND_ERR << endl;
		return false;
	}

	while (true)
	{
		cout << CREATERESERVATION_INP_DATE << endl;

		cin.ignore();
		getline(cin, fulldate);

		sscanf_s(fulldate.c_str(), "%hd.%hd.%hd", &reservation.day, &reservation.month, &reservation.year);

		if ((reservation.day > RESERVATION_DAY_MAX_LENGTH || reservation.day < RESERVATION_DAY_MIN_LENGTH) ||
			(reservation.month > RESERVATION_MONTH_MAX_LENGTH || reservation.month < RESERVATION_MONTH_MIN_LENGTH) ||
			(reservation.year < RESERVATION_YEAR_MIN_LENGTH || reservation.year > RESERVATION_YEAR_MAX_LENGTH))
		{
			cout << INP_DATE_INVALID << endl;
			continue;
		}
		break;
	}

	if (!IsRoomFree(reservation.id, reservation.day, reservation.month, reservation.year, rooms_data, reservations_data))
	{
		cout << CREATERESERVATION_NOT_AVAILABLE;
		return false;
	}

	cout << CREATERESERVATION_INP_CONFIRM(room.room_number);

	if (YesNoCheck())
	{
		ofstream out;
		out.open(path, ios::app);

		if (!out.is_open())
		{
			return false;
		}

		if (!CheckReservationsIntegrity(path, reservations_data))
		{
			return false;
		}

		reservations_data.push_back(reservation);

		out << reservation.id << ";"
			<< setw(2) << setfill('0') << reservation.day << "."
			<< setw(2) << setfill('0') << reservation.month << "."
			<< setw(4) << setfill('0') << reservation.year << endl;

		out.close();
	}
	else
	{
		cout << CREATERESERVATION_CANCELED << endl;
		return false;
	}
	return true;
}

void PrintReservationsTable(vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data)
{
	unsigned int records = 0;

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
		stringstream curr;
		records = 0;

		curr << rooms_data[i].reservation_price << " " << CURRENCY;
		cout << left << "| " << setw(9) << setfill(' ') << rooms_data[i].room_number
			<< left << "| " << setw(6) << setfill(' ') << rooms_data[i].floor
			<< left << "| " << setw(17) << setfill(' ') << rooms_data[i].seat_capacity
			<< left << "| " << setw(15) << setfill(' ') << curr.str();

		for (unsigned int j = 0; j < reservations_data.size(); j++)
		{
			if (records == 0 && rooms_data[i].id == reservations_data[j].id)
			{
				stringstream date;
				date << reservations_data[j].day << ".";
				cout << left << "| " << setw(3) << setfill(' ') << date.str()
					<< left << " " << setw(8) << setfill(' ') << GetMonthName(reservations_data[j].month)
					<< left << "  " << setw(4) << setfill(' ') << reservations_data[j].year << " |" << endl;
				records++;
			}
			
			else if (records > 0 && (rooms_data[i].id == reservations_data[j].id))
			{
				stringstream date;
				date << reservations_data[j].day << ".";
				cout << left << "| " << setw(9) << setfill(' ') << ""
					<< left << "| " << setw(6) << setfill(' ') << ""
					<< left << "| " << setw(17) << setfill(' ') << ""
					<< left << "| " << setw(15) << setfill(' ') << ""
					<< left << "| " << setw(3) << setfill(' ') << date.str()
					<< left << " " << setw(8) << setfill(' ') << GetMonthName(reservations_data[j].month)
					<< left << "  " << setw(4) << setfill(' ') << reservations_data[j].year << " |" << endl;
			}			
		}

		if (records == 0)
		{
			cout << left << "| " << setw(18) << setfill(' ') << " - " << " |" << endl;
		}
	}

	cout << "+" << setw(76) << setfill('-') << right << "+" << endl;
}

///
/// @brief Funkce, ktera ulozi obsah vektoru do CSV souboru se seznamem rezervaci
/// @param	path	Cesta k souboru, ktery obsahuje seznam rezervaci
/// @param	data	Vektor s ulozenymi rezervacemi
/// @retval true	Funkce vraci hodnotu true, jestlize ulozeni souboru probehlo uspesne
/// @retval false	Funkce vraci hodnotu false, jestlize ulozeni souboru skoncilo chybou
///

bool SaveReservationsStructure(string &path, vector <struct Reservation> &reservations)
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

	for (unsigned int i = 0; i < reservations.size(); i++)
	{
		file << reservations[i].id << ";"
			<< setw(2) << setfill('0') << reservations[i].day << "."
			<< setw(2) << setfill('0') << reservations[i].month << "."
			<< setw(4) << setfill('0') << reservations[i].year << endl;
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
		}
		else
		{
			SaveReservationsStructure(path, data);
		}
	}
	return true;
}

///
/// @brief Funkce pro vypsani obsahu struktury Reservation z vektoru, ktery byl naplnen CSV souborem
///

void PrintReservations(vector <struct Reservation> &reservations)
{
	cout << "+" << setw(27) << setfill('-') << "+" << endl;
	cout << "|   " << TABLECELL_ID << "   | " << TABLECELL_RESERVEDATE << " |" << endl;
	cout << left << setw(9) << setfill('-') << "+"
		<< left << setw(18) << setfill('-') << "+"
		<< left << "+" << endl;

	for (unsigned int i = 0; i < reservations.size(); i++)
	{
		stringstream date;

		date << reservations[i].day << "." << reservations[i].month << "." << reservations[i].year;

		cout << left << "| " << setw(7) << setfill(' ') << reservations[i].id;
		cout << left << "| " << setw(16) << setfill(' ') << date.str() << "|" << endl;
	}
	cout << "+" << setw(27) << setfill('-') << right << "+" << endl;
}

///
/// @brief	Funkce pro naplneni struktury Reservation z CSV souboru
/// @param	path	Cesta k souboru, ktery obsahuje seznam rezervaci
/// @param	data	Vektor s rezervacemi, ktery se ma naplnit
/// @retval	true	Funkce vraci hodnotu true, jestlize struktura byla uspesne naplnena
/// @retval	false	Funkce vraci hodnotu false, jestlize nastal problem pri nahravani dat ze souboru
///

bool FillReservationsStructure(string &path, vector <struct Reservation> &data)
{
	ifstream reservations_file;
	Reservation reservation;
	string s;

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
		reservation.id = atoi(s.c_str());
		if (reservation.id < RESERVATION_ID_MIN_LENGTH || reservation.id > RESERVATION_ID_MAX_LENGTH)
			return false;

		getline(line, s, '.');
		reservation.day = atoi(s.c_str());
		if (reservation.day < RESERVATION_DAY_MIN_LENGTH || reservation.day > RESERVATION_DAY_MAX_LENGTH)
			return false;

		getline(line, s, '.');
		reservation.month = atoi(s.c_str());
		if (reservation.month < RESERVATION_MONTH_MIN_LENGTH || reservation.month > RESERVATION_MONTH_MAX_LENGTH)
			return false;

		getline(line, s, '.');
		reservation.year = atoi(s.c_str());
		if (reservation.year < RESERVATION_YEAR_MIN_LENGTH || reservation.year > RESERVATION_YEAR_MAX_LENGTH)
			return false;

		data.push_back(reservation);
	}
	reservations_file.close();
	return true;
}

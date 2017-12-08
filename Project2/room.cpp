#include "room.h"
#include "message.h"
#include "reservation.h"
#include "helper.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

bool AppendRecordToRoomFile(string &path, Room &room)
{
	ofstream out;
	out.open(path, ios::app);

	if (!out.is_open())
	{
		return false;
	}	

	out << room.id << ";"
		<< room.floor << ";"
		<< room.room_number << ";"
		<< room.seat_capacity << ";"
		<< room.reservation_price << endl;

	out.close();

	return true;
}

bool IsRoomValid(Room room)
{
	if (room.id < ROOM_ID_MIN_LENGTH || room.id > ROOM_ID_MAX_LENGTH)
		return false;

	if (room.floor < ROOM_FLOOR_MIN_LENGTH || room.floor > ROOM_FLOOR_MAX_LENGTH)
		return false;

	if (room.room_number < ROOM_ROOMNUM_MIN_LENGTH || room.room_number > ROOM_ROOMNUM_MAX_LENGTH)
		return false;

	if (room.seat_capacity < ROOM_SEATS_MIN_LENGTH || room.seat_capacity > ROOM_SEATS_MAX_LENGTH)
		return false;

	if (room.reservation_price < ROOM_PRICE_MIN_LENGTH || room.reservation_price > ROOM_PRICE_MAX_LENGTH)
		return false;

	return true;
}

Room ParserRoom(string row)
{
	string s;
	Room room;
	istringstream line(row);

	getline(line, s, ';');
	room.id = atoi(s.c_str());	

	getline(line, s, ';');
	room.floor = atoi(s.c_str());	

	getline(line, s, ';');
	room.room_number = atoi(s.c_str());	

	getline(line, s, ';');
	room.seat_capacity = atoi(s.c_str());	

	getline(line, s, ';');
	room.reservation_price = atoi(s.c_str());

	return room;	
}

bool selectFreeRooms(vector <Room> &rooms_data, vector <Reservation> &reservation_data)
{
	Reservation reservation;
	int input;
	string fulldate;
	auto filteredRooms = getFreeRooms(rooms_data, reservation_data);
	
	switch (ShowSubMenu())
	{
	case 'q':
		break;
	case 'a':
		while (true)
		{
			cout << CREATERESERVATION_INP_DATE << endl;

			cin.ignore();
			getline(cin, fulldate);

			sscanf_s(fulldate.c_str(), "%hd.%hd.%hd", &reservation.day, &reservation.month, &reservation.year);

			if ((reservation.day > 31 || reservation.day < 0) || (reservation.month > 12 || reservation.month < 0) || (reservation.year < 2000 || reservation.year > 9999))
			{
				cout << INP_DATE_INVALID << endl;
				continue;
			}
			return true;
		}

		filteredRooms = getRoomsOnDate(reservation.day, reservation.month, reservation.year, rooms_data, reservation_data);
		PrintRooms(filteredRooms);
		return true;
	case 'b':
		cout << SELECTFREEROOMS_PRICE_LOW_INP;
		GET_INPUT(input, ADDROOM_INP_PRICE_ERR(INT_MAX), SELECTFREEROOMS_PRICE_LOW_INP);

		filteredRooms = getRoomsOnPrice(input, filteredRooms);
		PrintRooms(filteredRooms);
		return true;
	case 'c':
		cout << SELECTFREEROOMS_SEATS_INP;
		GET_INPUT(input, ADDROOM_INP_SEATS_ERR(INT_MAX), SELECTFREEROOMS_SEATS_INP);

		filteredRooms = getRoomsOnSeats(input, filteredRooms);
		PrintRooms(filteredRooms);
		return true;
	case 'd':
		cout << SELECTFREEROOMS_FLOOR_INP;
		GET_INPUT(input, ADDROOM_INP_FLOOR_ERR(SHRT_MIN, SHRT_MAX), SELECTFREEROOMS_FLOOR_INP);

		filteredRooms = getRoomsOnFloor(input, filteredRooms);
		return true;
	case 'e':
		PrintRooms(filteredRooms);
		return true;
	}
	return false;
}

void PrintRooms(vector <Room> &data)
{
	cout << "+" << setw(62) << setfill('-') << "+" << endl;
	cout << "| Mistnost | Patro | Kapacita sedadel | Cena za den rezervace |" << endl;
	cout << left << setw(11) << setfill('-') << "+"
		<< left << setw(8) << setfill('-') << "+"
		<< left << setw(19) << setfill('-') << "+"
		<< left << setw(24) << setfill('-') << "+"
		<< left << "+" << endl;

	for (unsigned int i = 0; i < data.size(); i++)
	{
		cout << left << "| " << setw(9) << setfill(' ') << data[i].room_number
			<< left << "| " << setw(6) << setfill(' ') << data[i].floor
			<< left << "| " << setw(17) << setfill(' ') << data[i].seat_capacity
			<< left << "| " << setw(22) << setfill(' ') << data[i].reservation_price << "|" << endl;
	}

	cout << "+" << setw(62) << setfill('-') << right << "+" << endl;
}

vector <struct Room> getRoomsOnDate(short day, short month, short year, vector <Room> &rooms_data, vector <Reservation> &reservation_data)
{
	vector<struct Room> free_rooms;

	for (unsigned int i = 0; i < rooms_data.size(); i++)
	{
		if (IsRoomFree(rooms_data[i].id, day, month, year, rooms_data, reservation_data))
		{
			free_rooms.push_back(rooms_data.at(i));
		}
	}
	return free_rooms;
}

vector<struct Room> getRoomsOnSeats(int seats, vector <Room> &data)
{
	vector<struct Room> free_rooms;

	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i].seat_capacity >= seats)
		{
			free_rooms = data;
		}
	}
	return free_rooms;
}

vector<struct Room> getRoomsOnPrice(int price, vector <Room> &data)
{
	vector<struct Room> free_rooms;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i].reservation_price <= price)
		{
			free_rooms = data;
		}
	}
	return free_rooms;
}

vector<struct Room> getRoomsOnFloor(short floor, vector <Room> &data)
{
	vector<struct Room> free_rooms;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i].floor == floor)
		{
			free_rooms = data;
		}
	}
	return free_rooms;
}

vector<struct Room> getFreeRooms(vector <Room> &rooms_data, vector <Reservation> &reservation_data)
{
	vector<struct Room> free_rooms;
	for (unsigned int i = 0; i < rooms_data.size(); i++)
	{
		if (IsRoomFree(rooms_data[i].id, reservation_data))
		{
			free_rooms.push_back(rooms_data.at(i));
		}
	}
	return free_rooms;
}

bool IsRoomFree(int room_id, short day, short month, short year, vector <Room> &rooms_data, vector <Reservation> &reservations_data)
{
	for (unsigned int i = 0; i < reservations_data.size(); i++)
	{
		if (reservations_data[i].id == room_id &&
			reservations_data[i].day == day &&
			reservations_data[i].month == month &&
			reservations_data[i].year == year)
		{
			return false;
		}
	}
	return true;
}

bool IsRoomFree(int room_id, vector <Reservation> &reservation_data)
{
	for (unsigned int i = 0; i < reservation_data.size(); i++)
	{
		if (reservation_data[i].id == room_id)
		{
			return false;
		}
	}
	return true;
}

int FindRoomID(int room, vector <Room> &data)
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i].room_number == room)
		{
			return data[i].id;
		}
	}
	return -1;
}

int FindRoomIndex(int room, vector <Room> &data)
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i].room_number == room)
		{
			return i;
		}
	}
	return -1;
}

bool RemoveRoom(string &rooms_path, string &reservations_path, vector <Room> &rooms_data, vector <Reservation> &reservations_data)
{
	int index;
	int room_id;
	int remove_what;

	PrintRoomsTable(rooms_data);

	cout << REMOVEROOM_INPUT;
	GET_INPUT(remove_what, ROOM_INP_ERR(ROOM_ROOMNUM_MIN_LENGTH, ROOM_ROOMNUM_MAX_LENGTH), REMOVEROOM_INPUT);

	index = FindRoomIndex(remove_what, rooms_data);
	room_id = FindRoomID(remove_what, rooms_data);

	if (index == -1)
	{
		cout << ROOM_NOT_FOUND_ERR << endl;
		return false;
	}

	rooms_data.erase(rooms_data.begin() + index);
	
	for (unsigned int i = 0; i < reservations_data.size(); i++)
	{
		if (reservations_data[i].id == room_id)
		{
			reservations_data.erase(reservations_data.begin() + i);
		}
	}
	
	SaveRoomsStructure(rooms_path, rooms_data);
	SaveReservationsStructure(reservations_path, reservations_data);

	return true;
}

void PrintRoomsTable(vector <Room> &data)
{
	cout << "+" << setw(71) << setfill('-') << "+" << endl;
	cout << "|   ID   | Patro | Mistnost | Kapacita sedadel | Cena za den rezervace |" << endl;
	cout << left << setw(9) << setfill('-') << "+"
		<< left << setw(8) << setfill('-') << "+"
		<< left << setw(11) << setfill('-') << "+"
		<< left << setw(19) << setfill('-') << "+"
		<< left << setw(24) << setfill('-') << "+"
		<< left << "+" << endl;

	for (unsigned int i = 0; i < data.size(); i++)
	{
		cout << left << "| " << setw(7) << setfill(' ') << data[i].id
			<< left << "| " << setw(6) << setfill(' ') << data[i].floor
			<< left << "| " << setw(9) << setfill(' ') << data[i].room_number
			<< left << "| " << setw(17) << setfill(' ') << data[i].seat_capacity
			<< left << "| " << setw(22) << setfill(' ') << data[i].reservation_price << "|" << endl;
	}

	cout << "+" << setw(71) << setfill('-') << right << "+" << endl;
}

bool SaveRoomsStructure(string &path, vector <Room> &data)
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
			<< data[i].floor << ";"
			<< data[i].room_number << ";"
			<< data[i].seat_capacity << ";"
			<< data[i].reservation_price << endl;
	}

	file.close();

	return true;
}

bool AddNewRoom(string &path, vector <Room> &data)
{
	Room room;

	cout << ADDROOM_INPUT;
	GET_INPUT(room.room_number, ROOM_INP_ERR(INT_MIN, INT_MAX), ADDROOM_INPUT);

	if (RoomExists(room.room_number, data))
	{
		cout << ADDROOM_ROOMNUM_EXISTS(room.room_number) << endl;
		return false;
	}

	cout << ADDROOM_INP_FLOOR;
	GET_INPUT(room.floor, ADDROOM_INP_FLOOR_ERR(SHRT_MIN, SHRT_MAX), ADDROOM_INP_FLOOR);

	cout << ADDROOM_INP_SEATS;
	GET_INPUT(room.seat_capacity, ADDROOM_INP_SEATS_ERR(INT_MAX), ADDROOM_INP_SEATS);

	cout << ADDROOM_INP_PRICE;
	GET_INPUT(room.reservation_price, ADDROOM_INP_PRICE_ERR(INT_MAX), ADDROOM_INP_PRICE);

	cout << "Cislo mistnosti: " << room.room_number << endl
		<< "Patro: " << room.floor << endl
		<< "Kapacita sedadel: " << room.seat_capacity << endl
		<< "Cena za den rezervace " << room.reservation_price << endl;

	cout << endl << ADDROOM_INP_CONFIRM << endl;

	if (YesNoCheck())
	{
		if (!CheckRoomsIntegrity(path, data))
		{
			return false;
		}

		room.id = data.back().id + 1;
		data.push_back(room);

		AppendRecordToRoomFile(path, room);
	}
	else
	{
		cout << ADDROOM_CANCELED << endl;
		return false;
	}

	return true;
}

bool CheckRoomsIntegrity(string &path, vector <Room> &data)
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
		int position = 0;
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
				cout << CHECKROOMINTEGRITY_DETAIL(position + 2, "id");
				cout << endl;
				passed = false;
			}

			getline(line, s, ';');
			if (atoi(s.c_str()) != data[position].floor)
			{
				cout << (passed ? CHECKROOMINTEGRITY_ERROR : "");
				cout << CHECKROOMINTEGRITY_DETAIL(position + 2, "patro");
				cout << endl;
				passed = false;
			}

			getline(line, s, ';');
			if (atoi(s.c_str()) != data[position].room_number)
			{
				cout << (passed ? CHECKROOMINTEGRITY_ERROR : "");
				cout << CHECKROOMINTEGRITY_DETAIL(position + 2, "mistnost");
				cout << endl;
				passed = false;
			}

			getline(line, s, ';');
			if (atoi(s.c_str()) != data[position].seat_capacity)
			{
				cout << (passed ? CHECKROOMINTEGRITY_ERROR : "");
				cout << CHECKROOMINTEGRITY_DETAIL(position + 2, "kapacita");
				cout << endl;
				passed = false;
			}

			getline(line, s, ';');
			if (atoi(s.c_str()) != data[position].reservation_price)
			{
				cout << (passed ? CHECKROOMINTEGRITY_ERROR : "");
				cout << CHECKROOMINTEGRITY_DETAIL(position + 2, "cena");
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
		cout << CHECKROOMINTEGRITY_ADD << endl;
		if (YesNoCheck())
		{
			data.clear();
			FillRoomsStructure(path, data);
			cout << CHECKROOMINTEGRITY_SUCCESS << endl;
		}
		else
		{
			SaveRoomsStructure(path, data);
		}
		return true;
	}
	return false;
}

bool RoomExists(int room, vector <Room> &data)
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i].room_number == room)
		{
			return true;
		}
	}
	return false;
}

bool RoomIdExists(int id, vector <Room> &data)
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i].id == id)
		{
			return true;
		}
	}
	return false;
}

bool FillRoomsStructure(string &path, vector <Room> &data)
{
	ifstream rooms_file;
	Room room;
	string s;

	rooms_file.open(path);

	if (!rooms_file.is_open())
	{
		cout << ERROR_FILE_NOT_FOUND(path) << endl;
		return 0;
	}

	getline(rooms_file, s);
	while (getline(rooms_file, s))
	{
		if (s.find('"') != string::npos)
			return false;

		if (count(s.begin(), s.end(), ';') != 4)
			return false;

		room = ParserRoom(s);

		if (!IsRoomValid(room))
		{
			return false;
		}

		if (RoomIdExists(room.id, data) || RoomExists(room.room_number, data))
		{
			return false;
		}

		data.push_back(room);
	}
	rooms_file.close();
	return true;
}
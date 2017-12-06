#include "room.h"
#include "message.h"
#include "reservation.h"
#include "helper.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>



bool selectFreeRooms(vector <struct Room> &rooms_data, vector <struct Reservation> &reservation_data)
{
	auto filteredRooms = getFreeRooms(rooms_data, reservation_data);
	
	switch (ShowSubMenu())
	{
	case 'q':
		break;
	case 'a':
		PrintRooms(filteredRooms);
		break;
	case 'b':
		short floor;
		cout << SELECTFREEROOMS_FLOOR_INP;
		GET_INPUT(floor, ADDROOM_INP_FLOOR_ERR(SHRT_MIN, SHRT_MAX), SELECTFREEROOMS_FLOOR_INP);

		filteredRooms = getRoomsOnFloor(floor, filteredRooms);
		PrintRooms(filteredRooms);
		break;
	case 'c':
		break;
	case 'd':
		break;
	}
	return false;
}

void PrintRooms(vector <struct Room> &data)
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

vector<struct Room> getRoomsOnSeats(int seats, vector <struct Room> &data)
{
	vector<struct Room> free_rooms;

	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i].seat_capacity == seats)
		{
			free_rooms = data;
		}
	}
	return free_rooms;
}

vector<struct Room> getRoomsOnPrice(int price, vector <struct Room> &data)
{
	vector<struct Room> free_rooms;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i].reservation_price == price)
		{
			free_rooms = data;
		}
	}
	return free_rooms;
}

vector<struct Room> getRoomsOnFloor(short floor, vector <struct Room> &data)
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

vector<struct Room> getFreeRooms(vector <struct Room> &rooms_data, vector <struct Reservation> &reservation_data)
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

bool IsRoomFree(int room_id, short day, short month, short year, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data)
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

bool IsRoomFree(int room_id, vector <struct Reservation> &reservation_data)
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

int FindRoomID(int room, vector <struct Room> &data)
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

int FindRoomIndex(int room, vector <struct Room> &data)
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

bool RemoveRoom(string &rooms_path, string &reservations_path, vector <struct Room> &rooms_data, vector <struct Reservation> &reservations_data)
{
	int index;
	int room_id;
	int remove_what;

	PrintRoomsTable(rooms_data);

	cout << REMOVEROOM_INPUT;

	while (!(cin >> remove_what))
	{
		cin.clear();

		while (cin.get() != '\n')
		{
			continue;
		}
	}

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

void PrintRoomsTable(vector <struct Room> &data)
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

bool SaveRoomsStructure(string &path, vector <struct Room> &data)
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

bool AddNewRoom(string &rooms_path, vector <struct Room> &rooms_data)
{
	Room room;

	cout << ADDROOM_INPUT;
	GET_INPUT(room.room_number, ROOM_INP_ERR(INT_MIN, INT_MAX), ADDROOM_INPUT);

	if (RoomExists(room.room_number, rooms_data))
	{
		cout << ADDROOM_ROOMNUM_EXISTS(room.room_number) << endl;
		return false;
	}

	cout << ADDROOM_INP_FLOOR;
	GET_INPUT(room.floor, ADDROOM_INP_FLOOR_ERR(SHRT_MIN, SHRT_MAX), ADDROOM_INP_FLOOR);

	cout << ADDROOM_INP_SEATS;
	GET_INPUT(room.seat_capacity, ADDROOM_INP_SEATS_ERR(SHRT_MAX), ADDROOM_INP_SEATS);

	cout << ADDROOM_INP_PRICE;
	GET_INPUT(room.reservation_price, ADDROOM_INP_PRICE_ERR(INT_MAX), ADDROOM_INP_PRICE);

	cout << "Cislo mistnosti: " << room.room_number << endl
		<< "Patro: " << room.floor << endl
		<< "Kapacita sedadel: " << room.seat_capacity << endl
		<< "Cena za den rezervace " << room.reservation_price << endl;

	cout << endl << ADDROOM_INP_CONFIRM << endl;

	if (YesNoCheck())
	{
		ofstream out_rooms;
		out_rooms.open(rooms_path, ios::app);

		if (!out_rooms.is_open())
		{
			return false;
		}

		CheckRoomsIntegrity(rooms_path, rooms_data);		

		room.id = rooms_data.back().id + 1;

		rooms_data.push_back(room);

		out_rooms << room.id << ";"
			<< room.floor << ";"
			<< room.room_number << ";"
			<< room.seat_capacity << ";"
			<< room.reservation_price << endl;

		out_rooms.close();
	}
	else
	{
		cout << ADDROOM_CANCELED << endl;
		return false;
	}

	return true;
}

bool CheckRoomsIntegrity(string &path, vector <struct Room> &data)
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

bool RoomExists(int room, vector <struct Room> &data)
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

bool RoomIdExists(int id, vector <struct Room> &data)
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

bool FillRoomsStructure(string &path, vector <struct Room> &data)
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
		istringstream line(s);

		if (s.find('"') != string::npos)
			return false;

		if (count(s.begin(), s.end(), ';') != 4)
			return false;

		getline(line, s, ';');
		room.id = atoi(s.c_str());
		if (room.id < 1 || room.id > 999999)
			return false;

		getline(line, s, ';');
		room.floor = atoi(s.c_str());
		if (room.floor < -999 || room.floor > 999 || s == "")
			return false;

		getline(line, s, ';');
		room.room_number = atoi(s.c_str());
		if (room.room_number < 0 || room.room_number > 99999999 || s == "")
			return false;

		getline(line, s, ';');
		room.seat_capacity = atoi(s.c_str());
		if (!room.seat_capacity || room.seat_capacity > 1000000)
			return false;

		getline(line, s, ';');
		room.reservation_price = atoi(s.c_str());
		if (room.reservation_price < 0 || room.reservation_price > INT_MAX - 1 || s == "")
			return false;

		if (RoomIdExists(room.id, data) || RoomExists(room.room_number, data))
		{
			return false;
		}

		data.push_back(room);
	}
	rooms_file.close();
	return true;
}
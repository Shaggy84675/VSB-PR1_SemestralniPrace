///
/// @brief Soubor obsahuje funkce pro praci s mistnostmi
/// @author Patrik Leifert
/// @file room.cpp
/// @date Prosinec 2017
///

#include "room.h"
#include "message.h"
#include "reservation.h"
#include "helper.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>


///
///

bool ExportToHtml(vector<Room> &rooms)
{
	string path;
	cout << HTMLEXPORT_CONFIRM;

	if (YesNoCheck())
	{
		ofstream out;
		out.open("out.html");

		if (!out.is_open())
		{
			cout << ERROR_FILE_NOT_FOUND(path);
			return false;
		}

		GenerateHTMLHeader(out);
		out << "<h1>Seznam volnych mistnosti</h1>" << endl;
		out << "<table><tr><th>ID</th><th>Patro</th><th>Mistnost</th><th>Kapacita sedadel</th><th>Cena rezervace</th></tr>" << endl;

		for (unsigned int i = 0; i < rooms.size(); i++)
		{
			stringstream curr;
			curr << rooms[i].reservation_price << " " << CURRENCY;
			out << "<tr>" << endl
				<< "<td>" << rooms[i].id << "</td>"
				<< "<td>" << rooms[i].floor << "</td>"
				<< "<td>" << rooms[i].room_number << "</td>"
				<< "<td>" << rooms[i].seat_capacity << "</td>"
				<< "<td>" << curr.str() << "</td>"
				<< "</tr>" << endl;
		}
		out << "</table>" << endl;
		GenerateHTMLFooter(out);
	}
	cout << endl;
	return true;
}

///
/// @brief Funkce, ktera prida obsah struktury Room na konec souboru
/// @param	path			Cesta k souboru, ktery obsahuje seznam mistnosti
/// @param	room			Struktura Room
/// @retval	true			Funkce vraci hodnotu true, jestlize se podari obsah struktury na konec souboru pridat
/// @retval false			Funkce vraci hodnotu false, jestlize pri ukladani obsahu struktury na konec souboru nastane chyba
///

bool AppendRecordToRoomFile(string &path, Room &room)
{
	ofstream out;
	out.open(path, ios::app);

	if (!out.is_open())
	{
		cout << ERROR_FILE_NOT_FOUND(path);
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

///
/// @brief Funkce, ktera zjisti, jestli mistnost vyhovuje vstupnim parametrum
/// @param	room				Struktura Room
/// @retval true			Funkce vraci hodnotu true, jestlize mistnost vyhovuje vstupnim parametrum
/// @retval false			Funkce vraci hodnotu false, jestlize mistnost obsahuje neplatne parametry
///

bool IsRoomValid(Room room)
{
	if (room.id < ROOM_ID_MIN_VALUE || room.id > ROOM_ID_MAX_VALUE)
		return false;

	if (room.floor < ROOM_FLOOR_MIN_VALUE || room.floor > ROOM_FLOOR_MAX_VALUE)
		return false;

	if (room.room_number < ROOM_ROOMNUM_MIN_VALUE || room.room_number > ROOM_ROOMNUM_MAX_VALUE)
		return false;

	if (room.seat_capacity < ROOM_SEATS_MIN_VALUE || room.seat_capacity > ROOM_SEATS_MAX_VALUE)
		return false;

	if (room.reservation_price < ROOM_PRICE_MIN_VALUE || room.reservation_price > ROOM_PRICE_MAX_VALUE)
		return false;

	return true;
}

///
/// @brief Funkce, ktera prevede data ze stringu do struktury Room
/// @param	row		String, ktery obsahuje radek v CSV souboru, ktery se ma prevest
/// @return	Funkce vraci strukturu Room s prevedenymi daty
///

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
	vector<Room> filteredRooms;
	
	switch (ShowSubMenu())
	{
	case SUBMENU_LEAVE:
		break;
	case SUBMENU_FILTERONDATE:
		do
		{
			cout << CREATERESERVATION_INP_DATE << endl;
			cin.ignore();
			getline(cin, fulldate);

			sscanf_s(fulldate.c_str(), "%hd.%hd.%hd", &reservation.day, &reservation.month, &reservation.year);
		} while (!IsDateValid(reservation.day, reservation.month, reservation.year));

		filteredRooms = getRoomsOnDate(reservation.day, reservation.month, reservation.year, rooms_data, reservation_data);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTERONPRICE:
		cout << SELECTFREEROOMS_PRICE_LOW_INP;
		GET_INPUT(input, ADDROOM_INP_PRICE_ERR(ROOM_PRICE_MAX_VALUE), SELECTFREEROOMS_PRICE_LOW_INP);

		filteredRooms = getRoomsOnPrice(input, rooms_data);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTERONSEATS:
		cout << SELECTFREEROOMS_SEATS_INP_ON;
		GET_INPUT(input, ADDROOM_INP_SEATS_ERR(ROOM_SEATS_MAX_VALUE), SELECTFREEROOMS_SEATS_INP_ON);

		filteredRooms = getRoomsOnSeats(input, rooms_data);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTEROVERSEATS:
		cout << SELECTFREEROOMS_SEATS_INP_OVER;
		GET_INPUT(input, ADDROOM_INP_SEATS_ERR(ROOM_SEATS_MAX_VALUE), SELECTFREEROOMS_SEATS_INP_OVER);

		filteredRooms = getRoomsOverSeats(input, rooms_data);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTERONFLOOR:
		cout << SELECTFREEROOMS_FLOOR_INP;
		GET_INPUT(input, ADDROOM_INP_FLOOR_ERR(ROOM_FLOOR_MIN_VALUE, ROOM_FLOOR_MAX_VALUE), SELECTFREEROOMS_FLOOR_INP);

		filteredRooms = getRoomsOnFloor(input, rooms_data);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTER:
		filteredRooms = getFreeRooms(rooms_data, reservation_data);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	}
	return false;
}

void PrintRooms(vector <Room> &data)
{
	GetTableRoomSeparator(62);
	cout << "| " << TABLECELL_ROOM << " | " << TABLECELL_FLOOR << " | " << TABLECELL_SEATS << " | " << TABLECELL_RESERVEDATE << " |" << endl;
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

	GetTableRoomSeparator(62);
}

///
/// @brief Funkce, ktera vrati vektor se vsemi volnymi mistnostmi na zaklade zadaneho data
/// @param	day				Den na ktery ma byt mistnost volna
/// @param	month			Mesic na ktery ma byt mistnost volna
/// @param	year			Rok na ktery ma byt mistnost volna
/// @param	rooms			Vektor s ulozenymi mistnostmi
/// @param	reservations	Vektor s ulozenymi rezervacemi
/// @return Funkce vraci vektor se strukturou Room se vsemi volnymi mistnostmi na zaklade zadaneho data
///

vector<Room> getRoomsOnDate(short day, short month, short year, vector <Room> &rooms, vector <Reservation> &reservations)
{
	vector<Room> free_rooms;

	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (IsRoomFree(rooms[i].id, day, month, year, reservations))
		{
			free_rooms.push_back(rooms.at(i));
		}
	}
	return free_rooms;
}

///
/// @brief Funkce, ktera vrati vektor s mistnostmi od zadane kapacity sedadel
/// @param	seats	Kapacita sedadel, od ktere se maji mistnosti zapsat do vektoru
/// @param	rooms	Vektor s ulozenymi mistnostmi
/// @return Funkce vraci vektor se strukturou Room se vsemi mistnostmi od zadane kapacity sedadel
///

vector<Room> getRoomsOverSeats(int seats, vector <Room> &rooms)
{
	vector<Room> free_rooms;

	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].seat_capacity >= seats)
		{
			free_rooms.push_back(rooms.at(i));
		}
	}
	return free_rooms;
}

///
/// @brief Funkce, ktera vrati vektor s mistnostmi do zadane kapacity sedadel
/// @param	seats	Kapacita sedadel, do ktere se maji mistnosti zapsat do vektoru
/// @param	rooms	Vektor s ulozenymi mistnostmi
/// @return Funkce vraci vektor se strukturou Room se vsemi mistnostmi do zadane kapacity sedadel
///

vector<Room> getRoomsOnSeats(int seats, vector <Room> &rooms)
{
	vector<Room> free_rooms;

	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].seat_capacity <= seats)
		{
			free_rooms.push_back(rooms.at(i));
		}
	}
	return free_rooms;
}

///
/// @brief Funkce, ktera vrati vektor s mistnostmi do zadane ceny
/// @param	price	Cena, do ktere se maji mistnosti zapsat do vektoru
/// @param	rooms	Vektor s ulozenymi mistnostmi
/// @return Funkce vraci vektor se strukturou Room se vsemi mistnostmi do zadane ceny
///

vector<Room> getRoomsOnPrice(int price, vector <Room> &rooms)
{
	vector<Room> free_rooms;
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].reservation_price <= price)
		{
			free_rooms.push_back(rooms.at(i));
		}
	}
	return free_rooms;
}

///
/// @brief Funkce, ktera vrati vektor s mistnostmi na zaklade zadaneho patra
/// @param	floor	Patro, na kterem se dane mistnosti nachazeji			
/// @param	rooms	Vektor s ulozenymi mistnostmi
/// @return Funkce vraci vektor se strukturou Room se vsemi mistnostmi na danem patre
///

vector<Room> getRoomsOnFloor(short floor, vector <Room> &rooms)
{
	vector<Room> free_rooms;
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].floor == floor)
		{
			free_rooms.push_back(rooms.at(i));
		}
	}
	return free_rooms;
}

///
/// @brief Funkce, ktera vrati vektor se vsemi nezarezerovanymi mistnostmi
/// @param	rooms			Vektor s ulozenymi mistnostmi
/// @param	reservations	Vektor s ulozenymi rezervacemi
/// @return Funkce vraci vektor se strukturou Room se vsemi nezarezervovanymi mistnostmi
///

vector<Room> getFreeRooms(vector <Room> &rooms, vector <Reservation> &reservations)
{
	vector<Room> free_rooms;
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (IsRoomFree(rooms[i].id, reservations))
		{
			free_rooms.push_back(rooms.at(i));
		}
	}
	return free_rooms;
}

///
/// @brief Funkce, ktera zjisti, jestli je mistnost volna na zaklade ID mistnosti a data rezervace
/// @param	room_id			ID mistnosti u ktere se ma zjistit, jestli je mistnost volna
/// @param	day				Den rezervace mistnosti
/// @param	month			Mesic rezervace mistnosti
/// @param	year			Rok rezervace mistnosti
/// @param	reservations	Vektor s ulozenymi rezervacemi
/// @retval	true			Funkce vraci hodnotu true, jestlize je dana mistnost volna
/// @retval false			Funkce vraci hodnotu false jestlize je dana mistnost obsazena
///

bool IsRoomFree(int room_id, short day, short month, short year, vector <Reservation> &reservations)
{
	for (unsigned int i = 0; i < reservations.size(); i++)
	{
		if (reservations[i].id == room_id &&
			reservations[i].day == day &&
			reservations[i].month == month &&
			reservations[i].year == year)
		{
			return false;
		}
	}
	return true;
}

///
/// @brief Funkce, ktera zjisti, jestli je mistnost volna na zaklade ID mistnosti
/// @param	room_id			ID mistnosti u ktere se ma zjistit, jestli je mistnost volna
/// @param	reservations	Vektor s ulozenymi rezervacemi
/// @retval	true			Funkce vraci hodnotu true, jestlize je dana mistnost volna
/// @retval false			Funkce vraci hodnotu false jestlize je dana mistnost obsazena
///

bool IsRoomFree(int room_id, vector <Reservation> &reservations)
{
	for (unsigned int i = 0; i < reservations.size(); i++)
	{
		if (reservations[i].id == room_id)
		{
			return false;
		}
	}
	return true;
}

///
/// @brief Funkce, ktera najde ID mistnosti na zaklade cisla mistnosti
/// @param	room	Cislo mistnosti u ktere se ma cislo mistnosti najit
/// @param	rooms	Vektor s ulozenymi mistnostmi
/// @return Funkce vraci ID mistnosti. Pokud ID nenalezne, vrati cislo -1.
///

int FindRoomID(int room, vector <Room> &rooms)
{
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].room_number == room)
		{
			return rooms[i].id;
		}
	}
	return -1;
}

///
/// @brief Funkce, ktera najde pozici mistnosti ve vektoru Room na zaklade cisla mistnosti
/// @param	room	Cislo mistnosti u ktere se ma pozice najit
/// @param	rooms	Vektor s ulozenymi mistnostmi
/// @return Funkce vraci index pozice mistnosti ve vektoru. Pokud pozici nenalezne, vrati cislo -1.
///

int FindRoomIndex(int room, vector <Room> &rooms)
{
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].room_number == room)
		{
			return i;
		}
	}
	return -1;
}

///
/// @brief Funkce, ktera odstrani mistnost
/// @param	rooms_path			Cesta k souboru, ktery obsahuje seznam mistnosti
/// @param	reservations_path	Cesta k souboru, ktery obsahuje seznam rezervaci
/// @param	rooms				Vektor s ulozenymi mistnostmi
/// @param	reservations		Vektor s ulozenymi rezervacemi
/// @retval	true				Funkce vraci hodnotu true, jestlize se mistnost podari uspesne odstranit
/// @retval false				Funkce vraci hodnotu false s chybovou hlaskou, ktera definuje proc se mistnost nepodarilo odstranit
///

bool RemoveRoom(string &rooms_path, string &reservations_path, vector <Room> &rooms, vector <Reservation> &reservations)
{
	int index;
	int room_id;
	int remove_what;

	PrintRoomsTable(rooms);

	cout << REMOVEROOM_INPUT;
	GET_INPUT(remove_what, ROOM_INP_ERR(ROOM_ROOMNUM_MIN_VALUE, ROOM_ROOMNUM_MAX_VALUE), REMOVEROOM_INPUT);

	index = FindRoomIndex(remove_what, rooms);
	room_id = FindRoomID(remove_what, rooms);

	if (index == -1)
	{
		cout << ROOM_NOT_FOUND_ERR << endl;
		return false;
	}

	if (!CheckRoomsIntegrity(rooms_path, rooms))
	{
		return false;
	}

	if (!CheckReservationsIntegrity(reservations_path, reservations))
	{
		return false;
	}

	rooms.erase(rooms.begin() + index);
	
	for (unsigned int i = 0; i < reservations.size(); i++)
	{
		if (reservations[i].id == room_id)
		{
			reservations.erase(reservations.begin() + i);
		}
	}
	
	SaveRoomsStructure(rooms_path, rooms);
	SaveReservationsStructure(reservations_path, reservations);

	return true;
}

void PrintRoomsTable(vector <Room> &data)
{
	GetTableRoomSeparator(71);
	cout << "|   " << TABLECELL_ID << "   | " << TABLECELL_FLOOR << " | " << TABLECELL_ROOM << " | " << TABLECELL_SEATS << " | " << TABLECELL_PRICE << " |" << endl;
	cout << left << setw(9) << setfill('-') << "+"
		<< left << setw(8) << setfill('-') << "+"
		<< left << setw(11) << setfill('-') << "+"
		<< left << setw(19) << setfill('-') << "+"
		<< left << setw(24) << setfill('-') << "+"
		<< left << "+" << endl;

	for (unsigned int i = 0; i < data.size(); i++)
	{
		stringstream curr;
		curr << data[i].reservation_price << " " << CURRENCY;
		cout << left << "| " << setw(7) << setfill(' ') << data[i].id
			<< left << "| " << setw(6) << setfill(' ') << data[i].floor
			<< left << "| " << setw(9) << setfill(' ') << data[i].room_number
			<< left << "| " << setw(17) << setfill(' ') << data[i].seat_capacity
			<< right << "| " << setw(21) << setfill(' ') << curr.str() << " |" << endl;
	}

	GetTableRoomSeparator(71);
}

///
/// @brief Funkce, ktera ulozi obsah vektoru do CSV souboru se seznamem mistnosti
/// @param	path			Cesta k souboru, ktery obsahuje seznam mistnosti
/// @param	rooms			Vektor s ulozenymi mistnostmi
/// @retval true			Funkce vraci hodnotu true, jestlize ulozeni souboru probehlo uspesne
/// @retval false			Funkce vraci hodnotu false, jestlize ulozeni souboru skoncilo chybou
///

bool SaveRoomsStructure(string &path, vector <Room> &rooms)
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

	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		file << rooms[i].id << ";"
			<< rooms[i].floor << ";"
			<< rooms[i].room_number << ";"
			<< rooms[i].seat_capacity << ";"
			<< rooms[i].reservation_price << endl;
	}

	file.close();

	return true;
}

bool AddNewRoom(string &path, vector <Room> &data)
{
	Room room;

	cout << ADDROOM_INPUT;
	GET_INPUT(room.room_number, ROOM_INP_ERR(INT_MIN, INT_MAX), ADDROOM_INPUT);

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

		if (RoomExists(room.room_number, data))
		{
			cout << ADDROOM_ROOMNUM_EXISTS(room.room_number) << endl;
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
	}
	return true;
}

///
/// @brief Funkce, ktera zjisti jestli existuje mistnost ve vektoru Room
/// @param	room	Cislo mistnosti, ktera se ma zjistit, jestli existuje
/// @param	rooms	Vektor s mistnostmi
/// @retval true	Funkce vraci hodnotu true, jestlize mistnost ve vektoru Room existuje
/// @retval false	Funkce vraci hodnotu false, jestlize mistnost nebyla ve vektoru Room nalezena
///

bool RoomExists(int room, vector <Room> &rooms)
{
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].room_number == room)
		{
			return true;
		}
	}
	return false;
}

///
/// @brief Funkce, ktera zjisti jestli existuje ve vektoru ID mistnosti
/// @param	id		ID mistnosti, ktera se ma zjistit, jestli existuje
/// @param	rooms	Vektor s mistnostmi
/// @retval true	Funkce vraci hodnotu true, jestlize ID mistnosti ve vektoru Room existuje
/// @retval false	Funkce vraci hodnotu false, jestlize ID mistnosti nebylo ve vektoru Room nalezeno
///

bool RoomIdExists(int id, vector <Room> &rooms)
{
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].id == id)
		{
			return true;
		}
	}
	return false;
}

///
/// @brief	Funkce pro naplneni struktury Room z CSV souboru
/// @param	path	Cesta k souboru, ktery obsahuje seznam mistnosti
/// @param	rooms	Vektor s mistnostmi, ktery se ma naplnit
/// @retval	true	Funkce vraci hodnotu true, jestlize struktura byla uspesne naplnena
/// @retval	false	Funkce vraci hodnotu false, jestlize nastal problem pri nahravani dat ze souboru
///

bool FillRoomsStructure(string &path, vector <Room> &rooms)
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

		if (RoomIdExists(room.id, rooms) || RoomExists(room.room_number, rooms))
		{
			return false;
		}

		rooms.push_back(room);
	}
	rooms_file.close();
	return true;
}
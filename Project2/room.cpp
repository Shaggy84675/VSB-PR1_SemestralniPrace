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
/// @brief Funkce, ktera porovnava dve struktury a zjistuje, zda jsou v techto strukturach stejna data
/// @param room_old			Struktura s puvodnimi daty mistnosti
/// @param room_new			Struktura s novymi daty mistnosti
/// @retval true			Funkce vraci hodnotu true, jestlize data v porovnanych strukturach jsou stejna
/// @retval false			Funkce vraci hodnotu false, jestlize data v porovnanych strukturach jsou odlisna
///

bool RoomComparator(Room room_old, Room room_new)
{
	if (room_old.id != room_new.id)
	{
		return false;
	}

	if (room_old.floor != room_new.floor)
	{
		return false;
	}

	if (room_old.room_number != room_new.room_number)
	{
		return false;
	}

	if (room_old.seat_capacity != room_new.seat_capacity)
	{
		return false;
	}

	if (room_old.reservation_price != room_new.reservation_price)
	{
		return false;
	}

	return true;
}

///
/// @brief Funkce, ktera vytvori HTML soubor s danymi mistnostmi
/// @param	rooms	Vektor s ulozenymi mistnostmi
/// @retval true	Funkce vraci hodnotu true, jestlize se podari vytvorit soubor s HTML obsahem
/// @retval false	Funkce vraci hodnotu false, pri vytvareni HTML souboru nastane chyba
///

bool ExportToHtml(vector<Room> &rooms)
{
	string path;
	cout << HTMLEXPORT_CONFIRM;

	if (YesNoCheck())
	{
		ofstream out;

		cout << FILE_INP;
		cin.ignore(INT_MAX, '\n');
		getline(cin, path);

		if (cin.fail())
		{
			cout << FILE_INP_ERR << endl;
			return false;
		}

		path += ".html";
		out.open(path);

		if (!out.is_open())
		{
			cout << ERROR_FILE_NOT_FOUND(path) << endl;
			return false;
		}

		GenerateHTMLHeader(out);
		out << "<h1>Seznam volnych mistnosti</h1>" << endl;
		out << "<table><tr><th>" << TABLECELL_ID << "</th><th>" << TABLECELL_FLOOR << "</th><th>" << TABLECELL_ROOM << "</th><th>"
			<< TABLECELL_SEATS << "</th><th>" << TABLECELL_PRICE << "</th></tr>" << endl;

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
/// @param	room	Struktura Room
/// @retval true	Funkce vraci hodnotu true, jestlize mistnost vyhovuje vstupnim parametrum
/// @retval false	Funkce vraci hodnotu false, jestlize mistnost obsahuje neplatne parametry
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

///
/// @brief Funkce, ktera provede dane vyhledavani na zaklaade moznosti z funkce ShowSubMenu()
/// @param	rooms			Vektor obsahujici seznam mistnosti
/// @param	reservations	Vektor obsahujici seznam rezervaci
/// @retval true			Funkce vraci hodnotu true, jestlize se uspesne provede prikaz ze switche
/// @retval	false			Funkce vraci hodnotu false, jestlize se nenalezne moznost pro switch
///

bool SubMenuSelection(vector <Room> &rooms, vector <Reservation> &reservations)
{
	Reservation reservation;
	int input;
	vector<Room> filteredRooms;
	string fulldate;

	switch (ShowSubMenu())
	{
	case SUBMENU_LEAVE:
		break;
	case SUBMENU_FILTERONDATE:
		cin.ignore(INT_MAX, '\n');
		do
		{
			cout << CREATERESERVATION_INP_DATE << endl;
			getline(cin, fulldate);

			sscanf_s(fulldate.c_str(), "%hd.%hd.%hd", &reservation.day, &reservation.month, &reservation.year);
		} while (!IsDateValid(reservation.day, reservation.month, reservation.year));

		filteredRooms = getRoomsOnDate(reservation.day, reservation.month, reservation.year, rooms, reservations);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTERONPRICE:
		cout << SELECTFREEROOMS_PRICE_LOW_INP;
		GET_INPUT(input, ADDROOM_INP_PRICE_ERR(ROOM_PRICE_MAX_VALUE), SELECTFREEROOMS_PRICE_LOW_INP, ROOM_PRICE_MIN_VALUE, ROOM_PRICE_MAX_VALUE);

		filteredRooms = getRoomsOnPrice(input, rooms);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTERONSEATS:
		cout << SELECTFREEROOMS_SEATS_INP_ON;
		GET_INPUT(input, ADDROOM_INP_SEATS_ERR(ROOM_SEATS_MAX_VALUE), SELECTFREEROOMS_SEATS_INP_ON, ROOM_SEATS_MIN_VALUE, ROOM_SEATS_MAX_VALUE);

		filteredRooms = getRoomsOnSeats(input, rooms);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTEROVERSEATS:
		cout << SELECTFREEROOMS_SEATS_INP_OVER;
		GET_INPUT(input, ADDROOM_INP_SEATS_ERR(ROOM_SEATS_MAX_VALUE), SELECTFREEROOMS_SEATS_INP_OVER, ROOM_SEATS_MIN_VALUE, ROOM_SEATS_MAX_VALUE);

		filteredRooms = getRoomsOverSeats(input, rooms);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTERONFLOOR:
		cout << SELECTFREEROOMS_FLOOR_INP;
		GET_INPUT(input, ADDROOM_INP_FLOOR_ERR(ROOM_FLOOR_MIN_VALUE, ROOM_FLOOR_MAX_VALUE), SELECTFREEROOMS_FLOOR_INP, ROOM_FLOOR_MIN_VALUE, ROOM_FLOOR_MAX_VALUE);

		filteredRooms = getRoomsOnFloor(input, rooms);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTER:
		filteredRooms = getFreeRooms(rooms, reservations);
		PrintRooms(filteredRooms);
		ExportToHtml(filteredRooms);
		return true;
	case SUBMENU_FILTERCLOSEST:
		PrintReservationsTable(rooms, reservations);
		ExportToHtml(rooms, reservations);
	}
	return false;
}

///
/// @brief Funkce, ktera vypise tabulku vsech mistnosti na zaklade daneho filtru ve vektoru Room
/// @param	rooms	Vektor s ulozenymi mistnostmi
///

void PrintRooms(vector <Room> &rooms)
{
	GetTableSeparator(62);
	cout << "| " << TABLECELL_ROOM << " | " << TABLECELL_FLOOR << " | " << TABLECELL_SEATS << " |    " << TABLECELL_RESERVEDATE << "    |" << endl;
	cout << left << setw(11) << setfill('-') << "+"
		<< left << setw(8) << setfill('-') << "+"
		<< left << setw(19) << setfill('-') << "+"
		<< left << setw(24) << setfill('-') << "+"
		<< left << "+" << endl;

	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		cout << left << "| " << setw(9) << setfill(' ') << rooms[i].room_number
			<< left << "| " << setw(6) << setfill(' ') << rooms[i].floor
			<< left << "| " << setw(17) << setfill(' ') << rooms[i].seat_capacity
			<< left << "| " << setw(22) << setfill(' ') << rooms[i].reservation_price << "|" << endl;
	}

	GetTableSeparator(62);
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
	GET_INPUT(remove_what, ROOM_INP_ERR(ROOM_ROOMNUM_MIN_VALUE, ROOM_ROOMNUM_MAX_VALUE), REMOVEROOM_INPUT, ROOM_ROOMNUM_MIN_VALUE, ROOM_ROOMNUM_MAX_VALUE);

	index = FindRoomIndex(remove_what, rooms);
	room_id = FindRoomID(remove_what, rooms);

	if (index == -1)
	{
		cout << ROOM_NOT_FOUND_ERR << endl;
		return false;
	}

	cout << REMOVEROOM_INP_CONFIRM(remove_what);

	if (YesNoCheck())
	{
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
	}
	else
	{
		cout << REMOVEROOM_CANCELED << endl;
		return false;
	}
	return true;
}

///
/// @brief Funkce pro vypsani obsahu struktury Room z vektoru, ktery byl naplnen CSV souborem
/// @param	rooms	Vektor s ulozenymi mistnostmi
///

void PrintRoomsTable(vector <Room> &rooms)
{
	GetTableSeparator(70);
	cout << "|   " << TABLECELL_ID << "   | " << TABLECELL_FLOOR << " | " << TABLECELL_ROOM << " | " << TABLECELL_SEATS << " |    " << TABLECELL_PRICE << "    |" << endl;
	cout << left << setw(9) << setfill('-') << "+"
		<< left << setw(8) << setfill('-') << "+"
		<< left << setw(11) << setfill('-') << "+"
		<< left << setw(19) << setfill('-') << "+"
		<< left << setw(23) << setfill('-') << "+"
		<< left << "+" << endl;

	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		stringstream curr;
		curr << rooms[i].reservation_price << " " << CURRENCY;
		cout << left << "| " << setw(7) << setfill(' ') << rooms[i].id
			<< left << "| " << setw(6) << setfill(' ') << rooms[i].floor
			<< left << "| " << setw(9) << setfill(' ') << rooms[i].room_number
			<< left << "| " << setw(17) << setfill(' ') << rooms[i].seat_capacity
			<< right << "| " << setw(20) << setfill(' ') << curr.str() << " |" << endl;
	}

	GetTableSeparator(70);
}

///
/// @brief Funkce, ktera ulozi obsah vektoru do CSV souboru se seznamem mistnosti
/// @param	path	Cesta k souboru, ktery obsahuje seznam mistnosti
/// @param	rooms	Vektor s ulozenymi mistnostmi
/// @retval true	Funkce vraci hodnotu true, jestlize ulozeni souboru probehlo uspesne
/// @retval false	Funkce vraci hodnotu false, jestlize ulozeni souboru skoncilo chybou
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

///
/// @brief Funkce, ktera prida novou mistnost
/// @param	path	Cesta k souboru, ktery obsahuje seznam mistnosti
/// @param	rooms	Vektor s ulozenymi mistnostmi
/// @retval true	Funkce vraci hodnotu true, jestlize se novou mistnost podari pridat
/// @retval false	Funkce vraci hodnotu false, jestlize nastane chyba pri pridavani mistnosti nebo uzivatel pridani mistnosti zrusil
///

bool AddNewRoom(string &path, vector <Room> &rooms)
{
	Room room;

	cout << ADDROOM_INPUT;
	GET_INPUT(room.room_number, ROOM_INP_ERR(ROOM_ROOMNUM_MIN_VALUE, ROOM_ROOMNUM_MAX_VALUE), ADDROOM_INPUT, ROOM_ROOMNUM_MIN_VALUE, ROOM_ROOMNUM_MAX_VALUE);

	cout << ADDROOM_INP_FLOOR;
	GET_INPUT(room.floor, ADDROOM_INP_FLOOR_ERR(ROOM_FLOOR_MIN_VALUE, ROOM_FLOOR_MAX_VALUE), ADDROOM_INP_FLOOR, ROOM_FLOOR_MIN_VALUE, ROOM_FLOOR_MAX_VALUE);

	cout << ADDROOM_INP_SEATS;
	GET_INPUT(room.seat_capacity, ADDROOM_INP_SEATS_ERR(ROOM_SEATS_MAX_VALUE), ADDROOM_INP_SEATS, ROOM_SEATS_MIN_VALUE, ROOM_SEATS_MAX_VALUE);

	cout << ADDROOM_INP_PRICE;
	GET_INPUT(room.reservation_price, ADDROOM_INP_PRICE_ERR(ROOM_PRICE_MAX_VALUE), ADDROOM_INP_PRICE, ROOM_PRICE_MIN_VALUE, ROOM_PRICE_MAX_VALUE);

	cout << ROOM_DESCRIPTION << room.room_number << endl
		<< FLOOR_DESCRIPTION << room.floor << endl
		<< SEATS_DESCRIPTION << room.seat_capacity << endl
		<< PRICE_DESCRIPTION << room.reservation_price << endl;

	cout << endl << ADDROOM_INP_CONFIRM << endl;

	if (YesNoCheck())
	{
		if (!CheckRoomsIntegrity(path, rooms))
		{
			return false;
		}

		if (RoomExists(room.room_number, rooms))
		{
			cout << ADDROOM_ROOMNUM_EXISTS(room.room_number) << endl;
			return false;
		}

		room.id = rooms.back().id + 1;
		rooms.push_back(room);

		AppendRecordToRoomFile(path, room);
	}
	else
	{
		cout << ADDROOM_CANCELED << endl;
		return false;
	}

	return true;
}

///
/// @brief	Funkce, ktera zkontroluje integritu mezi daty v aplikaci a CSV souboru
/// @param	path	Cesta k souboru, ktery obsahuje seznam mistnosti
/// @param	rooms	Vektor s mistnostmi, ktery se ma naplnit
/// @retval	true	Funkce vraci hodnotu true, jestlize jsou data v souboru stejna jako v aplikaci
/// @retval	false	Funkce vraci hodnotu false, jestlize nastal problem pri kontrole
///

bool CheckRoomsIntegrity(string &path, vector <Room> &rooms)
{
	ifstream file;
	Room room;
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

	if (rooms.size() == file_records)
	{
		unsigned int position = 0;
		file.clear();
		file.seekg(0, ios::beg);
		getline(file, s);

		while (getline(file, s))
		{
			room = ParserRoom(s);

			if (!RoomComparator(room, rooms[position]))
			{
				cout << (passed ? CHECKINTEGRITY_ERROR : "");
				cout << CHECKROOMINTEGRITY_DETAIL(position + 2) << endl;
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
		cout << CHECKINTEGRITY_ADD << endl;
		if (YesNoCheck())
		{
			rooms.clear();
			FillRoomsStructure(path, rooms);
			cout << CHECKINTEGRITY_SUCCESS << endl;
		}
		else
		{
			SaveRoomsStructure(path, rooms);
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
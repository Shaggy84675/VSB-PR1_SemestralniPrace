///
/// @brief Soubor obsahuje funkce pro praci s rezervacemi
/// @author Patrik Leifert
/// @file reservation.cpp
/// @date Prosinec 2017
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


///
/// @brief Funkce, ktera vytvori HTML soubor s danymi mistnostmi a daty rezervaci
/// @param	rooms			Vektor s ulozenymi mistnostmi
/// @param	reservations	Vektor s ulozenymi rezervacemi
/// @retval true			Funkce vraci hodnotu true, jestlize se podari vytvorit soubor s HTML obsahem
/// @retval false			Funkce vraci hodnotu false, pri vytvareni HTML souboru nastane chyba
///

bool ExportToHtml(vector<Room> &rooms, vector<Reservation> &reservations)
{
	string path;
	cout << HTMLEXPORT_CONFIRM;

	if (YesNoCheck())
	{
		ofstream out;
		unsigned int records = 0;

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
			cout << ERROR_FILE_NOT_FOUND(path);
			return false;
		}

		GenerateHTMLHeader(out);
		out << "<h1>Seznam volnych mistnosti</h1>" << endl;
		out << "<table><tr><th>" << TABLECELL_ROOM << "</th><th>" << TABLECELL_FLOOR << "</th><th>" << TABLECELL_SEATS << "</th><th>"
			<< TABLECELL_PRICE << "</th><th>" << TABLECELL_RESERVEDATE << "</th></tr>" << endl;

		for (unsigned int i = 0; i < rooms.size(); i++)
		{
			stringstream curr;
			records = 0;

			curr << rooms[i].reservation_price << " " << CURRENCY;
			out << "<td>" << rooms[i].room_number << "</td>"
				<< "<td>" << rooms[i].floor << "</td>"
				<< "<td>" << rooms[i].seat_capacity << "</td>"
				<< "<td>" << curr.str() << "</td>";

			for (unsigned int j = 0; j < reservations.size(); j++)
			{
				if (records == 0 && rooms[i].id == reservations[j].id)
				{
					stringstream date;
					date << reservations[j].day << ". " << GetMonthName(reservations[j].month) << " " << reservations[j].year;
					out << "<td>" << date.str() << "</td>";
					out << "</tr>" << endl;
					records++;
				}

				else if (records > 0 && (rooms[i].id == reservations[j].id))
				{
					stringstream date;
					date << reservations[j].day << ". " << GetMonthName(reservations[j].month) << " " << reservations[j].year;
					out << "<td></td><td></td><td></td><td></td>"
						<< "<td>" << date.str() << "</td>";
					out << "</tr>" << endl;

				}
			}

			if (records == 0)
			{
				out << "<td>-</td>";
				out << "</tr>" << endl;
			}
		}
		out << "</table>" << endl;
		GenerateHTMLFooter(out);
	}
	return true;
}

///
/// @brief Funkce, ktera porovnava dve struktury a zjistuje, zda jsou v techto strukturach stejna data
/// @param reservation_old	Struktura s puvodnimi daty rezervaci
/// @param reservation_new	Struktura s novymi daty rezervaci
/// @retval true			Funkce vraci hodnotu true, jestlize data v porovnanych strukturach jsou stejna
/// @retval false			Funkce vraci hodnotu false, jestlize data v porovnanych strukturach jsou odlisna
///

bool ReservationComparator(Reservation reservation_old, Reservation reservation_new)
{
	if (reservation_old.id != reservation_new.id)
	{
		return false;
	}

	if (reservation_old.day != reservation_new.day)
	{
		return false;
	}

	if (reservation_old.month != reservation_new.month)
	{
		return false;
	}

	if (reservation_old.year != reservation_new.year)
	{
		return false;
	}

	return true;
}

///
/// @brief Funkce, ktera zjistuje datum rezervace na zaklade vstupu z konzole a nasledne ulozi do struktury Reservation
/// @param	reservation	Struktura Reservation
/// @return Funkce vraci strukturu Reservation s vyplnenymi hodnotami data rezervace z konzole
///

Reservation GetReservationDate(Reservation &reservation)
{
	string fulldate;

	cin.ignore(INT_MAX, '\n');
	do
	{
		getline(cin, fulldate);

		sscanf_s(fulldate.c_str(), "%hd.%hd.%hd", &reservation.day, &reservation.month, &reservation.year);
	} while (!IsReservationValid(reservation));

	return reservation;
}

///
/// @brief Funkce, ktera prida obsah struktury Reservation na konec souboru
/// @param	path			Cesta k souboru, ktery obsahuje seznam rezervaci
/// @param	reservation		Struktura Reservation
/// @retval	true			Funkce vraci hodnotu true, jestlize se podari obsah struktury na konec souboru pridat
/// @retval false			Funkce vraci hodnotu false, jestlize pri ukladani obsahu struktury na konec souboru nastane chyba
///

bool AppendRecordToReservationFile(string &path, Reservation &reservation)
{
	ofstream out;
	out.open(path, ios::app);

	if (!out.is_open())
	{
		cout << ERROR_FILE_NOT_FOUND(path);
		return false;
	}

	out << reservation.id << ";"
		<< setw(2) << setfill('0') << reservation.day << "."
		<< setw(2) << setfill('0') << reservation.month << "."
		<< setw(4) << setfill('0') << reservation.year << endl;

	out.close();

	return true;
}

///
/// @brief Funkce, ktera zjisti, jestli rezervace vyhovuje vstupnim parametrum
/// @param	reservation		Struktura Reservation
/// @retval true			Funkce vraci hodnotu true, jestlize rezervace vyhovuje vstupnim parametrum
/// @retval false			Funkce vraci hodnotu false, jestlize rezervace obsahuje neplatne parametry
///

bool IsReservationValid(Reservation reservation)
{
	if (reservation.id < RESERVATION_ID_MIN_VALUE || reservation.id > RESERVATION_ID_MAX_VALUE)
	{
		return false;
	}
	else if (!IsDateValid(reservation.day, reservation.month, reservation.year))
	{
		cout << INP_DATE_INVALID << endl;
		return false;
	}
	return true;
}

///
/// @brief Funkce, ktera zjisti pozici rezervace ve vektoru Reservation na zaklade zadaneho id a data
/// @param	id				ID mistnosti
/// @param	day				Den rezervace mistnosti
/// @param	month			Mesic rezervace mistnosti
/// @param	year			Rok rezervace mistnosti
/// @param	reservations	Vektor s ulozenymi rezervacemi
/// @return	Funkce vraci index pozice rezervace ve vektoru. Pokud pozici nenalezne, vrati cislo -1.
///

int FindReservationIndex(int id, short day, short month, short year, vector <Reservation> &reservations)
{
	for (unsigned int i = 0; i < reservations.size(); i++)
	{
		if (reservations[i].id == id &&
			reservations[i].day == day &&
			reservations[i].month == month &&
			reservations[i].year == year)
		{
			return i;
		}
	}
	return -1;
}

///
/// @brief Funkce, ktera zrusi stavajici rezervaci
/// @param	path			Cesta k souboru, ktery obsahuje seznam rezervaci
/// @param	rooms			Vektor s ulozenymi mistnostmi
/// @param	reservations	Vektor s ulozenymi rezervacemi
/// @retval	true			Funkce vraci hodnotu true, jestlize se uspesne podari rezervaci zrusit
/// @retval false			Funkce vraci hodnotu false s chybovou hlaskou, ktera definuje proc se rezervaci nepodarilo zrusit
///

bool CancelReservation(string &path, vector <Room> &rooms, vector <Reservation> &reservations)
{
	Room room;
	Reservation reservation;

	cout << CANCELRESERVATION_INP_ROOMNUM;
	GET_INPUT(room.room_number, ROOM_INP_ERR(ROOM_ROOMNUM_MIN_VALUE, ROOM_ROOMNUM_MAX_VALUE), CANCELRESERVATION_INP_ROOMNUM, ROOM_ROOMNUM_MIN_VALUE, ROOM_ROOMNUM_MAX_VALUE);

	reservation.id = FindRoomID(room.room_number, rooms);

	if (reservation.id == -1)
	{
		cout << ROOM_NOT_FOUND_ERR << endl;
		return false;
	}

	if (IsRoomFree(reservation.id, reservations))
	{
		cout << ROOM_NOT_RESERVED << endl;
		return false;
	}

	cout << CANCELRESERVATION_INP_DATE << endl;
	reservation = GetReservationDate(reservation);

	cout << CANCELRESERVATION_INP_CONFIRM(room.room_number);

	if (YesNoCheck())
	{
		if (!CheckReservationsIntegrity(path, reservations))
		{
			return false;
		}

		if (IsRoomFree(reservation.id, reservation.day, reservation.month, reservation.year, reservations))
		{
			cout << ROOM_NOT_RESERVED << endl;
			return false;
		}

		int index = FindReservationIndex(reservation.id, reservation.day, reservation.month, reservation.year, reservations);

		if (index == -1)
		{
			cout << ROOM_INDEX_NOT_FOUND << endl;
			return false;
		}

		reservations.erase(reservations.begin() + index);

		if (!SaveReservationsStructure(path, reservations))
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

///
/// @brief Funkce, ktera vytvori rezervaci
/// @param	path			Cesta k souboru, ktery obsahuje seznam rezervaci
/// @param	rooms			Vektor s ulozenymi mistnostmi
/// @param	reservations	Vektor s ulozenymi rezervacemi
/// @retval	true			Funkce vraci hodnotu true, jestlize se uspesne podari rezervaci vytvorit
/// @retval false			Funkce vraci hodnotu false s chybovou hlaskou, ktera definuje proc se rezervaci nepodarilo vytvorit
///

bool MakeReservation(string &path, vector <Room> &rooms, vector <Reservation> &reservations)
{
	Room room;
	Reservation reservation;
	string fulldate;


	cout << CREATERESERVATION_INP_ROOMNUM;
	GET_INPUT(room.room_number, ROOM_INP_ERR(ROOM_ROOMNUM_MIN_VALUE, ROOM_ROOMNUM_MAX_VALUE), ADDROOM_INPUT, ROOM_ROOMNUM_MIN_VALUE, ROOM_ROOMNUM_MAX_VALUE);

	reservation.id = FindRoomID(room.room_number, rooms);

	if (reservation.id == -1)
	{
		cout << ROOM_NOT_FOUND_ERR << endl;
		return false;
	}

	cout << CREATERESERVATION_INP_DATE << endl;

	reservation = GetReservationDate(reservation);	

	cout << CREATERESERVATION_INP_CONFIRM(room.room_number);

	if (YesNoCheck())
	{
		if (!CheckReservationsIntegrity(path, reservations))
		{
			return false;
		}

		if (!IsRoomFree(reservation.id, reservation.day, reservation.month, reservation.year, reservations))
		{
			cout << CREATERESERVATION_NOT_AVAILABLE << endl;
			return false;
		}

		reservations.push_back(reservation);
		if (!AppendRecordToReservationFile(path, reservation))
		{
			return false;
		}
	}
	else
	{
		cout << CREATERESERVATION_CANCELED << endl;
		return false;
	}
	return true;
}

///
/// @brief Funkce, ktera vypise tabulku vsech rezervaci vcetne data na kdy je mistnost rezervovana
/// @param	rooms			Vektor s ulozenymi mistnostmi
/// @param	reservations	Vektor s ulozenymi rezervacemi
///

void PrintReservationsTable(vector <Room> &rooms, vector <Reservation> &reservations)
{
	unsigned int records = 0;

	GetTableSeparator(75);

	cout << "| " << TABLECELL_ROOM << " | " << TABLECELL_FLOOR << " | " << TABLECELL_SEATS << " | " << TABLECELL_PRICE << " |  " << TABLECELL_RESERVEDATE << "  |" << endl;
	cout << left << setw(11) << setfill('-') << "+"
		<< left << setw(8) << setfill('-') << "+"
		<< left << setw(19) << setfill('-') << "+"
		<< left << setw(17) << setfill('-') << "+"
		<< left << setw(20) << setfill('-') << "+"
		<< left << "+" << endl;

	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		stringstream curr;
		records = 0;

		curr << rooms[i].reservation_price << " " << CURRENCY;
		cout << left << "| " << setw(9) << setfill(' ') << rooms[i].room_number
			<< left << "| " << setw(6) << setfill(' ') << rooms[i].floor
			<< left << "| " << setw(17) << setfill(' ') << rooms[i].seat_capacity
			<< right << "| " << setw(14) << setfill(' ') << curr.str() << " ";

		for (unsigned int j = 0; j < reservations.size(); j++)
		{
			if (records == 0 && rooms[i].id == reservations[j].id)
			{
				stringstream date;
				date << reservations[j].day << ".";
				cout << left << "| " << setw(3) << setfill(' ') << date.str()
					<< left << " " << setw(8) << setfill(' ') << GetMonthName(reservations[j].month)
					<< left << " " << setw(4) << setfill(' ') << reservations[j].year << " |" << endl;
				records++;
			}
			
			else if (records > 0 && (rooms[i].id == reservations[j].id))
			{
				stringstream date;
				date << reservations[j].day << ".";
				cout << left << "| " << setw(9) << setfill(' ') << ""
					<< left << "| " << setw(6) << setfill(' ') << ""
					<< left << "| " << setw(17) << setfill(' ') << ""
					<< left << "| " << setw(15) << setfill(' ') << ""
					<< left << "| " << setw(3) << setfill(' ') << date.str()
					<< left << " " << setw(8) << setfill(' ') << GetMonthName(reservations[j].month)
					<< left << " " << setw(4) << setfill(' ') << reservations[j].year << " |" << endl;
			}			
		}

		if (records == 0)
		{
			cout << left << "| " << setw(17) << setfill(' ') << " - " << " |" << endl;
		}
	}

	GetTableSeparator(75);
}

///
/// @brief Funkce, ktera ulozi obsah vektoru do CSV souboru se seznamem rezervaci
/// @param	path			Cesta k souboru, ktery obsahuje seznam rezervaci
/// @param	reservations	Vektor s ulozenymi rezervacemi
/// @retval true			Funkce vraci hodnotu true, jestlize ulozeni souboru probehlo uspesne
/// @retval false			Funkce vraci hodnotu false, jestlize ulozeni souboru skoncilo chybou
///

bool SaveReservationsStructure(string &path, vector <Reservation> &reservations)
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

///
/// @brief Funkce, ktera prevede data ze stringu do struktury Reservation
/// @param	row		String, ktery obsahuje radek v CSV souboru, ktery se ma prevest
/// @return	Funkce vraci strukturu Reservation s prevedenymi daty
///

Reservation ParserReservation(string row)
{
	string s;
	Reservation reservation;
	istringstream line(row);

	getline(line, s, ';');
	reservation.id = atoi(s.c_str());	

	getline(line, s, '.');
	reservation.day = atoi(s.c_str());	

	getline(line, s, '.');
	reservation.month = atoi(s.c_str());	

	getline(line, s, '.');
	reservation.year = atoi(s.c_str());

	return reservation;
}

///
/// @brief	Funkce, ktera zkontroluje integritu mezi daty v aplikaci a CSV souboru
/// @param	path			Cesta k souboru, ktery obsahuje seznam rezervaci
/// @param	reservations	Vektor s rezervacemi, ktery se ma naplnit
/// @retval	true			Funkce vraci hodnotu true, jestlize jsou data v souboru stejna jako v aplikaci
/// @retval	false			Funkce vraci hodnotu false, jestlize nastal problem pri kontrole
///

bool CheckReservationsIntegrity(string &path, vector <Reservation> &reservations)
{
	ifstream file;
	bool passed = true;
	string s;
	Reservation reservation;
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

	if (reservations.size() == file_records)
	{
		unsigned int position = 0;
		file.clear();
		file.seekg(0, ios::beg);
		getline(file, s);

		while (getline(file, s))
		{
			reservation = ParserReservation(s);

			if (!ReservationComparator(reservation, reservations[position]))
			{
				cout << (passed ? CHECKINTEGRITY_ERROR : "");
				cout << CHECKRESERVATIONINTEGRITY_DETAIL(position) << endl;
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
		cout << CHECKINTEGRITY_ADD;
		if (YesNoCheck())
		{
			reservations.clear();
			FillReservationsStructure(path, reservations);
			cout << CHECKINTEGRITY_SUCCESS << endl;
		}
		else
		{
			SaveReservationsStructure(path, reservations);
		}
	}
	return true;
}

///
/// @brief Funkce pro vypsani obsahu struktury Reservation z vektoru, ktery byl naplnen CSV souborem
/// @param	reservations	Vektor s ulozenymi rezervacemi
///

void PrintReservations(vector <Reservation> &reservations)
{
	GetTableSeparator(27);
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
	GetTableSeparator(27);
}

///
/// @brief	Funkce pro naplneni struktury Reservation z CSV souboru
/// @param	path			Cesta k souboru, ktery obsahuje seznam rezervaci
/// @param	reservations	Vektor s rezervacemi, ktery se ma naplnit
/// @retval	true			Funkce vraci hodnotu true, jestlize struktura byla uspesne naplnena
/// @retval	false			Funkce vraci hodnotu false, jestlize nastal problem pri nahravani dat ze souboru
///

bool FillReservationsStructure(string &path, vector <Reservation> &reservations)
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
		if (s.find('"') != string::npos)
			return false;
		reservation = ParserReservation(s);

		if (!IsReservationValid(reservation))
		{
			return false;
		}
		reservations.push_back(reservation);
	}
	reservations_file.close();
	return true;
}

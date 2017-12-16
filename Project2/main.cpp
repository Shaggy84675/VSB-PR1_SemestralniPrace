///
/// @brief Hlavni soubor, ktery je potreba ke spusteni programu
/// @author Patrik Leifert
/// @file main.cpp
/// @date Prosinec 2017
///
#include <iostream>

#include "message.h"
#include "room.h"
#include "reservation.h"
#include "helper.h"

int main(int argc, char* argv[])
{
	string rooms_path;
	string reservations_path;
	vector <struct Room> rooms;
	vector <struct Reservation> reservations;

	if (argc != 3)
	{
		cout << INP_PARAMETERS_ERR << endl;
		cout << PRESS_ANY_KEY << endl;
		getchar();
		return 0;
	}

	rooms_path = argv[1];
	reservations_path = argv[2];

	//string rooms_path = "D:/Desktop/Semestrální práce - Programování 1/vstupnidata/seznam_mistnosti.csv";
	//string reservations_path = "D:/Desktop/Semestrální práce - Programování 1/vstupnidata/seznam_rezervaci.csv";
	//string rooms_path = "seznam_mistnosti.csv";
	//string reservations_path = "seznam_rezervaci.csv";

	WelcomeMessage();

	if (!FillRoomsStructure(rooms_path, rooms))
	{
		cout << TABLE_LOAD_ERR("mistnosti") << endl;
		cout << PRESS_ANY_KEY << endl;
		getchar();
		return 0;
	}

	if (!FillReservationsStructure(reservations_path, reservations))
	{
		cout << TABLE_LOAD_ERR("rezervaci") << endl;
		cout << PRESS_ANY_KEY << endl;
		getchar();
		return 0;
	}

	while (true)
	{
		switch (ShowMainMenu())
		{
		case EXIT_APPLICATION:
			return 0;
			break;
		case MAKE_RESERVATION:
			PrintRoomsTable(rooms);
			if (MakeReservation(reservations_path, rooms, reservations))
			{
				cout << CREATERESERVATION_SUCCESS << endl;
			}
			cout << endl;
			break;
		case CANCEL_RESERVATION:
			PrintReservationsTable(rooms, reservations);
			if (CancelReservation(reservations_path, rooms, reservations))
			{
				cout << CANCELRESERVATION_SUCCESS << endl;
			}
			cout << endl;
			break;
		case SEARCH_FREE_ROOMS:
			SubMenuSelection(rooms, reservations);
			break;
		case SHOW_ROOMS_TABLE:
			CheckRoomsIntegrity(rooms_path, rooms);
			PrintRoomsTable(rooms);
			cout << endl;
			break;
		case SHOW_RESERVATIONS_TABLE:
			CheckReservationsIntegrity(reservations_path, reservations);
			PrintReservations(reservations);
			cout << endl;
			break;
		case ADD_ROOM:
			if (AddNewRoom(rooms_path, rooms))
			{
				cout << ADDROOM_SUCCESS << endl;
				cout << endl;
				PrintRoomsTable(rooms);
			}
			cout << endl;
			break;
		case REMOVE_ROOM:
		{
			bool remove = RemoveRoom(rooms_path, reservations_path, rooms, reservations);			
			cout << endl;
			if (remove)
			{
				PrintRoomsTable(rooms);
			}
		} break;
		case 8:
			//CheckRoomsIntegrity(rooms_path, rooms);
			CheckReservationsIntegrity(reservations_path, reservations);
			//cout << CheckIDDuplicity(100, rooms);
			//cout << FindRoomIndex(304, rooms);
			//PrintFreeRooms(rooms, reservations, Filter::PRICE);
			cout << endl;
			break;
		}
	}

	return 0;
}

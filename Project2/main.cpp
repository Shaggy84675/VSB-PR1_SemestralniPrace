#include <iostream>

#include "message.h"
#include "room.h"
#include "reservation.h"
#include "helper.h"

int main()
{
	vector <struct Room> rooms;
	vector <struct Reservation> reservations;

	string rooms_path = "D:/Desktop/Semestrální práce - Programování 1/vstupnidata/seznam_mistnosti.csv";
	string reservations_path = "D:/Desktop/Semestrální práce - Programování 1/vstupnidata/seznam_rezervaci.csv";
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
		switch (ShowOptions())
		{
		case 0:
			return 0;
			break;
		case 1:
			PrintRoomsTable(rooms);
			if (MakeReservation(reservations_path, rooms, reservations))
			{
				cout << CREATERESERVATION_SUCCESS << endl;
			}
			cout << endl;
			break;
		case 2:
			PrintReservationsTable(rooms, reservations);
			if (CancelReservation(reservations_path, rooms, reservations))
			{
				cout << CANCELRESERVATION_SUCCESS << endl;
			}
			cout << endl;
			break;
		case 3:
			//FindFreeRooms();
			break;
		case 4:
			PrintRoomsTable(rooms);
			cout << endl;
			break;
		case 5:
			PrintReservationsTable(reservations);
			cout << endl;
			break;
		case 6:
			if (AddNewRoom(rooms_path, rooms))
			{
				cout << ADDROOM_SUCCESS << endl;
				cout << endl;
				PrintRoomsTable(rooms);
			}
			cout << endl;
			break;
		case 7:
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
			//CheckReservationsIntegrity(reservations_path, reservations);
			//cout << CheckIDDuplicity(100, rooms);
			//cout << FindRoomIndex(304, rooms);
			PrintFreeRooms(999, rooms, reservations, Filter::PRICE);
			cout << endl;
			break;
		}
	}

	return 0;
}

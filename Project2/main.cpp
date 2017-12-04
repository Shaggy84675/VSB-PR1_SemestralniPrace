#include <iostream>
#include <thread>
#include <chrono>
#include <cctype>

#include "messages.h"
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


	//getline(cin, path);

	//replace(path.begin(), path.end(), '\\', '/');
	//path.erase(remove(path.begin(), path.end(), '"'), path.end());

	/*for (int index = 0; index < s.length(); ++index)
		{
			cout << s[index];
			this_thread::sleep_for(chrono::milliseconds(50));
		}
		cout << endl;*/

		//cout << path << endl;

	if (!FillRoomsStructure(rooms_path, rooms))
	{
		cout << TABLE_LOAD_ERR("mistnosti") << endl;
		return 0;
	}

	if (!FillReservationsStructure(reservations_path, reservations))
	{
		cout << TABLE_LOAD_ERR("rezervace") << endl;
		return 0;
	}

	while (true)
	{
		switch (ShowOptions())
		{
		case 0:
			return 0;
			system("PAUSE");
			break;
		case 1:			
			BookRoom(rooms, reservations);
			cout << endl;
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
			AddNewRoom(rooms_path, rooms);
			cout << endl;
			PrintRoomsTable(rooms);
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
			PrintReservationsTable(rooms, reservations);
			//PrintFreeRooms(rooms, reservations);
			//cout << CheckIDDuplicity(100, rooms);
			//cout << FindRoomIndex(304, rooms);

			break;
		}
	}

	return 0;
}

#include "helper.h"
#include "message.h"

#include <iostream>


bool IsDateValid(short day, short month, short year)
{
	if ((day > RESERVATION_DAY_MAX_LENGTH || day < RESERVATION_DAY_MIN_LENGTH) ||
		(month > RESERVATION_MONTH_MAX_LENGTH || month < RESERVATION_MONTH_MIN_LENGTH) ||
		(year < RESERVATION_YEAR_MIN_LENGTH || year > RESERVATION_YEAR_MAX_LENGTH))
	{
		return false;
	}
	return true;
}

string GetMonthName(int month)
{
	switch (month)
	{
	case 1:
		return "leden";
	case 2:
		return "unor";
	case 3:
		return "brezen";
	case 4:
		return "duben";
	case 5:
		return "kveten";
	case 6:
		return "cerven";
	case 7:
		return "cervenec";
	case 8:
		return "srpen";
	case 9:
		return "zari";
	case 10:
		return "rijen";
	case 11:
		return "listopad";
	case 12:
		return "prosinec";
	default:
		return "";
		break;
	}
}


bool YesNoCheck()
{
	char input;

	while (cin >> input)
	{
		input = tolower(input);
		cin.clear();

		if (input == 'a' && cin.peek() == '\n')
		{
			return true;
		}

		if (input == 'n' && cin.peek() == '\n')
		{
			return false;
		}

		while (cin.get() != '\n');

		cout << YES_NO_INP_ERR << endl;
	}
	return false;
}

int ShowMainMenu()
{
	int option;
	PrintMainMenu();

	while (!(cin >> option) || (option > NUMBER_OF_OPTIONS || option < 0)) {
		cin.clear();

		while (cin.get() != '\n')
		{
			continue;
		}

		cout << endl << INP_ERR << endl;
		PrintMainMenu();
	}
	return option;
}

char ShowSubMenu()
{
	char option;
	PrintSubMenu();

	while (!(cin >> option) || (option > LAST_OPTION || option < FIRST_OPTION)) {
		cin.clear();

		while (cin.get() != '\n')
		{
			continue;
		}

		cout << endl << INP_ERR << endl;
		PrintSubMenu();
	}

	return option;
}


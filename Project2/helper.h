#pragma once

#include "room.h"
#include "reservation.h"

#include <vector>
#include <string>

#define NUMBER_OF_OPTIONS 8
#define FIRST_OPTION 'a'
#define LAST_OPTION 'e'
#define QUIT_OPTION 'q'
#define CURRENCY "CZK"

#define GET_INPUT(var, msg1, msg2) \
while (!(cin >> var)) \
{ \
	cin.clear(); \
	while (cin.get() != '\n') { continue; } \
	cout << msg1 << endl; \
	cout << msg2; \
}

using namespace std;

string GetMonthName(int month);
bool IsDateValid(short day, short month, short year);

int ShowMainMenu();
bool YesNoCheck();
char ShowSubMenu();

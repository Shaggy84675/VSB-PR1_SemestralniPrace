///
/// @brief Hlavickovy soubor, ktery obsahuje obecne deklarace pro praci s programem
/// @author Patrik Leifert
/// @file helper.h
/// @date Prosinec 2017
///

#pragma once

#include "room.h"
#include "reservation.h"

#include <vector>
#include <string>
#include <fstream>

#define NUMBER_OF_OPTIONS 8
#define FIRST_OPTION 'a'
#define LAST_OPTION 'g'
#define QUIT_OPTION 'q'
#define CURRENCY "CZK"

#define EXIT_APPLICATION		0
#define	MAKE_RESERVATION		1
#define CANCEL_RESERVATION		2
#define SEARCH_FREE_ROOMS		3
#define SHOW_ROOMS_TABLE		4
#define SHOW_RESERVATIONS_TABLE	5
#define ADD_ROOM				6
#define REMOVE_ROOM				7



#define GET_INPUT(var, msg1, msg2, min_value, max_value) \
while (!(cin >> var) || (var > max_value || var < min_value)) \
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
void GenerateHTMLHeader(ofstream &out);
void GenerateHTMLFooter(ofstream &out);
void GetTableSeparator(const int size);

///
/// @brief Soubor obsahuje obecne funkce pro praci s programem
/// @author Patrik Leifert
/// @file helper.cpp
/// @date Prosinec 2017
///

#include "helper.h"
#include "message.h"

#include <iostream>
#include <iomanip>

///
/// @brief Funkce, ktera oddeluje casti tabulky
/// @param size	Obsahuje cislo, ktere urcuje delku oddelovace
///

void GetTableSeparator(const int size)
{
	cout << "+" << setw(size) << setfill('-') << right << "+" << endl;
}

///
/// @brief Funkce, ktera generuje paticku pro HTML soubor
///

void GenerateHTMLFooter(ofstream &out)
{
	out << "<p>Vygenerovano programem: Rezervace mistnosti</p></html>";
}

///
/// @brief Funkce, ktera generuje hlavicku pro HTML soubor
///

void GenerateHTMLHeader(ofstream &out)
{
	out << "<!DOCTYPE html><html><head><title>Rezervace mistnosti</title> \
		<style>body{font-family: Segoe UI Light;} \
		table{border-collapse: collapse; width: 100%;} \
		th{font-family: Segoe UI; font-weight: bold;} \
		td, th{border: 1px solid #808080; padding: 8px;} \
		tr:nth-child(even) {background-color: #ABABAB;}</style></head>" << endl;
}

///
/// @brief Funkce, ktera zjistuje, jestli je datum validni
/// @param day		Den
/// @param month	Mesic
/// @param year		Rok
/// @retval true	Funkce vraci hodnotu true, jestlize je datum ve spravnem formatu
/// @retval false	Funkce vraci hodnotu false, jestlize je datum ve spatnem formatu
///

bool IsDateValid(short day, short month, short year)
{
	if ((day > RESERVATION_DAY_MAX_VALUE || day < RESERVATION_DAY_MIN_VALUE) ||
		(month > RESERVATION_MONTH_MAX_VALUE || month < RESERVATION_MONTH_MIN_VALUE) ||
		(year < RESERVATION_YEAR_MIN_VALUE || year > RESERVATION_YEAR_MAX_VALUE))
	{
		return false;
	}
	return true;
}

///
/// @brief Funkce, ktera vraci jmeno mesice na zaklade cisla mesice
/// @return Funkce vraci nazev mesice
///

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

///
/// @brief Funkce, pro vybrani moznosti ano nebo ne
/// @retval true	Funkce vraci hodnotu true, jestlize bylo zadano na vstupu pismeno a
/// @retval false	Funkce vraci hodnotu false, jestlize bylo zadano na vstupu pismeno n
///

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

///
/// @brief Funkce, pro vybrani moznosti v hlavni nabidce
/// @return Funkce vraci pismeno s vybranou moznosti
///

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

///
/// @brief Funkce, pro vybrani moznosti v podmenu vyhledavani mistnosti
/// @return Funkce vraci pismeno s vybranou moznosti
///

char ShowSubMenu()
{
	char option;
	PrintSubMenu();

	while (!(cin >> option) || (option > LAST_OPTION || option < FIRST_OPTION)) {
		cin.clear();

		if (option == QUIT_OPTION)
		{
			break;
		}

		while (cin.get() != '\n')
		{
			continue;
		}

		cout << endl << INP_ERR << endl;
		PrintSubMenu();
	}

	return option;
}

#include "message.h"

#include <iostream>
#include <iomanip>


using namespace std;

void PrintSubMenu()
{
	cout << "a - Vyhledat volne mistnosti na urcity datum" << endl
		<< "b - Vyhledat mistnosti do urcite ceny" << endl
		<< "c - Vyhledat mistnosti do urcite kapacity osob" << endl
		<< "d - Vyhledat mistnosti nad urcitou kapacitu osob" << endl
		<< "e - Vyhledat mistnosti na konkretnim patre" << endl
		<< "f - Vyhledat vsechny volne mistnosti" << endl
		<< "q - Vratit se do hlavni nabidky" << endl
		<< setw(74) << setfill('-') << "" << endl
		<< "Vyberte moznost ze seznamu, kterou chcete provest: ";
}

void PrintMainMenu()
{
	cout << "1 - Rezervovat mistnost" << endl
		<< "2 - Zrusit rezervaci" << endl
		<< "3 - Vyhledat volne mistnosti" << endl
		<< "4 - Zobrazit tabulku mistnosti" << endl
		<< "5 - Zobrazit tabulku rezervaci" << endl
		<< "6 - Pridat novou mistnost" << endl
		<< "7 - Odstranit stavajici mistnost" << endl
		<< "0 - Ukoncit program" << endl
		<< setw(74) << setfill('-') << "" << endl
		<< "Vyberte moznost ze seznamu, kterou chcete provest: ";
}

void WelcomeMessage()
{
	cout << "+" << setw(78) << setfill('-') << "+" << endl
		<< "[" << setw(55) << setfill(' ') << "Aplikace pro rezervaci mistnosti" << setw(23) << setfill(' ') << "]" << endl
		<< "[" << setw(49) << setfill(' ') << "Patrik Leifert (2017)" << setw(29) << setfill(' ') << "]" << endl
		<< "+" << setw(78) << setfill('-') << "+" << endl << endl;
}
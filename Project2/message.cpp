#include "message.h"

#include <iostream>
#include <iomanip>


using namespace std;

void PrintOptions2()
{
	cout << "a - Vyhledat volne mistnosti na urcity datum" << endl
		<< "b - Vyhledat volne mistnosti do urcite ceny" << endl
		<< "c - Vyhledat volne mistnosti do urcite kapacity osob" << endl
		<< "d - Vyhledat volne mistnosti na konkretnim patre" << endl
		<< "q - Vratit se do hlavni nabidky" << endl
		<< setw(74) << setfill('-') << "" << endl
		<< "Vyberte moznost ze seznamu, kterou chcete provest: ";
}

void PrintOptions()
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
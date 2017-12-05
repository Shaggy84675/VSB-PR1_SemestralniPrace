#include "messages.h"

#include <iostream>
#include <iomanip>


using namespace std;

void PrintOptions()
{
	cout << "1 - Rezervovat mistnost" << endl
		<< "2 - Vyhledat volne mistnosti (NYI)" << endl
		<< "3 - Exportovat data do HTML (NYI)" << endl
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
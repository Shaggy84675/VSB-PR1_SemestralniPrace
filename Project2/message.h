///
/// @brief Hlavickovy soubor, ktery obsahuje seznam vsech hlasek, ktere se vyskytuji v programu a deklarace pro praci s programem
/// @author Patrik Leifert
/// @file message.h
/// @date Prosinec 2017
///

#pragma once

#define INP_PARAMETERS_ERR								"Neco se nepovedlo :( \nNebyly zadany spravne vstupni parametry \nJe potreba zadat spravnou cestu k \nCSV souborum se seznamem mistnosti a rezervaci\n" \
														"Spravne pouziti: \nsemestralniPrace.exe	\"seznam_mistnosti.csv\" \"seznam_rezervace.csv\""

#define ERROR_FILE_NOT_FOUND(filename)					"Neco se nepovedlo :( \nMame problemy nalezt potrebny soubor " << filename << \
														" umisteny v adresari vstupnidata. \nProsim zkontrolujte dostupnost souboru a zkuste to znovu."
#define TABLE_LOAD_ERR(tablename)						"Neco se nepovedlo :( \nMame problemy nacist data ze souboru " << tablename << "."
#define YES_NO_INP_ERR									"Zadana moznost musi byt typu A - ano, N - ne "

#define INP_ERR											"Byla zadana chybna moznost. \nProsim zadejte nasledujici moznosti: "
#define INP_DATE_INVALID								"Bylo zadano spatne datum"

#define PRESS_ANY_KEY									"Stisknete Enter pro ukonceni programu..."

#define	FILE_INP										"Zadejte nazev souboru (bez pripony): "
#define FILE_INP_ERR									"Neco se nepovedlo :( \nNastala chyba pri zadavani nazvu souboru."

//Zpravy pro praci s mistnostmi

#define ROOM_INP_ERR(inpmin, inpmax)					"Zadano chybne cislo mistnosti \nCislo mistnosti musi byt cele cislo od " << inpmin << " do " << inpmax
#define ROOM_NOT_FOUND_ERR								"Pozadovanou mistnost se nepodarilo nalezt"
#define ROOM_INDEX_NOT_FOUND							"Index mistnosti se nepodarilo nalezt"
#define ROOM_NOT_RESERVED								"Zadana mistnost neni na tento den rezervovana"

//

#define ADDROOM_INPUT									"Zadejte cislo mistnosti, kterou chcete pridat: " 
#define ADDROOM_ROOMNUM_EXISTS(room)					"Mistnost " << room << " se jiz v seznamu nachazi"

#define ADDROOM_INP_FLOOR								"Zadejte patro mistnosti: "
#define ADDROOM_INP_FLOOR_ERR(inpmin, inpmax)			"Zadano chybne patro mistnosti \nPatro musi byt v cele cislo v rozmezi od " << inpmin << " do " << inpmax

#define ADDROOM_INP_SEATS								"Zadejte kapacitu sedadel teto mistnosti: "
#define ADDROOM_INP_SEATS_ERR(inpmax)					"Zadana chybne kapacita sedadel \nKapacita sedadel musi byt kladne cele cislo do " << inpmax

#define ADDROOM_INP_PRICE								"Zadejte cenu za den rezervace mistnosti: "
#define ADDROOM_INP_PRICE_ERR(inpmax)					"Zadana chybne cena mistnosti \nCena mistnosti musi byt kladne cele cislo do " << inpmax 

#define ADDROOM_INP_CONFIRM								"Prejete si nasledujici mistnost pridat do databaze? (A/N): "
#define ADDROOM_CANCELED								"Pridani mistnosti bylo zruseno"
#define ADDROOM_SUCCESS									"Mistnost byla uspesne pridana do seznamu mistnosti"


#define REMOVEROOM_SELECT								"Zadejte kriterium, podle ktereho si prejete mistnost odstranit: \na) ID mistnosti \nb) Cislo mistnosti"
#define REMOVEROOM_INPUT								"Zadejte cislo mistnosti, kterou chcete odstranit: "
#define REMOVEROOM_INP_CONFIRM(room)					"Prejete si nasledujici mistnost " << room << " odstranit z databaze? (A/N): "
#define REMOVEROOM_CANCELED								"Odstraneni mistnosti bylo zruseno"

#define CREATERESERVATION_INP_ROOMNUM					"Zadejte cislo mistnosti, kterou si prejete rezervovat: "
#define CREATERESERVATION_NOT_AVAILABLE					"Zadana mistnost je jiz bohuzel na dany den obsazena"
#define CREATERESERVATION_INP_DATE						"Zadejte datum rezervace ve formatu (DD.MM.YYYY) nebo (D.M.YYYY): "
#define CREATERESERVATION_INP_CONFIRM(room)				"Prejete si mistnost " << room << " rezervovat? (A/N): "
#define CREATERESERVATION_SUCCESS						"Mistnost byla uspesne zarezervovana"
#define CREATERESERVATION_CANCELED						"Rezervace mistnosti bylo zruseno"

#define CANCELRESERVATION_INP_ROOMNUM					"Zadejte cislo mistnosti, pro kterou si prejete zrusit rezervaci: "
#define CANCELRESERVATION_INP_DATE						"Zadejte datum zruseni rezervace ve formatu (DD.MM.YYYY) nebo (D.M.YYYY): "
#define CANCELRESERVATION_INP_CONFIRM(room)				"Prejete si zrusit rezervaci mistnosti " << room << "? (A/N): "
#define CANCELRESERVATION_SUCCESS						"Rezervace mistnosti byla uspesne zrusena"
#define CANCELRESERVATION_CANCELED						"Zruseni rezervace mistnosti bylo zruseno"

//Kontrola integrity

#define CHECKINTEGRITY_ERROR							"Neco se nepovedlo :( \nNesouhlasi data ze souboru s daty v programu. \nPravdepodobne doslo ke zmene dat pri behu programu. \n"
#define CHECKINTEGRITY_ADD								"Prejete si upravena data v souboru nahrat do programu a prepsat? (A/N) \nV pripade ze zvolite ne, budou data v souboru prepsana zpet: "
#define CHECKINTEGRITY_SUCCESS							"Data ze souboru byly uspesne nahrany do programu "

#define CHECKROOMINTEGRITY_DETAIL(row)					"Radek v souboru se seznamem mistnosti: " << row
#define CHECKRESERVATIONINTEGRITY_DETAIL(row)			"Radek v souboru se seznamem rezervaci: " << row


#define SELECTFREEROOMS_FLOOR_INP						"Zadejte patro na kterem si prejete volne mistnosti vyhledat: "
#define SELECTFREEROOMS_DATE_INP						"Zadejte datum ve formatu (DD.MM.YYYY) nebo (D.M.YYYY) na ktere si prejete volne mistnosti vyhledat: "
#define SELECTFREEROOMS_PRICE_LOW_INP					"Zadejte castku do ktere si prejete volne mistnosti vyhledat: "
#define SELECTFREEROOMS_SEATS_INP_ON					"Zadejte pocet sedadel do kterych si prejete volne mistnosti vyhledat: "
#define SELECTFREEROOMS_SEATS_INP_OVER					"Zadejte pocet sedadel nad ktere si prejete volne mistnosti vyhledat: "

#define	TABLECELL_ID									"ID"
#define TABLECELL_RESERVEDATE							"Datum rezervace"
#define TABLECELL_ROOM									"Mistnost"
#define TABLECELL_FLOOR									"Patro"
#define TABLECELL_SEATS									"Kapacita sedadel"
#define TABLECELL_PRICE									"Cena rezervace"

#define ROOM_DESCRIPTION								"Cislo mistnosti: "
#define FLOOR_DESCRIPTION								"Patro: "
#define SEATS_DESCRIPTION								"Kapacita sedadel: "
#define PRICE_DESCRIPTION								"Cena za den rezervace: "

#define HTMLEXPORT_CONFIRM								"Prejete si nasledujici tabulku vyexportovat do HTML? (A/N): "

void WelcomeMessage();
void PrintMainMenu();
void PrintSubMenu();
#pragma once

#define ERROR_FILE_NOT_FOUND(filename)					"Neco se nepovedlo :( \nMame problemy nalezt potrebny soubor " << filename << \
														" umisteny v adresari vstupnidata. \nProsim zkontrolujte dostupnost souboru a zkuste to znovu."

#define TABLE_LOAD_ERR(tablename)						"Neco se nepovedlo :( \nNepodarilo se nacist data tabulky " << tablename << "."
#define YES_NO_INP_ERR									"Zadana moznost musi byt typu A - ano, N - ne "

#define INP_ERR											"Byla zadana chybna moznost. \nProsim zadejte nasledujici moznosti: "
#define INP_DATE_INVALID								"Bylo zadano spatne datum."

//Zpravy pro praci s mistnostmi

#define ROOM_INP_ERR(inpmin, inpmax)					"Zadano chybne cislo mistnosti \nCislo mistnosti musi byt cele cislo od " << inpmin << " do " << inpmax
#define ROOM_NOT_FOUND_ERR								"Pozadovanou mistnost se nepodarilo nalezt "

#define ADDROOM_INPUT									"Zadejte cislo mistnosti, kterou chcete pridat: " 

#define ADDROOM_INP_FLOOR								"Zadejte patro mistnosti: "
#define ADDROOM_INP_FLOOR_ERR(inpmin, inpmax)			"Zadano chybne patro mistnosti \nPatro musi byt v cele cislo v rozmezi od " << inpmin << " do " << inpmax

#define ADDROOM_INP_SEATS								"Zadejte kapacitu sedadel teto mistnosti: "
#define ADDROOM_INP_SEATS_ERR(inpmax)					"Zadana chybne kapacita sedadel \nKapacita sedadel musi byt kladne cele cislo do " << inpmax

#define ADDROOM_INP_PRICE								"Zadejte cenu za den rezervace mistnosti: "
#define ADDROOM_INP_PRICE_ERR(inpmax)					"Zadana chybne cena mistnosti \nCena mistnosti musi byt kladne cele cislo do " << inpmax 

#define ADDROOM_INP_CONFIRM								"Prejete si nasledujici mistnost pridat do databaze? (A/N) "				

#define REMOVEROOM_SELECT								"Zadejte kriterium, podle ktereho si prejete mistnost odstranit: \na) ID mistnosti \nb) Cislo mistnosti"
#define REMOVEROOM_INPUT								"Zadejte cislo mistnosti, kterou chcete odstranit: "



#define CREATERESERVATION_INP_ROOMNUM					"Zadejte cislo mistnosti, kterou si prejete rezervovat: "
#define CREATERESERVATION_NOT_AVAILABLE					"Zadana mistnost je jiz bohuzel na dany den obsazena"
#define CREATERESERVATION_INP_DATE						"Zadejte datum rezervace ve formatu (DD.MM.YYYY) nebo (D.M.YYYY): "
#define CREATERESERVATION_INP_CONFIRM(room)				"Prejete si mistnost " << room << " rezervovat? (A/N) "
#define CREATERESERVATION_SUCCESS						"Mistnost byla uspesne zarezervovana"
#define CREATERESERVATION_CANCELED						"Rezervace mistnosti bylo zruseno"

#define CANCELRESERVATION_INP_ROOMNUM					"Zadejte cislo mistnosti, pro kterou si prejete zrusit rezervaci: "
#define CANCELRESERVATION_NOT_RESERVED					"Zadana mistnost neni na tento den rezervovana"
#define CANCELRESERVATION_INP_DATE						"Zadejte datum zruseni rezervace ve formatu (DD.MM.YYYY) nebo (D.M.YYYY): "
#define CANCELRESERVATION_INP_CONFIRM(room)				"Prejete si zrusit rezervaci mistnosti " << room << " ? (A/N) "
#define CANCELRESERVATION_SUCCESS						"Rezervace mistnosti byla uspesne zrusena"
#define CANCELRESERVATION_CANCELED						"Zruseni rezervace mistnosti bylo zruseno"


#define CHECKINTEGRITY_ERROR							"Neco se nepovedlo :( \nNekonzistentni data. Zkontrolujte obsah vstupnich dat a akci opakujte."

#define CHECKROOMINTEGRITY_ERROR						"Neco se nepovedlo :( \nNesouhlasi data ze souboru s daty v programu. \nPravdepodobne doslo ke zmene dat pri behu programu. \n"
#define CHECKROOMINTEGRITY_DETAIL(row, column)			"Radek v souboru seznam_mistnosti.csv: " << row << " polozka: " << column
#define CHECKROOMINTEGRITY_ADD							"Prejete si upravena data v souboru nahrat do programu a prepsat? (A/N) \nV pripade ze zvolite ne, budou data v souboru prepsana zpet: "
#define CHECKROOMINTEGRITY_SUCCESS						"Data ze souboru byly uspesne nahrany do programu "

#define CHECKRESERVATIONINTEGRITY_DETAIL(row, column)	"Radek v souboru seznam_rezervaci.csv: " << row << " polozka: " << column



void WelcomeMessage();
void PrintOptions();
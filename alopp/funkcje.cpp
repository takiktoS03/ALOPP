/**
 @file funkcje.cpp
*/

// definicje (implementacje) funkcji

#define _USE_MATH_DEFINES

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <cmath>
#include <memory>

#include "klasy.h"
#include "funkcje.h"

std::vector<std::shared_ptr<element>> odczyt_wejscia(const std::string& nazwa_pliku)
{
	std::ifstream plik (nazwa_pliku);
	std::string linia;
	char typ;
	int wez_p, wez_k;
	double wartosc, fi, frq;
	std::vector<double> freq;
	std::vector<std::shared_ptr<element>> elementy;
	if (plik)
	{
		while (getline(plik, linia))
		{
			std::stringstream ss;
			ss << linia;
			ss >> typ >> wez_p >> wez_k >> wartosc;
			{
				typ = std::toupper(typ);
				std::pair<int, int> miejsce{ wez_p, wez_k };
				if (typ == 'E' || typ == 'I')
				{
					ss >> fi >> frq;
					freq.push_back(frq);
					for (auto& i : freq)
					{
						if (frq != i)
						{
							std::cout << "Zrodla nie moga miec tej roznej czestotliwosci w obwodzie!\n";
							return elementy;
						}
					}
					if (typ == 'E')
						elementy.push_back(std::shared_ptr<element>(new E(typ, miejsce, wartosc, fi, frq)));
					else
						elementy.push_back(std::shared_ptr<element>(new I(typ, miejsce, wartosc, fi, frq)));
				}
				if (typ == 'R')
					elementy.push_back(std::shared_ptr<element>(new R(typ, miejsce, wartosc)));
				else if (typ == 'C')
					elementy.push_back(std::shared_ptr<element>(new C(typ, miejsce, wartosc)));
				else if (typ == 'L')
					elementy.push_back(std::shared_ptr<element>(new L(typ, miejsce, wartosc)));
			}
		}
	}
	return elementy;
}

double czestotliwosc(const std::vector<std::shared_ptr<element>>& elementy)
{
	for (int i = 0; i < elementy.size(); i++)
	{
		if (elementy[i]->typ == 'E' || elementy[i]->typ == 'I')
			if (elementy[E]->
	}
	return 0;
}

void wypisz_elementy(const std::vector<std::shared_ptr<element>>& elementy)
{
	std::cout << std::endl;
	for (auto& i : elementy)
	{
		std::cout << i->typ << " " << i->umiejscowienie.first << " " << i->umiejscowienie.second << " " << i->wartosc << " " << i->fi << std::endl;
	}
}

std::set<int> wektor_wezlow(const std::vector<std::shared_ptr<element>>& elementy)
{
	std::set<int> wezly;
	for (auto& el : elementy)
	{
		wezly.insert(el->umiejscowienie.first);
		wezly.insert(el->umiejscowienie.second);
	}	
	return wezly;
}

std::vector<std::shared_ptr<element>> dodaj_rez_obok_sem(std::vector<std::shared_ptr<element>>& elementy, const std::set<int>& wezly)
{
	int max = 0;
	for (auto& i : wezly)
	{
		if (i > max)
		{
			max = i;
		}
	}
	int wolny_wezel = max;
	for (int i = 0; i < elementy.size(); i++)
	{
		if (elementy[i]->typ == 'E')
		{
			wolny_wezel++;
			elementy[i]->umiejscowienie.second = wolny_wezel;
			elementy.push_back(std::shared_ptr<element>(new R('R', { elementy[i]->umiejscowienie.second, wolny_wezel }, -1))); // rezystor -1 dodany na gałąź ze źródłem SEM
		}
	}
	return elementy;
}
 
 
//std::pair<macierz,std::unordered_map<int,int>> coltri(const std::vector<element>& elementy, const std::set<int>& wezly)
//{
//	macierz potencjaly;
//	std::unordered_map<int,int> stare2nowe;
//	int nowywezel = 0;
//	for (auto& i : wezly)
//	{
//		stare2nowe[i] = nowywezel;
//		nowywezel++;
//	}
// 	auto iter = stare2nowe.begin();
//	iter++;  // omijamy uziemiony wezel (zawsze to bedzie najmniejszy wezel znaleziony w pliku wejscia)
// 	for ( ; iter != stare2nowe.end(); iter++)
//	{
//		std::vector<double> wiersz(stare2nowe.size() + 1);
//		for (auto& j : elementy)
//		{
//			if (j.typ == 'R' && j.umiejscowienie.first == iter->first)
//			{
//				wiersz[iter->second] += (1 / j.wartosc);
//				wiersz[stare2nowe[j.umiejscowienie.second]] -= (1 / j.wartosc);
//			}
//			else if (j.typ == 'R' && j.umiejscowienie.second == iter->first)
//			{
//				wiersz[iter->second] += (1 / j.wartosc);
//				wiersz[stare2nowe[j.umiejscowienie.first]] -= (1 / j.wartosc);
//			}
//			else if (j.typ == 'E' && j.umiejscowienie.first == iter->first)
//			{
//				wiersz[iter->second] += 1;
//				wiersz[stare2nowe[j.umiejscowienie.second]] -= 1;
//			}
//			else if (j.typ == 'E' && j.umiejscowienie.second == iter->first)
//			{
//				wiersz[iter->second] += 1;
//				wiersz[stare2nowe[j.umiejscowienie.first]] -= 1;
//			}
//		}
//		for (auto& j : elementy)
//		{
//			if (j.typ == 'E' && j.umiejscowienie.first == iter->first)
//			{
//				wiersz[stare2nowe.size()] += j.wartosc;
//			}
//			else if (j.typ == 'E' && j.umiejscowienie.second == iter->first)
//			{
//				wiersz[stare2nowe.size()] -= j.wartosc;
//			}
//			else if (j.typ == 'I' && j.umiejscowienie.first == iter->first)
//			{
//				wiersz[stare2nowe.size()] += j.wartosc;
//			}
//			else if (j.typ == 'I' && j.umiejscowienie.second == iter->first)
//			{
//				wiersz[stare2nowe.size()] -= j.wartosc;
//			}
//		}
//		wiersz.erase(wiersz.begin());
//		potencjaly.push_back(wiersz);
//		wiersz.clear();
//	}
//	//wypisz(potencjaly);
//	return {potencjaly, stare2nowe};
//}
//
////void wypisz(const macierz& macierz)
////{
////	std::vector<double> wiersz;
////	std::cout << std::endl << macierz[0].size() << " x " << macierz.size() << std::endl;
////	for (auto& i : macierz)
////	{
////		wiersz = i;
////		for (auto& j : wiersz)
////		{
////			std::cout << j << " ";
////		}
////		std::cout << std::endl;
////	}
////}
//
//
//void licz_prady(std::vector<element>& elementy, std::unordered_map<int, double>& potencjaly)  
//{
//	for (int i = 0; i < elementy.size(); i++)
//	{
//		if (elementy[i].typ == 'R')
//		{
//			elementy[i].prad = (potencjaly[elementy[i].umiejscowienie.first] - potencjaly[elementy[i].umiejscowienie.second]) / elementy[i].wartosc;
//		}
//		if (elementy[i].typ == 'R' && elementy[i].wartosc == -1)
//		{
//			elementy[i].prad = (potencjaly[elementy[i].umiejscowienie.second] - potencjaly[elementy[i].umiejscowienie.first]) / elementy[i].wartosc;
//		}
//		if (elementy[i].typ == 'I')
//		{
//			elementy[i].prad = -elementy[i].wartosc; // prad na zrodle SPM jest taki jak jego wartosc
//		}
//	}
//	for (int i = 0; i < elementy.size(); i++)
//	{
//		for (int j = 0; j < elementy.size(); j++)
//		{
//			if (elementy[i].typ == 'E' && elementy[j].typ == 'R' && elementy[j].wartosc == -1 && elementy[i].umiejscowienie.second == elementy[j].umiejscowienie.second)
//			{
//				elementy[i].prad = elementy[j].prad; // prad na zrodle SEM jest taki jak prad do niego doplywajacy (z dodanego rezystora -1)
//				elementy[i].umiejscowienie.second = elementy[j].umiejscowienie.first; // zamiana numerow wezlow spowrotem
//			}
//		}
//	}
//}
//
//void licz_napiecia(std::vector<element>& elementy, std::unordered_map<int, double>& potencjaly)  
//{
//	for (int i = 0; i < elementy.size(); i++)
//	{
//		if (elementy[i].typ == 'R')
//		{
//			elementy[i].napiecie = elementy[i].prad * elementy[i].wartosc;
//		}
//		else if (elementy[i].typ == 'I')
//		{
//			elementy[i].napiecie = potencjaly[elementy[i].umiejscowienie.first] - potencjaly[elementy[i].umiejscowienie.second];
//		}
//		else if (elementy[i].typ == 'E')
//		{
//			elementy[i].napiecie = elementy[i].wartosc; // napiecie na zrodle SEM jest taki jak jego wartosc
//		}
//	}
//}
//
//void licz_moce(std::vector<element>& elementy)
//{
//	for (auto& i : elementy)
//	{
//		i.moc = i.napiecie * i.prad;
//	}
//}
//
//void zapis_wyjscia(const std::string& nazwa_pliku, const std::vector<element>& elementy)
//{
//	std::ofstream plik(nazwa_pliku);
//	if (plik)
//	{
//		double bilans = 0;
//		plik << "Elementy obwodu:" << std::endl << "TYP | WĘZEŁ POCZĄTKOWY | WĘZEŁ KOŃCOWY | WARTOŚĆ | PRĄD | NAPIĘCIE | MOC" << std::endl;
//		for (auto& i : elementy)
//		{
//			if (i.typ == 'R' && i.wartosc == -1)
//				continue;
//			plik << i.typ << " " << i.umiejscowienie.first << " " << i.umiejscowienie.second << " " << i.wartosc << " " << i.prad << "A " << i.napiecie << "V " << i.moc << "W" << std::endl;
//			bilans += i.moc;
//		}
//		plik << "Bilans mocy układu: " << bilans << "W";
//		//std::cout << "Bilans mocy ukladu: " << bilans << "W";
//	}
//}

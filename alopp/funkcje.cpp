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
#include <complex>
#include <iomanip>

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
					if (frq <= 0)
					{
						//std::cout << "Zrodla musza miec czestotliwosc wieksza od zera!\n";
						return elementy;
					}
					freq.push_back(frq);
					for (auto& i : freq)
					{
						if (frq != i)
						{
							//std::cout << "Zrodla nie moga miec tej roznej czestotliwosci w obwodzie!\n";
							return elementy;
						}
					}
					if (typ == 'E')
						elementy.push_back(std::shared_ptr<element>(new E(typ, miejsce, wartosc, fi, frq)));
					else
						elementy.push_back(std::shared_ptr<element>(new I(typ, miejsce, wartosc, fi, frq)));
				}
				else if (typ == 'R')
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

//void wypisz_elementy(const std::vector<std::shared_ptr<element>>& elementy)
//{
//	std::cout << std::endl;
//	for (auto& i : elementy)
//	{
//		std::cout << i->typ << " " << i->umiejscowienie.first << " " << i->umiejscowienie.second << " " << i->admitancja << std::endl;
//	}
//}

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
			elementy.push_back(std::shared_ptr<element>(new R('R', { elementy[i]->umiejscowienie.second, wolny_wezel }, -1))); // rezystor -1 dodany na gałąź ze źródłem SEM
			elementy[i]->umiejscowienie.second = wolny_wezel;
		}
	}
	return elementy;
}
 
 
std::pair<macierz, std::unordered_map<int, int>> coltri(const std::vector<std::shared_ptr<element>>& elementy, const std::set<int>& wezly)
{
	macierz potencjaly;
	std::unordered_map<int,int> stare2nowe;
	int nowywezel = 0;
	for (auto& i : wezly)
	{
		stare2nowe[i] = nowywezel;
		nowywezel++;
	}
 	auto iter = stare2nowe.begin();
	iter++;  // omijamy uziemiony wezel (zawsze to bedzie najmniejszy wezel znaleziony w pliku wejscia)
 	for ( ; iter != stare2nowe.end(); iter++)
	{
		std::vector<std::complex<double>> wiersz(stare2nowe.size() + 1);
		for (auto& j : elementy)
		{
			if ((j->typ == 'R' || j->typ == 'C' || j->typ == 'L') && j->umiejscowienie.first == iter->first)
			{
				wiersz[iter->second] += j->admitancja;
				wiersz[stare2nowe[j->umiejscowienie.second]] -= j->admitancja;
			}
			else if ((j->typ == 'R' || j->typ == 'C' || j->typ == 'L') && j->umiejscowienie.second == iter->first)
			{
				wiersz[iter->second] += j->admitancja;
				wiersz[stare2nowe[j->umiejscowienie.first]] -= j->admitancja;
			}
			else if (j->typ == 'E' && j->umiejscowienie.first == iter->first)
			{
				wiersz[iter->second] += {1, 0};
				wiersz[stare2nowe[j->umiejscowienie.second]] -= {1, 0};
			}
			else if (j->typ == 'E' && j->umiejscowienie.second == iter->first)
			{
				wiersz[iter->second] += {1, 0};
				wiersz[stare2nowe[j->umiejscowienie.first]] -= {1, 0};
			}
		}
		for (auto& j : elementy)
		{
			if ((j->typ == 'E' || j->typ == 'I') && j->umiejscowienie.first == iter->first)
			{
				wiersz[stare2nowe.size()] += j->impedancja;
			}
			else if ((j->typ == 'E' || j->typ == 'I') && j->umiejscowienie.second == iter->first)
			{
				wiersz[stare2nowe.size()] -= j->impedancja;
			}
		}
		wiersz.erase(wiersz.begin());
		potencjaly.push_back(wiersz);
		wiersz.clear();
	}
	//wypisz(potencjaly);
	return {potencjaly, stare2nowe};
}

//void wypisz(const macierz& macierz)
//{
//	std::vector<std::complex<double>> wiersz;
//	std::cout << std::endl << macierz.size() << " x " << macierz[0].size() << std::endl;
//	for (auto& i : macierz)
//	{
//		wiersz = i;
//		for (auto& j : wiersz)
//		{
//			std::cout << j << " ";
//		}
//		std::cout << std::endl;
//	}
//}

void zapis_wyjscia(const std::string& nazwa_pliku, const std::vector<std::shared_ptr<element>>& elementy)
{
	std::ofstream plik(nazwa_pliku);
	if (plik)
	{
		double bilans_czynnej = 0, bilans_biernej = 0;
		plik << "Elementy obwodu:" << std::endl << "TYP | WĘZEŁ POCZĄTKOWY | WĘZEŁ KOŃCOWY | WARTOŚĆ | PRĄD | NAPIĘCIE | CZESTOTLIWOSC REZONANSOWA | MOC CZYNNA | MOC BIERNA" << std::endl;
		for (auto& i : elementy)
		{
			if (i->typ == 'R' && i->wartosc == -1)
				continue;
			plik << std::setprecision(5) << i->typ << " " << i->umiejscowienie.first << " " << i->umiejscowienie.second << " " << i->wartosc << " "
				//<< i->prad << " [A] " << i->napiecie << " [V] "
				 << sqrt(pow(i->prad.real(), 2) + pow(i->prad.imag(), 2)) << "e^j(" << atan(imag(i->prad) / real(i->prad)) * 180 / M_PI << ") [A] "
				 << sqrt(pow(i->napiecie.real(), 2) + pow(i->napiecie.imag(), 2)) << "e^j(" << atan(imag(i->napiecie) / real(i->napiecie)) * 180 / M_PI << ") [V] "
				 << std::setprecision(8) << i->czest_rez << "Hz " << i->moc_czynna << "[W] " << i->moc_bierna << "[VA]" << std::endl;
			bilans_czynnej += i->moc_czynna;
			bilans_biernej += i->moc_bierna;
		}
		plik << "Bilans mocy układu (moc czynna, moc bierna): " << std::setprecision(4) << bilans_czynnej << "[W] " << bilans_biernej << "[VA]";
		//std::cout << "Bilans mocy ukladu: " << bilans << "W";
	}
}
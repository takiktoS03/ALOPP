#pragma once
/**
@file klasy.h
*/

#ifndef KLASY_H
#define KLASY_H

#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <complex>
#include <cmath>


typedef
std::vector<std::vector<std::complex<double>>> macierz;

const double e = std::numeric_limits<double>::min();

/**
@brief Wirtualna klasa bazowa reprezentujaca element i jego wartosci.
@details Klasa przechowuje informacje o elemencie ukladu:
- typ (rezystor, zrodlo SEM, zrodlo SPM, kondensator, cewka)
- umiejscowienie (wezel poczatkowy, wezel koncowy)
- wartosc
- odlozone napiecie
- prad przeplywajacy przez element
- moc wydzielona
Posiada funkcje wirtualne obliczajace impedancje, admitancje, prad, napiecie, czestotliwosc rezonansowa elementu.
*/
class element
{
	public:
		char typ;
		std::pair<int, int> umiejscowienie;
		double wartosc, moc_czynna, moc_bierna, czest_rez;
		std::complex<double> impedancja, admitancja, napiecie, prad;
		/**
		@brief Funkcja wyznaczajaca impedancje elementu
		@param wartosc Wartosc parametru podana z pliku tekstowego
		@param freq Czestotliwosc zrodel w obwodzie
		@return impedancja Zwraca impedancje elementu w postaci zespolonej
		*/
		virtual std::complex<double> wyznacz_Z(double& wartosc, double& freq) = 0;
		/**
		@brief Funkcja wyznaczajaca admitancje elementu
		@param brak parametrow
		@return admitancja Zwraca admitancje elementu w postaci zespolonej
		*/
		virtual std::complex<double> wyznacz_Y() = 0;
		/**
		@brief Funkcja wyznaczajaca prad na galezi elementu
		@details Wykorzystuje do tego obliczone wczesniej potencjaly w kazdym wezle obwodu.
		@param elementy Wektor wszystkich elementow z obwodu
		@param potencjaly Mapa potencjalow w wezlach
		*/
		virtual void wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly) = 0;
		/**
		@brief Funkcja wyznaczajaca napiecie na kazdym elemencie
		@details Wykorzystuje do tego obliczone wczesniej potencjaly w kazdym wezle obwodu.
		@param potencjaly Mapa potencjalow w wezlach
		*/
		virtual void wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly) = 0;
		/**
		@brief Funkcja wyznaczajaca moce dla kazdego elementu
		@details Oblicza moc czynna i bierna dla kazdego elementu. Jesli moc jest dodatnia, to znaczy, ze element pobiera energie. Jesli ujemna, to znaczy, ze ja oddaje.
		Funkcja oblicza bilans mocy na podstawie mocy oddanej lub pobranej dla kazdego elementu.
		@param brak perametrow
		*/
		virtual void wyznacz_moce() = 0;
		/**
		@brief Funkcja wyznaczajaca czestotliwosc rezonansowa dla kazdego elementu
		@param elementy Wektor wszystkich elementow obwodu
		@return freq Czestotliwosc rezonansu kondensatora lub cewki
		*/
		virtual double wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy) = 0;
		/**
		@brief Funkcja zwracajaca skladowa freq dla kazdego elementu
		@param brak parametrow
		@return freq Czestotliwosc zrodel
		*/
		virtual double czestotliwosc() = 0;
};
/**
@brief Klasa reprezentuje zrodla elektromotoryczne (SEM), jest klasa pochodna elementu.
Posiada dodatkowo zmienne double fi oznaczajaca przesuniecie fazowe zrodla oraz double freq oznaczajaca czestotliwosc zrodla.
*/
class E : public element
{
	private:
		double fi, freq;
		/**
		@brief Konstruktor tworzacy zrodlo SPM
		@param typ Typ elementu
		@param miejsce Umiejscowienie elementu (wezel poczatkowy-koncowy)
		@param wartosc Wartosc skuteczna elementu w Voltach
		@param fi Przesuniecie fazowe zrodla
		@param freq Czestotliwosc pracy zrodla
		*/
	public:
		E(char typ, std::pair<int, int> miejsce, double wartosc, double fi, double freq);
		/**
		@brief Funkcja wyznaczajaca impedancje zrodla SEM 
		@details Wykorzystuje do tego przesuniecie fazowe zrodla (na plaszczyznie kartezjanskiej)
		@param wartosc Wartosc skuteczna zrodla
		@param freq Czestotliwosc pracy zrodel
		*/
		std::complex<double> wyznacz_Z(double& wartosc, double& freq);
		/**
		@brief Funkcja wyznaczajaca admitancje elementu
		@param brak parametrow
		@return admitancja Zwraca admitancje elementu w postaci zespolonej
		*/
		std::complex<double> wyznacz_Y();
		/**
		@brief Funkcja wyznaczajaca prad na galezi elementu
		@details Wykorzystuje do tego obliczony prad na poprzednim dodanym rezystorze -1
		@param elementy Wektor wszystkich elementow z obwodu
		@param potencjaly Mapa potencjalow w wezlach
		*/
		void wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly);
		/**
		@brief Funkcja wyznaczajaca napiecie na kazdym elemencie
		@details Wykorzystuje do tego obliczone wczesniej potencjaly w kazdym wezle obwodu.
		@param potencjaly Mapa potencjalow w wezlach
		*/
		void wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly);
		/**
		@brief Funkcja wyznaczajaca moce dla kazdego elementu
		@details Oblicza moc czynna i bierna dla kazdego elementu. Jesli moc jest dodatnia, to znaczy, ze element pobiera energie. Jesli ujemna, to znaczy, ze ja oddaje.
		@param brak parametrow
		*/
		void wyznacz_moce();
		/**
		@brief Funkcja wyznaczajaca czestotliwosc rezonansowa dla kazdego elementu
		@details Dla zrodla elektromotorycznego rezonans nie zachodzi
		@param elementy Wektor wszystkich elementow obwodu
		@return freq Czestotliwosc rezonansu kondensatora lub cewki
		*/
		double wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy);
		/**
		@brief Funkcja zwracajaca skladowa freq dla kazdego elementu
		@param brak parametrow
		@return freq Czestotliwosc zrodel
		*/
		double czestotliwosc();
		/**
		@brief Destruktor elementu
		*/
		~E();
};
/**
@brief Klasa reprezentuje zrodla pradomotoryczne (SPM), jest klasa pochodna elementu.
Posiada dodatkowo zmienne double fi oznaczajaca przesuniecie fazowe zrodla oraz double freq oznaczajaca czestotliwosc zrodla.
*/
class I : public element
{
	private:
		double fi, freq;
		/**
		@brief Konstruktor tworzacy zrodlo SEM
		@param typ Typ elementu
		@param miejsce Umiejscowienie elementu (wezel poczatkowy-koncowy)
		@param wartosc Wartosc skuteczna elementu w Amperach
		@param fi Przesuniecie fazowe zrodla
		@param freq Czestotliwosc pracy zrodla
		*/
	public:
		I(char typ, std::pair<int, int> miejsce, double wartosc, double fi, double freq);
		/**
		@brief Funkcja wyznaczajaca impedancje zrodla SPM
		@details Wykorzystuje do tego przesuniecie fazowe zrodla (na plaszczyznie kartezjanskiej)
		@param wartosc Wartosc skuteczna zrodla
		@param freq Czestotliwosc pracy zrodel
		@return impedancja
		*/
		std::complex<double> wyznacz_Z(double& wartosc, double& freq);
		/**
		@brief Funkcja wyznaczajaca admitancje elementu
		@param brak parametrow
		@return admitancja Zwraca admitancje elementu w postaci zespolonej
		*/
		std::complex<double> wyznacz_Y();
		/**
		@brief Funkcja wyznaczajaca prad na galezi elementu
		@details Prad na galezi z SPM jest taki sam jak jego impedancja z przeciwnym znakiem
		@param elementy Wektor wszystkich elementow z obwodu
		@param potencjaly Mapa potencjalow w wezlach
		*/
		void wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly);
		/**
		@brief Funkcja wyznaczajaca napiecie na kazdym elemencie
		@details Wykorzystuje do tego obliczone wczesniej potencjaly w kazdym wezle obwodu.
		@param potencjaly Mapa potencjalow w wezlach
		*/
		void wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly);
		/**
		@brief Funkcja wyznaczajaca moce dla kazdego elementu
		@details Oblicza moc czynna i bierna dla kazdego elementu. Jesli moc jest dodatnia, to znaczy, ze element pobiera energie. Jesli ujemna, to znaczy, ze ja oddaje.
		@param brak parametrow
		*/
		void wyznacz_moce();
		/**
		@brief Funkcja wyznaczajaca czestotliwosc rezonansowa dla kazdego elementu
		@details Rezonans nie zachodzi na zrodla SPM
		@param elementy Wektor wszystkich elementow obwodu
		@return freq Czestotliwosc rezonansu kondensatora lub cewki
		*/
		double wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy);
		/**
		@brief Funkcja zwracajaca skladowa freq dla kazdego elementu
		@param brak parametrow
		@return freq Czestotliwosc zrodel
		*/
		double czestotliwosc();
		/**
		@brief Destruktor elementu
		*/
		~I();
};
/**
@brief Klasa reprezentuje rezystory, jest klasa pochodna elementu.
*/
class R : public element
{
	public:
		/**
		@brief Konstruktor tworzacy rezystor
		@param typ Typ elementu
		@param miejsce Umiejscowienie elementu (wezel poczatkowy-koncowy)
		@param wartosc Wartosc elementu w Ohmach
		*/
		R(char typ, std::pair<int, int> miejsce, double wartosc);
		/**
		@brief Funkcja wyznaczajaca impedancje rezystora
		@param wartosc Opornosc rezystora
		@param freq Czestotliwosc pracy zrodel
		@return impedancja
		*/
		std::complex<double> wyznacz_Z(double& wartosc, double& freq);
		/**
		@brief Funkcja wyznaczajaca admitancje elementu
		@param brak parametrow
		@return admitancja Zwraca admitancje elementu w postaci zespolonej
		*/
		std::complex<double> wyznacz_Y();
		/**
		@brief Funkcja wyznaczajaca prad na galezi elementu
		@details Wykorzystuje do tego obliczone wczesniej potencjaly w kazdym wezle obwodu i admitancje
		@param elementy Wektor wszystkich elementow z obwodu
		@param potencjaly Mapa potencjalow w wezlach
		*/
		void wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly);
		/**
		@brief Funkcja wyznaczajaca napiecie na kazdym elemencie
		@details Wykorzystuje do tego obliczone wczesniej potencjaly w kazdym wezle obwodu.
		@param potencjaly Mapa potencjalow w wezlach
		*/
		void wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly);
		/**
		@brief Funkcja wyznaczajaca moce dla kazdego elementu
		@details Oblicza moc czynna i bierna dla kazdego elementu. Jesli moc jest dodatnia, to znaczy, ze element pobiera energie. Jesli ujemna, to znaczy, ze ja oddaje.
		@param brak parametrow
		*/
		void wyznacz_moce();
		/**
		@brief Funkcja wyznaczajaca czestotliwosc rezonansowa dla kazdego elementu
		@details Rezonans nie zachodzi dla rezystorow
		@param elementy Wektor wszystkich elementow obwodu
		@return freq Czestotliwosc rezonansu kondensatora lub cewki
		*/
		double wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy);
		/**
		@brief Funkcja zwracajaca skladowa freq dla kazdego elementu
		@details Dla rezystora zwroci NULL
		@param brak parametrow
		@return freq Czestotliwosc zrodel
		*/
		double czestotliwosc();
		/**
		@brief Destruktor elementu
		*/
		~R();
};
/**
@brief Klasa reprezentuje kondensatory, jest klasa pochodna elementu.
*/
class C : public element
{
	public:
		/**
		@brief Konstruktor tworzacy kondensator
		@param typ Typ elementu
		@param miejsce Umiejscowienie elementu (wezel poczatkowy-koncowy)
		@param wartosc Wartosc elementu w Faradach
		*/
		C(char typ, std::pair<int, int> miejsce, double wartosc);
		/**
		@brief Funkcja wyznaczajaca impedancje kondensatora
		@param wartosc Kapacytancja
		@param freq Czestotliwosc pracy zrodel
		@return impedancja
		*/
		std::complex<double> wyznacz_Z(double& wartosc, double& freq);
		/**
		@brief Funkcja wyznaczajaca admitancje elementu
		@param brak parametrow
		@return admitancja Zwraca admitancje elementu w postaci zespolonej
		*/
		std::complex<double> wyznacz_Y();
		/**
		@brief Funkcja wyznaczajaca prad na galezi elementu
		@details Wykorzystuje do tego obliczone wczesniej potencjaly w kazdym wezle obwodu i admitancje
		@param elementy Wektor wszystkich elementow z obwodu
		@param potencjaly Mapa potencjalow w wezlach
		*/
		void wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly);
		/**
		@brief Funkcja wyznaczajaca napiecie na kazdym elemencie
		@details Wykorzystuje do tego obliczone wczesniej potencjaly w kazdym wezle obwodu.
		@param potencjaly Mapa potencjalow w wezlach
		*/
		void wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly);
		/**
		@brief Funkcja wyznaczajaca moce dla kazdego elementu
		@details Oblicza moc czynna i bierna dla kazdego elementu. Jesli moc jest dodatnia, to znaczy, ze element pobiera energie. Jesli ujemna, to znaczy, ze ja oddaje.
		@param brak parametrow
		*/
		void wyznacz_moce();
		/**
		@brief Funkcja wyznaczajaca czestotliwosc rezonansowa dla kazdego elementu
		@details rezonans -> 1/sqrt(LC)
		@param elementy Wektor wszystkich elementow obwodu
		@return freq Czestotliwosc rezonansu kondensatora lub cewki
		*/
		double wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy);
		/**
		@brief Funkcja zwracajaca skladowa freq dla kazdego elementu
		@details Dla kondensatora zwroci NULL
		@param brak parametrow
		@return freq Czestotliwosc zrodel
		*/
		double czestotliwosc();
		/**
		@brief Destruktor elementu
		*/
		~C();
};
/**
@brief Klasa reprezentuje cewki, jest klasa pochodna elementu.
*/
class L : public element
{
	public:
		/**
		@brief Konstruktor tworzacy cewke
		@param typ Typ elementu
		@param miejsce Umiejscowienie elementu (wezel poczatkowy-koncowy)
		@param wartosc Wartosc elementu w Henrach
		*/
		L(char typ, std::pair<int, int> miejsce, double wartosc);
		/**
		@brief Funkcja wyznaczajaca impedancje cewki
		@param wartosc Przewodnosc
		@param freq Czestotliwosc pracy zrodel
		@return impedancja
		*/
		std::complex<double> wyznacz_Z(double& wartosc, double& freq);
		/**
		@brief Funkcja wyznaczajaca admitancje elementu
		@param brak parametrow
		@return admitancja Zwraca admitancje elementu w postaci zespolonej
		*/
		std::complex<double> wyznacz_Y();
		/**
		@brief Funkcja wyznaczajaca prad na galezi elementu
		@details Wykorzystuje do tego obliczone wczesniej potencjaly w kazdym wezle obwodu i admitancje
		@param elementy Wektor wszystkich elementow z obwodu
		@param potencjaly Mapa potencjalow w wezlach
		*/
		void wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly);
		/**
		@brief Funkcja wyznaczajaca napiecie na kazdym elemencie
		@details Wykorzystuje do tego obliczone wczesniej potencjaly w kazdym wezle obwodu.
		@param potencjaly Mapa potencjalow w wezlach
		*/
		void wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly);
		/**
		@brief Funkcja wyznaczajaca moce dla kazdego elementu
		@details Oblicza moc czynna i bierna dla kazdego elementu. Jesli moc jest dodatnia, to znaczy, ze element pobiera energie. Jesli ujemna, to znaczy, ze ja oddaje.
		@param brak parametrow
		*/
		void wyznacz_moce();
		/**
		@brief Funkcja wyznaczajaca czestotliwosc rezonansowa dla kazdego elementu
		@details rezonans -> 1/sqrt(LC)
		@param elementy Wektor wszystkich elementow obwodu
		@return freq Czestotliwosc rezonansu kondensatora lub cewki
		*/
		double wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy);
		/**
		@brief Funkcja zwracajaca skladowa freq dla kazdego elementu
		@details Dla cewki zwroci NULL
		@param brak parametrow
		@return freq Czestotliwosc zrodel
		*/
		double czestotliwosc();
		/**
		@brief Destruktor elementu
		*/
		~L();
};

#endif

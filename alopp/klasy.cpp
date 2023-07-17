#pragma once
/**
@file klasy.cpp
*/

#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "klasy.h"
#include "funkcje.h"

//element::element(char nazwa, std::pair<int, int> para, double wart)
//{
//	typ = nazwa;
//	umiejscowienie = para;
//	wartosc = wart;
//}
E::E(char typ, std::pair<int, int> miejsce, double wartosc, double fi, double freq)
{
	typ = this->typ;
	miejsce = umiejscowienie;
	wartosc = this->wartosc;
	fi = this->fi;
	freq = this->freq;
}

I::I(char typ, std::pair<int, int> miejsce, double wartosc, double fi, double freq)
{
	typ = this->typ;
	miejsce = umiejscowienie;
	wartosc = this->wartosc;
	fi = this->fi;
	freq = this->freq;
}

R::R(char typ, std::pair<int, int> miejsce, double wartosc)
{
	typ = this->typ;
	miejsce = umiejscowienie;
	wartosc = this->wartosc;
}

C::C(char typ, std::pair<int, int> miejsce, double wartosc)
{
	typ = this->typ;
	miejsce = umiejscowienie;
	wartosc = this->wartosc;
}

L::L(char typ, std::pair<int, int> miejsce, double wartosc)
{
	typ = this->typ;
	miejsce = umiejscowienie;
	wartosc = this->wartosc;
}

E::~E() {};
I::~I() {};
R::~R() {};
C::~C() {};
L::~L() {};

double E::wyznacz_Z(double& wartosc, double& freq)
{
	return wartosc;
}
double I::wyznacz_Z(double& wartosc, double& freq)
{
	return wartosc;
}
double R::wyznacz_Z(double& wartosc, double& freq)
{
	return wartosc;
}

double C::wyznacz_Z(double& wartosc, double& freq)
{
	return (1 / (2 * M_PI * freq * wartosc));
}

double L::wyznacz_Z(double& wartosc, double& freq)
{
	return (2 * M_PI * freq * wartosc);
}

//std::vector<E> E::odczyt_wejscia(const std::string& nazwa_pliku)
//{
//	std::ifstream plik(nazwa_pliku);
//	std::string linia;
//	char typ;
//	int wez_p, wez_k;
//	double wartosc, fi, freq;
//	std::vector<E> zrodlaE;
//	E sem;
//	if (plik)
//	{
//		while (getline(plik, linia))
//		{
//			std::stringstream ss;
//			ss << linia;
//			while (ss >> typ >> wez_p >> wez_k >> wartosc)
//			{
//				typ = std::toupper(typ);
//				std::pair<int, int> miejsce{ wez_p, wez_k };
//				if (typ == 'E')
//				{
//					ss >> fi >> freq;
//					sem.typ = typ;
//					sem.umiejscowienie = miejsce;
//					sem.wartosc = wartosc;
//					sem.fi = fi;
//					sem.freq = freq;
//					for (auto& i : zrodlaE)
//					{
//						if (freq != i.freq)
//						{
//							std::cout << "Zrodla nie moga miec tej roznej czestotliwosci w obwodzie!\n";
//							return zrodlaE;
//						}
//					}
//					zrodlaE.push_back(sem);
//				}
//			}
//		}
//	}
//	return zrodlaE;
//}
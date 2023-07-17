#pragma once
/**
@file klasy.h
*/

#ifndef KLASY_H
#define KLASY_H

#include <map>
#include <vector>
#include <string>
#include <complex>
#include <cmath>

/** 
@brief Struktura reprezentuje element i jego wartosci.
Struktura przechowuje informacje o elemencie ukladu:
- typ (rezystor, zrodlo SEM, zrodlo SPM)
- umiejscowienie (wezel poczatkowy, wezel koncowy)
- wartosc
- odlozone napiecie
- prad przeplywajacy przez element
- moc wydzielona
*/
//struct elem
//{
//	char typ;
//	std::pair<int, int> umiejscowienie;
//	double wartosc, fi, freq;
//};

typedef
std::vector<std::vector<double>> macierz;

class element
{
	public:
		char typ;
		std::pair<int, int> umiejscowienie;
		double wartosc, napiecie, prad, moc;
		//virtual double wyznacz_I();
		virtual double wyznacz_Z(double& wartosc, double& freq) = 0;
};
class E : public element
{
	public:
		double fi, freq;
		E(char typ, std::pair<int, int> miejsce, double wartosc, double fi, double freq);
		double wyznacz_Z(double& wartosc, double& freq);
		//std::complex<double> wyznacz_I() override;
		~E();
};
class I : public element
{
	public:
		double fi, freq;
		I(char typ, std::pair<int, int> miejsce, double wartosc, double fi, double freq);
		double wyznacz_Z(double& wartosc, double& freq);
		//std::complex<double> wyznacz_I() override;
		~I();
};
class R : public element
{
	public:
		R(char typ, std::pair<int, int> miejsce, double wartosc);
		double wyznacz_Z(double& wartosc, double& freq);
		//std::complex<double> wyznacz_I() override;
		~R();
};
class C : public element
{
	public:
		C(char typ, std::pair<int, int> miejsce, double wartosc);
		double wyznacz_Z(double& wartosc, double& freq);
		//std::complex<double> wyznacz_I() override;
		~C();
};
class L : public element
{
	public:
		L(char typ, std::pair<int, int> miejsce, double wartosc);
		double wyznacz_Z(double& wartosc, double& freq);
		//std::complex<double> wyznacz_I() override;
		~L();
};

#endif

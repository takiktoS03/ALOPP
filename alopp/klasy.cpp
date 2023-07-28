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
	this->typ = typ;
	umiejscowienie = miejsce;
	this->wartosc = wartosc;
	this->fi = fi;
	this->freq = freq;
}

I::I(char typ, std::pair<int, int> miejsce, double wartosc, double fi, double freq)
{
	this->typ = typ;
	umiejscowienie = miejsce;
	this->wartosc = wartosc;
	this->fi = fi;
	this->freq = freq;
}

R::R(char typ, std::pair<int, int> miejsce, double wartosc)
{
	this->typ = typ;
	umiejscowienie = miejsce;
	this->wartosc = wartosc;
}

C::C(char typ, std::pair<int, int> miejsce, double wartosc)
{
	this->typ = typ;
	umiejscowienie = miejsce;
	this->wartosc = wartosc;
}

L::L(char typ, std::pair<int, int> miejsce, double wartosc)
{
	this->typ = typ;
	umiejscowienie = miejsce;
	this->wartosc = wartosc;
}

E::~E() {};
I::~I() {};
R::~R() {};
C::~C() {};
L::~L() {};

double E::czestotliwosc()
{
	return E::freq;
}
double I::czestotliwosc()
{
	return I::freq;
}
double R::czestotliwosc()
{
	return NULL;
}
double C::czestotliwosc()
{
	return NULL;
}
double L::czestotliwosc()
{
	return NULL;
}

std::complex<double> E::wyznacz_Z(double& wartosc, double& freq)
{
	return { wartosc * cos(E::fi * M_PI / 180),wartosc * sin(E::fi * M_PI / 180) };
	
}
std::complex<double> I::wyznacz_Z(double& wartosc, double& freq)
{
	return { wartosc * cos(I::fi * M_PI / 180),wartosc * sin(I::fi * M_PI / 180) };
}
std::complex<double> R::wyznacz_Z(double& wartosc, double& freq)
{
	return { wartosc,0 };
}
std::complex<double> C::wyznacz_Z(double& wartosc, double& freq)
{
	return { e,-1 / (2 * M_PI * freq * wartosc) + e };
}
std::complex<double> L::wyznacz_Z(double& wartosc, double& freq)
{
	return { e,(2 * M_PI * freq * wartosc) + e};
}

std::complex<double> jeden {1, 0};
std::complex<double> E::wyznacz_Y()
{
	return jeden / E::impedancja;
}
std::complex<double> I::wyznacz_Y()
{
	return jeden / I::impedancja;
}
std::complex<double> R::wyznacz_Y()
{
	return jeden / R::impedancja;
}
std::complex<double> C::wyznacz_Y()
{
	return jeden / C::impedancja;
}
std::complex<double> L::wyznacz_Y()
{
	return jeden / L::impedancja;
}
double E::wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy)
{
	return NULL;
}
double I::wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy)
{
	return NULL;
}
double R::wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy)
{
	return NULL;
}
double C::wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy)
{
	for (auto& i : elementy)
	{
		if (i->typ == 'L' && (i->umiejscowienie.first == C::umiejscowienie.second || i->umiejscowienie.first == C::umiejscowienie.first || i->umiejscowienie.second == C::umiejscowienie.first || i->umiejscowienie.second == C::umiejscowienie.second))
		{
			return 1 / (2 * M_PI * sqrt(i->wartosc * C::wartosc));
		}
	}
	return 0;
}
//((i->umiejscowienie == C::umiejscowienie || (i->umiejscowienie.first == C::umiejscowienie.second && i->umiejscowienie.second == C::umiejscowienie.first)) //rownolegle polaczenie
double L::wyznacz_czest_rez(const std::vector<std::shared_ptr<element>>& elementy)
{
	for (auto& i : elementy)
	{
		if (i->typ == 'C' && (i->umiejscowienie.first == L::umiejscowienie.second || i->umiejscowienie.first == L::umiejscowienie.first || i->umiejscowienie.second == L::umiejscowienie.first || i->umiejscowienie.second == L::umiejscowienie.second))
		{
			return 1 / (2 * M_PI * sqrt(i->wartosc * L::wartosc));
		}
	}
	return 0;
}

void E::wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly) 
{
	for (auto& j : elementy)
	{
		if (j->typ == 'R' && j->wartosc == -1 && E::umiejscowienie.second == j->umiejscowienie.second)
		{
			E::prad = j->prad; // prad na zrodle SEM jest taki jak prad do niego doplywajacy (z dodanego rezystora -1)
			E::umiejscowienie.second = j->umiejscowienie.first; // zamiana numerow wezlow spowrotem
		}
	}
}
void I::wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly)
{
	I::prad = -I::impedancja; // prad na zrodle SPM jest taki jak jego wartosc
}
void R::wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly)
{
	if (R::typ == 'R' && R::wartosc == -1)
	{
		R::prad = (potencjaly[R::umiejscowienie.second] - potencjaly[R::umiejscowienie.first]) * R::admitancja;
	}
	else
		R::prad = (potencjaly[R::umiejscowienie.first] - potencjaly[R::umiejscowienie.second]) * R::admitancja;
}
void C::wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly)
{
	C::prad = (potencjaly[C::umiejscowienie.first] - potencjaly[C::umiejscowienie.second]) * C::admitancja;
}
void L::wyznacz_I(const std::vector<std::shared_ptr<element>>& elementy, std::unordered_map<int, std::complex<double>>& potencjaly)
{
	L::prad = (potencjaly[L::umiejscowienie.first] - potencjaly[L::umiejscowienie.second]) * L::admitancja;
}


void E::wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly)
{
	E::napiecie = potencjaly[E::umiejscowienie.first] - potencjaly[E::umiejscowienie.second];
}
void I::wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly)
{
	I::napiecie = potencjaly[I::umiejscowienie.first] - potencjaly[I::umiejscowienie.second];
}
void R::wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly)
{
	R::napiecie = potencjaly[R::umiejscowienie.first] - potencjaly[R::umiejscowienie.second];
}
void C::wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly)
{
	C::napiecie = potencjaly[C::umiejscowienie.first] - potencjaly[C::umiejscowienie.second];
}
void L::wyznacz_V(std::unordered_map<int, std::complex<double>>& potencjaly)
{
	L::napiecie = potencjaly[L::umiejscowienie.first] - potencjaly[L::umiejscowienie.second];
}

void E::wyznacz_moce()
{
	E::moc_czynna = sqrt(pow(E::napiecie.real(), 2) + pow(E::napiecie.imag(), 2)) * sqrt(pow(E::prad.real(), 2) + pow(E::prad.imag(), 2)) * cos(arg(E::napiecie) - arg(E::prad));
	E::moc_bierna = sqrt(pow(E::napiecie.real(), 2) + pow(E::napiecie.imag(), 2)) * sqrt(pow(E::prad.real(), 2) + pow(E::prad.imag(), 2)) * sin(arg(E::napiecie) - arg(E::prad));
}
void I::wyznacz_moce()
{
	I::moc_czynna = sqrt(pow(I::napiecie.real(), 2) + pow(I::napiecie.imag(), 2)) * sqrt(pow(I::prad.real(), 2) + pow(I::prad.imag(), 2)) * cos(arg(I::napiecie) - arg(I::prad));
	I::moc_bierna = sqrt(pow(I::napiecie.real(), 2) + pow(I::napiecie.imag(), 2)) * sqrt(pow(I::prad.real(), 2) + pow(I::prad.imag(), 2)) * sin(arg(I::napiecie) - arg(I::prad));
}
void R::wyznacz_moce()
{
	R::moc_czynna = sqrt(pow(R::napiecie.real(), 2) + pow(R::napiecie.imag(), 2)) * sqrt(pow(R::prad.real(), 2) + pow(R::prad.imag(), 2)) * cos(arg(R::napiecie) - arg(R::prad));
	R::moc_bierna = sqrt(pow(R::napiecie.real(), 2) + pow(R::napiecie.imag(), 2)) * sqrt(pow(R::prad.real(), 2) + pow(R::prad.imag(), 2)) * sin(arg(R::napiecie) - arg(R::prad));
}
void C::wyznacz_moce()
{
	C::moc_czynna = sqrt(pow(C::napiecie.real(), 2) + pow(C::napiecie.imag(), 2)) * sqrt(pow(C::prad.real(), 2) + pow(C::prad.imag(), 2)) * cos(arg(C::napiecie) - arg(C::prad));
	C::moc_bierna = sqrt(pow(C::napiecie.real(), 2) + pow(C::napiecie.imag(), 2)) * sqrt(pow(C::prad.real(), 2) + pow(C::prad.imag(), 2)) * sin(arg(C::napiecie) - arg(C::prad));
}
void L::wyznacz_moce()
{
	L::moc_czynna = sqrt(pow(L::napiecie.real(), 2) + pow(L::napiecie.imag(), 2)) * sqrt(pow(L::prad.real(), 2) + pow(L::prad.imag(), 2)) * cos(arg(L::napiecie) - arg(L::prad));
	L::moc_bierna = sqrt(pow(L::napiecie.real(), 2) + pow(L::napiecie.imag(), 2)) * sqrt(pow(L::prad.real(), 2) + pow(L::prad.imag(), 2)) * sin(arg(L::napiecie) - arg(L::prad));
}
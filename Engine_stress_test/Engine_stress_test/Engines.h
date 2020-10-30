#pragma once
#include <vector>

namespace engines
{
	//Добавление двигателя 
	struct InternalCombustionEngine
	{
		//Момент инерции двигателя I (кг∙м2)
		double I = 10;
		//Температура перегрева
		double overheatTemperature = 110;
		//Коэффициент зависимости скорости нагрева от крутящего момента
		double Hm = 0.01;
		//Коэффициент зависимости скорости нагрева от скорости вращения коленвала
		double Hv = 0.0001;
		//Коэффициент зависимости скорости охлаждения от температуры двигателя и окружающей среды
		double C = 0.1;
		//Значения для крутящего момента M
		std::vector<double> startM = { 20, 75, 100, 105, 75, 0 };
		//Значения для скорости вращения коленвала
		std::vector<double> startV = { 0, 75, 150, 200, 250, 300 };

		double M;
		double V;
		//Функция для нахождения скорости охлаждения двигателя
		double Vc(double ambientTemperature, double engineTemperature);
		//Функция для нахождения скорости нагревания двигателя
		double Vh();
	};

}
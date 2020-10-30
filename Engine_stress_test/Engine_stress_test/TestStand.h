#pragma once
#include "Engines.h"
#include <iostream>
#define ABSOLUTE_ERROR 10e-2
#define MAX_TIME 1800

namespace standArrea
{
	
	template <typename T>
	class TestStand
	{
		int numberOfMandV;
		double engineTemperature;
		double ambientTemperature;
		T* engine;

	public:
		//Функция возвращает время перегрева двигателя
		int StartEngine();
		//Конструктор класса TestStand 
		TestStand(T& engine, const double ambientTemperature);
	};

	template<typename T>
	TestStand<T>::TestStand(T& engine, const double ambientTemperature)
	{
		this->engine = &engine;
		this->engine->M = engine.startM[0];
		this->engine->V = engine.startV[0];
		engineTemperature = ambientTemperature;
		this->ambientTemperature = ambientTemperature;
	}


	template<typename T>
	int TestStand<T>::StartEngine()
	{
		// Текущее усколрение вала
		//std::cout << "Current M - " << engine->M << std::endl;
		//std::cout << "Current I - " << engine->I << std::endl;
		double acceleration = engine->M / engine->I; 
		//std::cout << "Current acceleration - " << acceleration << std::endl;
		// Погрешность температуры для определения времени
		double eps = engine->overheatTemperature - engineTemperature;
		// Текущее время рабоы двигателя
		int time = 0;
		//Промежуток времени через который змеряется температура в секундах
		int deltaTime = 1;
		//Начальная скорость на текущем промежутке
		double V_0 = 0;
		//Текущая граница для интерполяции крутящего момента и подсчета ускорения вращения вала
		int currentNumberInStartV = 1;
		//Флаг на достижение максимального предоставленного крутящего момента 
		bool VIsMax = false;

		while (eps > ABSOLUTE_ERROR && time < MAX_TIME)
		{
			time+= deltaTime;
			engine->V = V_0 + acceleration * deltaTime;
			V_0 = engine->V;
			//Проверка перехода к другому значению ускорения (достижение определенной скорости вращения)
			if (engine->V > engine->startV[currentNumberInStartV] && VIsMax == false)
			{
				acceleration = engine->startM[currentNumberInStartV]/ engine->I;
				currentNumberInStartV++;
				if (currentNumberInStartV == engine->startM.size()-1)
				{
					VIsMax = true;
				}
				
			}
			//Интерполяция для значения M (крутящий момент)
			engine->M = engine->startM[currentNumberInStartV - 1] + ((engine->startM[currentNumberInStartV] - engine->startM[currentNumberInStartV - 1]) /
				(engine->startV[currentNumberInStartV] - engine->startV[currentNumberInStartV - 1])) * (engine->V - engine->startV[currentNumberInStartV - 1]);
			//Рассчет температуры
			engineTemperature = engineTemperature + (engine->Vc(ambientTemperature, engineTemperature) + engine->Vh()) * deltaTime;
			//Обновление условия цикла
			eps = engine->overheatTemperature - engineTemperature;
		}
		return time;
	}

}
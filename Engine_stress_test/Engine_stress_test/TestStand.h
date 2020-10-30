#pragma once
#include "Engines.h"

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
		int StartEngine();
		TestStand(T& engine, const double ambientTemperature);
	};

	template<typename T>
	TestStand<T>::TestStand(T& engine, const double ambientTemperature)
	{
		//numberOfMandV = 0;
		this->engine = &engine;
		/*this->engine->M = engine.startM[numberOfMandV];
		this->engine->V = engine.startV[numberOfMandV];*/
		this->engine->M = engine.startM[0];
		this->engine->V = engine.startV[0];
		engineTemperature = ambientTemperature;
		this->ambientTemperature = ambientTemperature;
	}


	template<typename T>
	int TestStand<T>::StartEngine()
	{
		// ������� ���������� ����
		double acceleration = engine->M / engine->I; 
		// ����������� ����������� ��� ����������� �������
		double eps = engine->overheatTemperature - engineTemperature;
		// ������� ����� ����� ���������
		int time = 0;
		//���������� ������� ����� ������� ��������� ����������� � ��������
		int deltaTime = 10;
		//��������� �������� �� ������� ����������
		double V_0 = 0;
		//������� ������� ��� ������������ ��������� ������� � �������� ��������� �������� ����
		int currentNumberInStartV = 1;

		while (eps > ABSOLUTE_ERROR && time < MAX_TIME)
		{
			time+= deltaTime;
			engine->V = V_0 + acceleration * deltaTime;
			V_0 = engine->V;
			//�������� �������� � ������� �������� ��������� (���������� ������������ �������� ��������)
			if (engine->V > engine->startV[currentNumberInStartV])
			{
				acceleration = engine->startM[currentNumberInStartV]/ engine->I;
				currentNumberInStartV++;
				
			}
			//������������ ��� �������� M (�������� ������)
			engine->M = engine->startM[currentNumberInStartV - 1] + ((engine->startM[currentNumberInStartV] - engine->startM[currentNumberInStartV - 1]) /
				(engine->startV[currentNumberInStartV] - engine->startV[currentNumberInStartV - 1])) * (engine->V - engine->startV[currentNumberInStartV - 1]);
			engineTemperature += engineTemperature + (engine->Vc(ambientTemperature, engineTemperature) + engine->Vh()) * deltaTime;
			//
			//engine->V += acceleration; // V = V_0+a*time
			//if (numberOfMandV < engine->startM.size() - 2)
			//	numberOfMandV += engine->V < engine->startV[numberOfMandV + 1] ? 0 : 1;
			//double up = (engine->V - engine->startV[numberOfMandV]);
			//double down = (engine->startV[numberOfMandV + 1] - engine->startV[numberOfMandV]);
			//double factor = (engine->startM[numberOfMandV + 1] - engine->startM[numberOfMandV]);
			//engine->M = up / down * factor + engine->startM[numberOfMandV];

			//engineTemperature += engine->Vc(ambientTemperature, engineTemperature) + engine->Vh();

			//acceleration = engine->M / engine->I;

			eps = engine->overheatTemperature - engineTemperature;
		}
		return time;
	}

}
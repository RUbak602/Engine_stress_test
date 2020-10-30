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
		//������� ���������� ����� ��������� ���������
		int StartEngine();
		//����������� ������ TestStand 
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
		// ������� ���������� ����
		//std::cout << "Current M - " << engine->M << std::endl;
		//std::cout << "Current I - " << engine->I << std::endl;
		double acceleration = engine->M / engine->I; 
		//std::cout << "Current acceleration - " << acceleration << std::endl;
		// ����������� ����������� ��� ����������� �������
		double eps = engine->overheatTemperature - engineTemperature;
		// ������� ����� ����� ���������
		int time = 0;
		//���������� ������� ����� ������� ��������� ����������� � ��������
		int deltaTime = 1;
		//��������� �������� �� ������� ����������
		double V_0 = 0;
		//������� ������� ��� ������������ ��������� ������� � �������� ��������� �������� ����
		int currentNumberInStartV = 1;
		//���� �� ���������� ������������� ���������������� ��������� ������� 
		bool VIsMax = false;

		while (eps > ABSOLUTE_ERROR && time < MAX_TIME)
		{
			time+= deltaTime;
			engine->V = V_0 + acceleration * deltaTime;
			V_0 = engine->V;
			//�������� �������� � ������� �������� ��������� (���������� ������������ �������� ��������)
			if (engine->V > engine->startV[currentNumberInStartV] && VIsMax == false)
			{
				acceleration = engine->startM[currentNumberInStartV]/ engine->I;
				currentNumberInStartV++;
				if (currentNumberInStartV == engine->startM.size()-1)
				{
					VIsMax = true;
				}
				
			}
			//������������ ��� �������� M (�������� ������)
			engine->M = engine->startM[currentNumberInStartV - 1] + ((engine->startM[currentNumberInStartV] - engine->startM[currentNumberInStartV - 1]) /
				(engine->startV[currentNumberInStartV] - engine->startV[currentNumberInStartV - 1])) * (engine->V - engine->startV[currentNumberInStartV - 1]);
			//������� �����������
			engineTemperature = engineTemperature + (engine->Vc(ambientTemperature, engineTemperature) + engine->Vh()) * deltaTime;
			//���������� ������� �����
			eps = engine->overheatTemperature - engineTemperature;
		}
		return time;
	}

}
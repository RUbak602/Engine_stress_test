// Engine_stress_test.cpp 
//
#include "TestStand.h"
#include <iostream>
using namespace std;

int main()
{
	double ambientTemperature;
	cout << "Please, enter ambient temperature in degrees Celsius" << endl;
	cin >> ambientTemperature;
	engines::InternalCombustionEngine engine;
	standArrea::TestStand<engines::InternalCombustionEngine> stand(engine, ambientTemperature);
	double time = stand.StartEngine();
	if (time == MAX_TIME)
		cout << endl << "At this ambient temperature, the engine does not overheat." << endl;
	else
		cout << endl << "Time of engine overheating: " << time << " sec" << endl;
}


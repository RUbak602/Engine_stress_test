#include "Engines.h"

namespace engines
{
	double InternalCombustionEngine::Vc(double ambientTemperature, double engineTemperature)
	{
		return C * (ambientTemperature - engineTemperature);
	}

	double InternalCombustionEngine::Vh()
	{
		return M * Hm + V * V * Hv;
	}
}
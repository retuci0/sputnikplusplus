#include "ModuleManager.h"

#include "modules/FlightModule.h"


void ModuleManager::addModules() {
	addModule<FlightModule>();
}
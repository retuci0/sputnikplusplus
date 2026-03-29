#include "ModuleManager.h"

#include "modules/FlightModule.h"


// añade todos los módulos
void ModuleManager::addModules() {
	addModule<FlightModule>();
}
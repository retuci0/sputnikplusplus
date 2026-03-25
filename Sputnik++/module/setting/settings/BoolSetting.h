#pragma once

#include "../Setting.h"


class BoolSetting : public Setting<bool> {
public:
	BoolSetting(std::string name, std::string desc, bool defaultValue)
		: Setting<bool>(name, desc, defaultValue) {}

	void toggle() {
		setValue(!getValue());
	}
};
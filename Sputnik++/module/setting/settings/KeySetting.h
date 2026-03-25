#pragma once

#include "../Setting.h"
#include "../../../util/KeyUtil.h"
#include <windows.h>
#include <WinUser.h>


class KeySetting : public Setting<int> {
public:
	KeySetting(const std::string& name, const std::string& desc, int defaultKey)
		: Setting(name, desc, defaultKey) {}

	void setValue(const int& key) override {
		int newKey = key;
		if (newKey <= 0 || newKey == VK_ESCAPE) newKey = -1;
		Setting<int>::setValue(newKey);
	}

	const std::string getKeyName() {
		return KeyUtil::getKeyName(value);
	}
};
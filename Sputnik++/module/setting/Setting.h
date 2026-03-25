#pragma once

#include <string>
#include <functional>

class Module;
class SettingGroup;


class SettingBase {
protected:
	std::string name, desc;
	SettingBase(const std::string& name, const std::string& desc)
		: name(name), desc(desc) {}

public:
	std::string& getName() { return name; }
	std::string& getDesc() { return desc; }
};

template<typename T>
class Setting : public SettingBase {
public:
	virtual ~Setting() = default;

	const T& getValue() const {
		return value;
	}

	virtual void setValue(const T& value) {
		if (this->value != value) {
			this->value = value;

			if (updateListener != nullptr) {
				updateListener(value);
			}
		}
	}

	const T& getDefaultValue() const {
		return defaultValue;
	}

	void setDefaultValue(const T& defaultValue) {
		this->defaultValue = defaultValue;
	}

	void reset() {
		setValue(defaultValue);
	}

	SettingGroup* getSg() const {
		return sg;
	}

	void setSg(SettingGroup* sg) {
		this->sg = sg;
	}

	Module* getModule() const {
		return sg->getModule();
	}

	bool isVisible() const {
		if (vCondition != nullptr) {
			return vCondition();
		}
		return visible;
	}

	void setVisible(bool visible) {
		this->visible = visible;
		this->vCondition = nullptr;
	}

	Setting<T>& visibility(std::function<bool()> vCondition) {
		this->vCondition = vCondition;
		return *this;
	}

	bool isSearchMatch() const {
		return searchMatch;
	}

	void setSearchMatch(bool searchMatch) {
		this->searchMatch = searchMatch;
	}

	void onUpdate(std::function<void(T)> updateListener) {
		this->updateListener = updateListener;
		if (updateListener != nullptr) updateListener(value);
	}

protected:
	T value, defaultValue;
	std::function<void(T)> updateListener = nullptr;
	std::function<bool()> vCondition = nullptr;
	bool visible, searchMatch;
	SettingGroup* sg = nullptr;

	Setting(const std::string& name, const std::string& desc, T defaultValue)
		: SettingBase(name, desc), value(defaultValue), defaultValue(defaultValue) {}
};
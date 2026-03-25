#pragma once

#include "../Setting.h"
#include <algorithm>


class NumberSetting : public Setting<double> {
public:
	NumberSetting(const std::string& name, const std::string& desc, double defaultValue, double min, double max, double step)
		: Setting<double>(name, desc, defaultValue), min(min), max(max), step(step), locked(false) {}

	double getMin() const { return min; }
	double getMax() const { return max; }
	double getStep() const { return step; }

	void increment(bool positive) {
		if (locked) return;
		double newValue = value;
		if (positive) newValue += step;
		else newValue -= step;
		setValue(newValue);
	}

	void setValue(const double& newValue) override {
		if (newValue == value || locked) return;
		double clamped = std::clamp(newValue, min, max);
		double stepped = std::round(clamped / step) * step;
		Setting<double>::setValue(stepped);
	}

	bool isValid(double newValue) const {
		return newValue >= min && newValue <= max;
	}

	void lock() { locked = true; }
	void unlock() { locked = false; }

private:
	double min, max;
	double step;
	bool locked;
};
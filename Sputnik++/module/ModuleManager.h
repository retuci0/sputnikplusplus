#pragma once

#include "Module.h"
#include <iostream>
#include <memory>
#include <vector>

class ModuleManager {
public:
	static ModuleManager* getInstance() {
		static ModuleManager INSTANCE;
		return &INSTANCE;
	}

	template<typename T>
	void addModule() {
		modules.push_back(std::unique_ptr<T>(new T()));
	}

	void addModules();

	template<typename T>
	T* getModuleByClass() {
		for (auto& mod : modules) {
			T* derived = dynamic_cast<T*>(mod.get());
			if (derived) {
				return derived;
			}
		}
		return nullptr;
	}

	void tick() {
		for (auto& mod : modules) {
			if (mod->isEnabled()) {
				mod->onTick();
			}
		}
	}

	void onKey(int key, int action) {
		for (auto& mod : modules) {
			if (mod->getKey() == key) {
				mod->toggle();
			}

			if (mod->isEnabled()) {
				mod->onKey(key, action);
			}
		}
	}

	void onClick(int button, int action) {
		for (auto& mod : modules) {
			if (mod->isEnabled()) {
				mod->onClick(button, action);
			}
		}
	}

	void onWheel(int delta) {
		for (auto& mod : modules) {
			if (mod->isEnabled()) {
				mod->onWheel(delta);
			}
		}
	}

	const std::vector<std::unique_ptr<Module>>& getModules() const { return modules; }

private:
	ModuleManager() {}
	ModuleManager(const ModuleManager&);
	std::vector<std::unique_ptr<Module>> modules;
};
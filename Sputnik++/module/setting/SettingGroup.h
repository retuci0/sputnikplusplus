#pragma once

#include "../Module.h"
#include "Setting.h"

#include <string>
#include <vector>

class SettingGroup {
public:
    SettingGroup(const std::string& name, bool extended)
        : name(name), extended(extended) {}

    auto begin() { return settings.begin(); }
    auto end() { return settings.end(); }
    auto begin() const { return settings.begin(); }
    auto end()   const { return settings.end(); }

    template<typename S>
    S* add(S* setting) {
        setting->setSg(this);
        settings.push_back(setting);
        return setting;
    }

    template<typename... Args>
    void addAll(Args* ... args) {
        (add(args), ...);
    }

    SettingBase* getSetting(const std::string& name) {
        auto it = std::find_if(settings.begin(), settings.end(),
            [&](SettingBase* s) { return s->getName() == name; });
        return (it != settings.end()) ? *it : nullptr;
    }

    const std::vector<SettingBase*>& getSettings() const {
        return settings;
    }

    std::string getName() const {
        return name;
    }

    bool isExtended() const {
        return extended;
    }

    void setExtended(bool extended) {
        this->extended = extended;
    }

    Module* getModule() const {
        return module;
    }

    void setModule(Module* module) {
        this->module = module;
    }

private:
    Module* module = nullptr;
    std::string name;
    bool extended;
    std::vector<SettingBase*> settings;
};
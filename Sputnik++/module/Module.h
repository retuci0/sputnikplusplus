#pragma once

#include "Category.h"
#include "setting/SettingGroup.h"
#include "setting/settings/KeySetting.h"

#include <memory>
#include <string>
#include <vector>


class Module {
public:
    Module(const std::string& name, const std::string& desc, const Category& category, int key)
        : name(name), desc(desc), category(category) {
        bind->setValue(key);
    }

    virtual ~Module() = default;

	// ni copiable ni asignable, los módulos son propiedad única del ModuleManager
    Module(const Module&) = delete;
    Module& operator=(const Module&) = delete;

    int  getKey()     const { return bind->getValue(); }
    bool isEnabled()  const { return enabled; }

    void setEnabled(bool value) { enabled = value; }

    void toggle() {
        enabled = !enabled;
        enabled ? onEnable() : onDisable();
    }

    virtual void onEnable() {}
    virtual void onDisable() {}
    virtual void onTick() {}

    virtual void onKey(int vkCode, int action) {}
    virtual void onClick(int button, int action) {}
    virtual void onWheel(int delta) {}

protected:
    SettingGroup* addSg(const std::string& sgName, bool extended) {
        auto sg = std::make_unique<SettingGroup>(sgName, extended);
        auto raw = sg.get();
        sgs.push_back(std::move(sg));
        return raw;
    }

    std::string name, desc;
    Category category;
    bool enabled = false;
    bool searchMatch = false;
    bool saveSettings = false;

    std::vector<std::unique_ptr<SettingGroup>> sgs;
    SettingGroup* sgGeneral = addSg("general", true);
    KeySetting* bind = sgGeneral->add(new KeySetting("tecla", "tecla asignada", -1));
};
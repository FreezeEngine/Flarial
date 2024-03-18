#pragma once

#include "../Module.hpp"
#include "../../../Events/EventHandler.hpp"
#include "SneakListener.hpp"


class Sneak : public Module {

public:


    Sneak() : Module("Toggle Sneak", "No need to hold down your sneak key.", R"(\Flarial\assets\slowly.png)", "SHIFT") {

        Module::setup();

    };

    void onEnable() override {
        EventHandler::registerListener(new SneakListener("Sneak", this));
        Module::onEnable();
    }

    void onDisable() override {
        EventHandler::unregisterListener("Sneak");
        Module::onDisable();
    }

    void defaultConfig() override {

        if (settings.getSettingByName<bool>("status") == nullptr) settings.addSetting("status", false);

        if (settings.getSettingByName<float>("textscale") == nullptr) settings.addSetting("textscale", 0.80f);

        if (settings.getSettingByName<bool>("always") == nullptr) {
            settings.addSetting("always", false);
        }
    }

    void settingsRender() override {

        float toggleX = Constraints::PercentageConstraint(0.019, "left");
        float toggleY = Constraints::PercentageConstraint(0.10, "top");

        FlarialGUI::KeybindSelector(0, toggleX, toggleY, getKeybind());

    }
};


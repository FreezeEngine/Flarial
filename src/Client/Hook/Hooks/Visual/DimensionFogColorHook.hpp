#pragma once

#include "../Hook.hpp"
#include "../../../../Utils/Memory/Memory.hpp"
#include "../../../../Utils/Utils.hpp"
#include "../../../Module/Manager.hpp"
#include "../../../Events/Render/FogColorEvent.hpp"

class DimensionFogColorHook : public Hook {
private:

    static MCCColor &
    DimensionFogColorCallback(Dimension *_this, MCCColor &result, MCCColor const &baseColor, float brightness) {

        FogColorEvent event(funcOriginal(_this, result, baseColor, brightness));
        EventHandler::onGetFogColor(event);

        return event.getFogColor();

    }

public:
    typedef MCCColor &(__thiscall *dimensionFogColorOriginal)(Dimension *, MCCColor &, MCCColor const &, float);

    static inline dimensionFogColorOriginal funcOriginal = nullptr;

    DimensionFogColorHook() : Hook("Fog Color Hook", "41 0F 10 00 48 8B C2 0F 11 02") {}

    void enableHook() override {

        this->autoHook( (void *) DimensionFogColorCallback, (void **) &funcOriginal);

    }
};



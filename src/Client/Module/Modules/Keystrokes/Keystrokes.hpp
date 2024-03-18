#pragma once

#include "../../../../SDK/SDK.hpp"
#include <format>
#include "../../../Events/Listener.hpp"
#include "../Module.hpp"
#include "../../../GUI/Engine/Engine.hpp"
#include <Windows.h>
#include "KeystrokesListener.hpp"
#include "../CPS/CPSListener.hpp"
#include "../../../Events/EventHandler.hpp"
#include "../ClickGUI/ClickGUIRenderer.hpp"

class Keystrokes : public Module {

public:

    enum Strokes {
        W,
        A,
        S,
        D,
        SPACEBAR,
        LMB,
        RMB
    };

    std::vector<D2D1_COLOR_F> states;
    std::vector<D2D1_COLOR_F> textStates;


    Keystrokes() : Module("Keystrokes", "Displays real-time information about your\nWASD and mouse inputs.",
                          R"(\Flarial\assets\keyboard.png)", "") {
        Module::setup();
    };

    void onSetup() override {
        D2D1_COLOR_F d = FlarialGUI::HexToColorF(settings.getSettingByName<std::string>("bgColor")->value);
        D2D1_COLOR_F e = FlarialGUI::HexToColorF(settings.getSettingByName<std::string>("textColor")->value);
        d.a = settings.getSettingByName<float>("bgOpacity")->value;
        e.a = settings.getSettingByName<float>("textOpacity")->value;

        states = std::vector<D2D1_COLOR_F>(7, d);
        textStates = std::vector<D2D1_COLOR_F>(7, e);
    }

    void onEnable() override {
        EventHandler::registerListener(new KeystrokesListener("Keystrok", this));
        Module::onEnable();
    }

    void onDisable() override {
        EventHandler::unregisterListener("Keystrok");
        Module::onDisable();
    }

    void defaultConfig() override {
        if (settings.getSettingByName<float>("rounding") == nullptr)
            settings.addSetting("rounding", 11.0f);

        if (settings.getSettingByName<bool>("cps") == nullptr)
            settings.addSetting("cps", false);

        if (settings.getSettingByName<float>("percentageX") == nullptr) {
            settings.addSetting("percentageX", 0.0f);
            settings.addSetting("percentageY", 0.0f);
        }

        if (settings.getSettingByName<bool>("border") == nullptr) {
            settings.addSetting("border", true);
            settings.addSetting("borderWidth", 1.0f);
        }

        if (settings.getSettingByName<std::string>("enabledColor") == nullptr)
            settings.addSetting("enabledColor", (std::string) "fafafa");
        if (settings.getSettingByName<bool>("enabledRGB") == nullptr)
            settings.addSetting("enabledRGB", false);
        if (settings.getSettingByName<float>("enabledRGBSpeed") == nullptr)
            settings.addSetting("enabledRGBSpeed", 1.0f);

        if (settings.getSettingByName<std::string>("textEnabledColor") == nullptr)
            settings.addSetting("textEnabledColor", (std::string) "fafafa");
        if (settings.getSettingByName<bool>("textEnabledRGB") == nullptr)
            settings.addSetting("textEnabledRGB", false);
        if (settings.getSettingByName<float>("textEnabledRGBSpeed") == nullptr)
            settings.addSetting("textEnabledRGBSpeed", 1.0f);

        if (settings.getSettingByName<float>("enabledOpacity") == nullptr)
            settings.addSetting("enabledOpacity", 0.55f);

        if (settings.getSettingByName<float>("textEnabledOpacity") == nullptr)
            settings.addSetting("textEnabledOpacity", 0.55f);

        if (settings.getSettingByName<float>("uiscale") == nullptr)
            settings.addSetting("uiscale", 1.0f);

        if (settings.getSettingByName<float>("textscale") == nullptr)
            settings.addSetting("textscale", 1.0f);

        if (settings.getSettingByName<float>("textscale2") == nullptr)
            settings.addSetting("textscale2", 1.0f);

        if (settings.getSettingByName<bool>("BlurEffect") == nullptr)
            settings.addSetting("BlurEffect", false);

        if (settings.getSettingByName<bool>("lmbrmb") == nullptr)
            settings.addSetting("lmbrmb", true);

        if (settings.getSettingByName<bool>("hidecps") == nullptr)
            settings.addSetting("hidecps", true);

        if (settings.getSettingByName<std::string>("lmbtext") == nullptr)
            settings.addSetting("lmbtext", (std::string) "{value} CPS");

        if (settings.getSettingByName<std::string>("rmbtext") == nullptr)
            settings.addSetting("rmbtext", (std::string) "{value} CPS");

        if (settings.getSettingByName<std::string>("wText") == nullptr)
            settings.addSetting("wText", (std::string) "W");

        if (settings.getSettingByName<std::string>("aText") == nullptr)
            settings.addSetting("aText", (std::string) "A");

        if (settings.getSettingByName<std::string>("sText") == nullptr)
            settings.addSetting("sText", (std::string) "S");

        if (settings.getSettingByName<std::string>("dText") == nullptr)
            settings.addSetting("dText", (std::string) "D");

        if (settings.getSettingByName<float>("spacebarWidth") == nullptr)
            settings.addSetting("spacebarWidth", 0.5f);

        if (settings.getSettingByName<float>("spacebarHeight") == nullptr) {
            settings.addSetting("spacebarHeight", 0.09f);
        }

        if (settings.getSettingByName<float>("keySpacing") == nullptr) {
            settings.addSetting("keySpacing", 1.63f);
        }
    }

    void settingsRender() override {
        /* Border Start */

        float toggleX = Constraints::PercentageConstraint(0.019, "left");
        float toggleY = Constraints::PercentageConstraint(0.10, "top");

        const float textWidth = Constraints::RelativeConstraint(0.12, "height", true);
        const float textHeight = Constraints::RelativeConstraint(0.029, "height", true);

        FlarialGUI::ScrollBar(toggleX, toggleY, 140, Constraints::SpacingConstraint(5.5, textWidth), 2);
        FlarialGUI::SetScrollView(toggleX, Constraints::PercentageConstraint(0.00, "top"),
                                  Constraints::RelativeConstraint(1.0, "width"),
                                  Constraints::RelativeConstraint(1.0f, "height"));

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"UI Scale", textWidth * 6.9f,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::RelativeConstraint(0.12, "height", true),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        float percent = FlarialGUI::Slider(4, toggleX + FlarialGUI::SettingsTextWidth("UI Scale "),
                                           toggleY,
                                           this->settings.getSettingByName<float>("uiscale")->value, 2.0f);

        this->settings.getSettingByName<float>("uiscale")->value = percent;

        /* Rounding Start */
        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"Rounding", textWidth * 6.9f,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::RelativeConstraint(0.12, "height", true),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        percent = FlarialGUI::Slider(6, toggleX + FlarialGUI::SettingsTextWidth("Rounding "),
                                     toggleY,
                                     this->settings.getSettingByName<float>("rounding")->value);

        this->settings.getSettingByName<float>("rounding")->value = percent;

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX + Constraints::SpacingConstraint(0.60, textWidth), toggleY,
                                        L"Show CPS",
                                        textWidth * 6.9f, textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::SpacingConstraint(1.05, textWidth),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        if (FlarialGUI::Toggle(2, toggleX, toggleY,
                               this->settings.getSettingByName<bool>("cps")->value))
            this->settings.getSettingByName<bool>("cps")->value = !this->settings.getSettingByName<bool>(
                    "cps")->value;

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"WASD Text Scale", textWidth * 6.9f,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::RelativeConstraint(0.12, "height", true),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        percent = FlarialGUI::Slider(8, toggleX + FlarialGUI::SettingsTextWidth("WASD Text Scale "),
                                     toggleY, this->settings.getSettingByName<float>("textscale")->value, 2.00);


        this->settings.getSettingByName<float>("textscale")->value = percent;

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"CPS Text Scale", textWidth * 6.9f,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::RelativeConstraint(0.12, "height", true),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        percent = FlarialGUI::Slider(12, toggleX + FlarialGUI::SettingsTextWidth("CPS Text Scale "),
                                     toggleY, this->settings.getSettingByName<float>("textscale2")->value, 2.00);


        this->settings.getSettingByName<float>("textscale2")->value = percent;

        /* Rounding End */

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX + Constraints::SpacingConstraint(0.60, textWidth), toggleY,
                                        L"Translucency", textWidth * 6.9f, textHeight,
                                        DWRITE_TEXT_ALIGNMENT_LEADING, Constraints::SpacingConstraint(1.05, textWidth),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        if (FlarialGUI::Toggle(4, toggleX, toggleY, this->settings.getSettingByName<bool>(
                "BlurEffect")->value))
            this->settings.getSettingByName<bool>("BlurEffect")->value = !this->settings.getSettingByName<bool>(
                    "BlurEffect")->value;

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"Space bar width", textWidth * 6.9,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::RelativeConstraint(0.12, "height", true),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        percent = FlarialGUI::Slider(9, toggleX + FlarialGUI::SettingsTextWidth("Space bar width "),
                                     toggleY, this->settings.getSettingByName<float>("spacebarWidth")->value, 1.00, 0,
                                     false);

        this->settings.getSettingByName<float>("spacebarWidth")->value = percent;

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"Space bar height", textWidth * 6.9,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::RelativeConstraint(0.12, "height", true),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        percent = FlarialGUI::Slider(10, toggleX + FlarialGUI::SettingsTextWidth("Space bar height "),
                                     toggleY, this->settings.getSettingByName<float>("spacebarHeight")->value, 1.00, 0,
                                     false);

        this->settings.getSettingByName<float>("spacebarHeight")->value = percent;

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"Key Spacing", textWidth * 6.9f,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::RelativeConstraint(0.12, "height", true),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        percent = FlarialGUI::Slider(11, toggleX + FlarialGUI::SettingsTextWidth("Key Spacing "),
                                     toggleY, this->settings.getSettingByName<float>("keySpacing")->value, 10.00);

        this->settings.getSettingByName<float>("keySpacing")->value = percent;

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX + Constraints::SpacingConstraint(0.60, textWidth), toggleY,
                                        L"\"LMB\" and \"RMB\"", textWidth * 6.9f,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::SpacingConstraint(1.05, textWidth),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        if (FlarialGUI::Toggle(10, toggleX, toggleY, this->settings.getSettingByName<bool>(
                "lmbrmb")->value))
            this->settings.getSettingByName<bool>("lmbrmb")->value = !this->settings.getSettingByName<bool>(
                    "lmbrmb")->value;

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX + Constraints::SpacingConstraint(0.60, textWidth), toggleY,
                                        L"Hide CPS", textWidth * 6.9f, textHeight,
                                        DWRITE_TEXT_ALIGNMENT_LEADING, Constraints::SpacingConstraint(1.05, textWidth),
                                        DWRITE_FONT_WEIGHT_NORMAL);

        if (FlarialGUI::Toggle(5, toggleX, toggleY, this->settings.getSettingByName<bool>(
                "hidecps")->value))
            this->settings.getSettingByName<bool>("hidecps")->value = !this->settings.getSettingByName<bool>(
                    "hidecps")->value;

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);
        FlarialGUI::TextBoxVisual(4, settings.getSettingByName<std::string>("lmbtext")->value, 16, toggleX, toggleY,
                                  "LMB Text");

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);
        FlarialGUI::TextBoxVisual(5, settings.getSettingByName<std::string>("rmbtext")->value, 16, toggleX, toggleY,
                                  "RMB Text");

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);
        FlarialGUI::TextBoxVisual(6, settings.getSettingByName<std::string>("wText")->value, 1, toggleX, toggleY,
                                  "W Key");

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);
        FlarialGUI::TextBoxVisual(7, settings.getSettingByName<std::string>("aText")->value, 1, toggleX, toggleY,
                                  "A Key");

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);
        FlarialGUI::TextBoxVisual(8, settings.getSettingByName<std::string>("sText")->value, 1, toggleX, toggleY,
                                  "S Key");

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);
        FlarialGUI::TextBoxVisual(9, settings.getSettingByName<std::string>("dText")->value, 1, toggleX, toggleY,
                                  "D Key");

        /* Color picker start */

        toggleX = Constraints::PercentageConstraint(0.55, "left");
        toggleY = Constraints::PercentageConstraint(0.10, "top");

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"Background", textWidth * 6.9f,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::SpacingConstraint(1.05, textWidth),
                                        DWRITE_FONT_WEIGHT_NORMAL);
        FlarialGUI::ColorPicker(0, toggleX + FlarialGUI::SettingsTextWidth("Background "),
                                toggleY - Constraints::SpacingConstraint(0.017, textWidth),
                                settings.getSettingByName<std::string>("bgColor")->value,
                                settings.getSettingByName<float>("bgOpacity")->value,
                                settings.getSettingByName<bool>("bgRGB")->value);

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"Text", textWidth * 6.9f,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::SpacingConstraint(1.05, textWidth),
                                        DWRITE_FONT_WEIGHT_NORMAL);
        FlarialGUI::ColorPicker(1, toggleX + FlarialGUI::SettingsTextWidth("Text "), toggleY * 0.99f,
                                settings.getSettingByName<std::string>("textColor")->value,
                                settings.getSettingByName<float>("textOpacity")->value,
                                settings.getSettingByName<bool>("textRGB")->value);

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"BG Pressed", textWidth * 6.9f,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::SpacingConstraint(1.05, textWidth),
                                        DWRITE_FONT_WEIGHT_NORMAL);
        FlarialGUI::ColorPicker(2, toggleX + FlarialGUI::SettingsTextWidth("BG Pressed "), toggleY * 0.99f,
                                settings.getSettingByName<std::string>("enabledColor")->value,
                                settings.getSettingByName<float>("enabledOpacity")->value,
                                settings.getSettingByName<bool>("enabledRGB")->value);

        toggleY += Constraints::SpacingConstraint(0.35, textWidth);

        FlarialGUI::FlarialTextWithFont(toggleX, toggleY, L"Text Pressed", textWidth * 6.9f,
                                        textHeight, DWRITE_TEXT_ALIGNMENT_LEADING,
                                        Constraints::SpacingConstraint(1.05, textWidth),
                                        DWRITE_FONT_WEIGHT_NORMAL);
        FlarialGUI::ColorPicker(3, toggleX + FlarialGUI::SettingsTextWidth("Text Pressed "), toggleY * 0.99f,
                                settings.getSettingByName<std::string>("textEnabledColor")->value,
                                settings.getSettingByName<float>("textEnabledOpacity")->value,
                                settings.getSettingByName<bool>("textEnabledRGB")->value);

        FlarialGUI::UnsetScrollView();

        FlarialGUI::ColorPickerWindow(0, settings.getSettingByName<std::string>("bgColor")->value,
                                      settings.getSettingByName<float>("bgOpacity")->value,
                                      settings.getSettingByName<bool>("bgRGB")->value);
        FlarialGUI::ColorPickerWindow(1, settings.getSettingByName<std::string>("textColor")->value,
                                      settings.getSettingByName<float>("textOpacity")->value,
                                      settings.getSettingByName<bool>("textRGB")->value);
        FlarialGUI::ColorPickerWindow(2, settings.getSettingByName<std::string>("enabledColor")->value,
                                      settings.getSettingByName<float>("enabledOpacity")->value,
                                      settings.getSettingByName<bool>("enabledRGB")->value);
        FlarialGUI::ColorPickerWindow(3, settings.getSettingByName<std::string>("textEnabledColor")->value,
                                      settings.getSettingByName<float>("textEnabledOpacity")->value,
                                      settings.getSettingByName<bool>("textEnabledRGB")->value);
        /* Color Pickers End */

    }

    static std::pair<float, float>
    centerChildRectangle(float parentWidth, float parentHeight, float childWidth, float childHeight) {
        return std::make_pair((parentWidth - childWidth) / 2, (parentHeight - childHeight) / 2);
    }

    void normalRender(int index, std::string& value) override {
        if (SDK::hasInstanced) {
            if (SDK::clientInstance->getLocalPlayer() != nullptr) {
                // lmb
                std::string lmbText = settings.getSettingByName<std::string>("lmbtext")->value;
                std::string uppercaseSentence;
                std::string search = "{VALUE}";

                for (char c: lmbText) uppercaseSentence += std::toupper(c);

                size_t pos = uppercaseSentence.find(search);
                if (pos != std::string::npos) {
                    lmbText.replace(pos, search.length(), std::to_string(CPSListener::GetLeftCPS()));
                }

                // rmb
                std::string rmbText = settings.getSettingByName<std::string>("rmbtext")->value;
                uppercaseSentence = "";
                for (char c: rmbText) uppercaseSentence += std::toupper(c);

                pos = uppercaseSentence.find(search);
                if (pos != std::string::npos) {
                    rmbText.replace(pos, search.length(), std::to_string(CPSListener::GetRightCPS()));
                }

                float keycardSize = Constraints::RelativeConstraint(
                        0.08f * this->settings.getSettingByName<float>("uiscale")->value, "height", true);
                float spacing =
                        this->settings.getSettingByName<float>("keySpacing")->value * Constraints::RelativeConstraint(
                                0.0015f * this->settings.getSettingByName<float>("uiscale")->value, "height", true);
                float textSize = this->settings.getSettingByName<float>("textscale")->value;
                float textSize2 = this->settings.getSettingByName<float>("textscale2")->value;

                Vec2<float> settingperc = Vec2<float>(this->settings.getSettingByName<float>("percentageX")->value,
                                                      this->settings.getSettingByName<float>("percentageY")->value);

                Vec2<float> realcenter;

                if (settingperc.x != 0)
                    realcenter = Vec2<float>(settingperc.x * MC::windowSize.x, settingperc.y * MC::windowSize.y);
                else realcenter = Constraints::CenterConstraint(keycardSize, keycardSize);

                Vec2<float> rounde = Constraints::RoundingConstraint(
                        this->settings.getSettingByName<float>("rounding")->value *
                        settings.getSettingByName<float>("uiscale")->value,
                        this->settings.getSettingByName<float>("rounding")->value *
                        settings.getSettingByName<float>("uiscale")->value);

                float totalWidth = keycardSize * 3 + spacing * 2;
                float totalHeight = keycardSize * 2.0f + keycardSize / 2.0f + spacing * 2;

                if (settings.getSettingByName<bool>("cps")->value) totalHeight += keycardSize + spacing;

                if (ModuleManager::getModule("ClickGUI")->isEnabled() ||
                    ClickGUIRenderer::editmenu) { // makes module movable
                    FlarialGUI::SetWindowRect(realcenter.x - (keycardSize + spacing), realcenter.y, totalWidth,
                                              totalHeight, index, keycardSize + spacing);

                    Vec2<float> vec2 = FlarialGUI::CalculateMovedXY(realcenter.x, realcenter.y, index, totalWidth,
                                                                    totalHeight);

                    realcenter.x = vec2.x;
                    realcenter.y = vec2.y;

                    realcenter = realcenter;

                    Vec2<float> percentages = Constraints::CalculatePercentage(realcenter.x, realcenter.y);

                    this->settings.setValue("percentageX", percentages.x);
                    this->settings.setValue("percentageY", percentages.y);
                }

                float fontSize = Constraints::SpacingConstraint(textSize * 3.0f, keycardSize);
                float fontSize2 = Constraints::SpacingConstraint(textSize2 * 3.0f, keycardSize);

                //todo only resave colors on settings render and once on setup

                D2D1_COLOR_F enabledColor = settings.getSettingByName<bool>("enabledRGB")->value ? FlarialGUI::rgbColor
                                                                                                 : FlarialGUI::HexToColorF(
                                settings.getSettingByName<std::string>("enabledColor")->value);
                D2D1_COLOR_F disabledColor = settings.getSettingByName<bool>("bgRGB")->value ? FlarialGUI::rgbColor
                                                                                             : FlarialGUI::HexToColorF(
                                settings.getSettingByName<std::string>("bgColor")->value);
                D2D1_COLOR_F textColor = settings.getSettingByName<bool>("textRGB")->value ? FlarialGUI::rgbColor
                                                                                           : FlarialGUI::HexToColorF(
                                settings.getSettingByName<std::string>("textColor")->value);
                D2D1_COLOR_F textEnabledColor = settings.getSettingByName<bool>("textEnabledRGB")->value
                                                ? FlarialGUI::rgbColor : FlarialGUI::HexToColorF(
                                settings.getSettingByName<std::string>("textEnabledColor")->value);

                disabledColor.a = settings.getSettingByName<float>("bgOpacity")->value;
                textColor.a = settings.getSettingByName<float>("textOpacity")->value;
                enabledColor.a = settings.getSettingByName<float>("enabledOpacity")->value;
                textEnabledColor.a = settings.getSettingByName<float>("textEnabledOpacity")->value;

                MoveInputComponent *handler = SDK::clientInstance->getLocalPlayer()->getMoveInputHandler();

                if (handler->forward) {
                    states[Strokes::W] = FlarialGUI::LerpColor(states[Strokes::W], enabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::W] = FlarialGUI::LerpColor(textStates[Strokes::W], textEnabledColor,
                                                                   0.15f * FlarialGUI::frameFactor);
                } else {
                    states[Strokes::W] = FlarialGUI::LerpColor(states[Strokes::W], disabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::W] = FlarialGUI::LerpColor(textStates[Strokes::W], textColor,
                                                                   0.15f * FlarialGUI::frameFactor);
                }

                if (handler->backward) {
                    states[Strokes::S] = FlarialGUI::LerpColor(states[Strokes::S], enabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::S] = FlarialGUI::LerpColor(textStates[Strokes::S], textEnabledColor,
                                                                   0.15f * FlarialGUI::frameFactor);
                } else {
                    states[Strokes::S] = FlarialGUI::LerpColor(states[Strokes::S], disabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::S] = FlarialGUI::LerpColor(textStates[Strokes::S], textColor,
                                                                   0.15f * FlarialGUI::frameFactor);
                }

                if (handler->left) {
                    states[Strokes::A] = FlarialGUI::LerpColor(states[Strokes::A], enabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::A] = FlarialGUI::LerpColor(textStates[Strokes::A], textEnabledColor,
                                                                   0.15f * FlarialGUI::frameFactor);
                } else {
                    states[Strokes::A] = FlarialGUI::LerpColor(states[Strokes::A], disabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::A] = FlarialGUI::LerpColor(textStates[Strokes::A], textColor,
                                                                   0.15f * FlarialGUI::frameFactor);
                }

                if (handler->right) {
                    states[Strokes::D] = FlarialGUI::LerpColor(states[Strokes::D], enabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::D] = FlarialGUI::LerpColor(textStates[Strokes::D], textEnabledColor,
                                                                   0.15f * FlarialGUI::frameFactor);
                } else {
                    states[Strokes::D] = FlarialGUI::LerpColor(states[Strokes::D], disabledColor,
                                                               0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::D] = FlarialGUI::LerpColor(textStates[Strokes::D], textColor,
                                                                   0.15f * FlarialGUI::frameFactor);
                }

                if (handler->jumping) {
                    states[Strokes::SPACEBAR] = FlarialGUI::LerpColor(states[Strokes::SPACEBAR], enabledColor,
                                                                      0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::SPACEBAR] = FlarialGUI::LerpColor(textStates[Strokes::SPACEBAR],
                                                                          textEnabledColor,
                                                                          0.15f * FlarialGUI::frameFactor);
                } else {
                    states[Strokes::SPACEBAR] = FlarialGUI::LerpColor(states[Strokes::SPACEBAR], disabledColor,
                                                                      0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::SPACEBAR] = FlarialGUI::LerpColor(textStates[Strokes::SPACEBAR], textColor,
                                                                          0.15f * FlarialGUI::frameFactor);
                }

                if (CPSListener::GetRightHeld()) {
                    states[Strokes::RMB] = FlarialGUI::LerpColor(states[Strokes::RMB], enabledColor,
                                                                 0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::RMB] = FlarialGUI::LerpColor(textStates[Strokes::RMB], textEnabledColor,
                                                                     0.15f * FlarialGUI::frameFactor);
                } else {
                    states[Strokes::RMB] = FlarialGUI::LerpColor(states[Strokes::RMB], disabledColor,
                                                                 0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::RMB] = FlarialGUI::LerpColor(textStates[Strokes::RMB], textColor,
                                                                     0.15f * FlarialGUI::frameFactor);
                }

                if (CPSListener::GetLeftHeld()) {
                    states[Strokes::LMB] = FlarialGUI::LerpColor(states[Strokes::LMB], enabledColor,
                                                                 0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::LMB] = FlarialGUI::LerpColor(textStates[Strokes::LMB], textEnabledColor,
                                                                     0.15f * FlarialGUI::frameFactor);
                } else {
                    states[Strokes::LMB] = FlarialGUI::LerpColor(states[Strokes::LMB], disabledColor,
                                                                 0.15f * FlarialGUI::frameFactor);
                    textStates[Strokes::LMB] = FlarialGUI::LerpColor(textStates[Strokes::LMB], textColor,
                                                                     0.15f * FlarialGUI::frameFactor);
                }

                // W
                if (settings.getSettingByName<bool>("BlurEffect") != nullptr)
                    if (settings.getSettingByName<bool>("BlurEffect")->value) FlarialGUI::BlurRect(D2D1::RoundedRect(
                                                                                                           D2D1::RectF(realcenter.x, realcenter.y, realcenter.x + keycardSize,
                                                                                                                       realcenter.y + keycardSize), rounde.x, rounde.x),
                                                                                                   Client::settings.getSettingByName<float>(
                                                                                                           "blurintensity")->value);

                FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::W], keycardSize, keycardSize,
                                        rounde.x,
                                        rounde.x);
                FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, FlarialGUI::to_wide(
                                                        settings.getSettingByName<std::string>("wText")->value).c_str(), keycardSize, keycardSize,
                                                DWRITE_TEXT_ALIGNMENT_CENTER, fontSize, DWRITE_FONT_WEIGHT_NORMAL,
                                                textStates[Strokes::W], true);

                // S

                realcenter.y += (keycardSize + spacing);

                if (settings.getSettingByName<bool>("BlurEffect") != nullptr)
                    if (settings.getSettingByName<bool>("BlurEffect")->value) FlarialGUI::BlurRect(D2D1::RoundedRect(
                                                                                                           D2D1::RectF(realcenter.x, realcenter.y, realcenter.x + keycardSize,
                                                                                                                       realcenter.y + keycardSize), rounde.x, rounde.x),
                                                                                                   Client::settings.getSettingByName<float>(
                                                                                                           "blurintensity")->value);

                FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::S], keycardSize, keycardSize,
                                        rounde.x,
                                        rounde.x);

                FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, FlarialGUI::to_wide(
                                                        settings.getSettingByName<std::string>("sText")->value).c_str(), keycardSize, keycardSize,
                                                DWRITE_TEXT_ALIGNMENT_CENTER, fontSize, DWRITE_FONT_WEIGHT_NORMAL,
                                                textStates[Strokes::S], true);
                // A
                realcenter.x -= (keycardSize + spacing);

                if (settings.getSettingByName<bool>("BlurEffect") != nullptr)
                    if (settings.getSettingByName<bool>("BlurEffect")->value) FlarialGUI::BlurRect(D2D1::RoundedRect(
                                                                                                           D2D1::RectF(realcenter.x, realcenter.y, realcenter.x + keycardSize,
                                                                                                                       realcenter.y + keycardSize), rounde.x, rounde.x),
                                                                                                   Client::settings.getSettingByName<float>(
                                                                                                           "blurintensity")->value);
                FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::A], keycardSize, keycardSize,
                                        rounde.x,
                                        rounde.x);
                FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, FlarialGUI::to_wide(
                                                        settings.getSettingByName<std::string>("aText")->value).c_str(), keycardSize, keycardSize,
                                                DWRITE_TEXT_ALIGNMENT_CENTER, fontSize, DWRITE_FONT_WEIGHT_NORMAL,
                                                textStates[Strokes::A], true);

                // D
                realcenter.x += 2 * (keycardSize + spacing);

                if (settings.getSettingByName<bool>("BlurEffect") != nullptr)
                    if (settings.getSettingByName<bool>("BlurEffect")->value) FlarialGUI::BlurRect(D2D1::RoundedRect(
                                                                                                           D2D1::RectF(realcenter.x, realcenter.y, realcenter.x + keycardSize,
                                                                                                                       realcenter.y + keycardSize), rounde.x, rounde.x),
                                                                                                   Client::settings.getSettingByName<float>(
                                                                                                           "blurintensity")->value);

                FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::D], keycardSize, keycardSize,
                                        rounde.x,
                                        rounde.x);
                FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, FlarialGUI::to_wide(
                                                        settings.getSettingByName<std::string>("dText")->value).c_str(), keycardSize, keycardSize,
                                                DWRITE_TEXT_ALIGNMENT_CENTER, fontSize, DWRITE_FONT_WEIGHT_NORMAL,
                                                textStates[Strokes::D], true);

                // SPACEBAR
                float spacebarWidth = 3 * (keycardSize) + 2 * spacing;
                float spacebarHeight = 0.55f * (keycardSize);
                realcenter.x -= 2 * (keycardSize + spacing);

                bool hideCPS = settings.getSettingByName<bool>("hidecps")->value;


                if (!settings.getSettingByName<bool>("cps")->value) realcenter.y += (keycardSize + spacing);
                else {

                    realcenter.y += (keycardSize + spacing);
                    // LMB
                    if (settings.getSettingByName<bool>("BlurEffect") != nullptr)
                        if (settings.getSettingByName<bool>("BlurEffect")->value) FlarialGUI::BlurRect(
                                    D2D1::RoundedRect(D2D1::RectF(realcenter.x, realcenter.y,
                                                                  realcenter.x + keycardSize + (keycardSize / 2.0f) +
                                                                  spacing / 2.0f,
                                                                  realcenter.y + keycardSize - (keycardSize * 0.05)),
                                                      rounde.x, rounde.x),
                                    Client::settings.getSettingByName<float>("blurintensity")->value);

                    FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::LMB],
                                            keycardSize + (keycardSize / 2.0f) + spacing / 2.0f,
                                            keycardSize - (keycardSize * 0.05),
                                            rounde.x,
                                            rounde.x);
                    if (settings.getSettingByName<bool>("lmbrmb")->value) {
                        FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, L"LMB",
                                                        keycardSize + (keycardSize / 2.0f) + spacing / 2.0f,
                                                        Constraints::SpacingConstraint(hideCPS ? 1.0f : 0.65f,
                                                                                       keycardSize -
                                                                                       (keycardSize * 0.05)),
                                                        DWRITE_TEXT_ALIGNMENT_CENTER,
                                                        fontSize2 + Constraints::SpacingConstraint(0.48, keycardSize),
                                                        DWRITE_FONT_WEIGHT_NORMAL, textStates[Strokes::LMB], true);
                        if (!hideCPS)
                            FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y +
                                                                          Constraints::SpacingConstraint(0.55,
                                                                                                         keycardSize -
                                                                                                         (keycardSize *
                                                                                                          0.05)),
                                                            FlarialGUI::to_wide(lmbText).c_str(),
                                                            keycardSize + (keycardSize / 2.0f) +
                                                            spacing / 2.0f,
                                                            Constraints::SpacingConstraint(0.35, keycardSize -
                                                                                                 (keycardSize *
                                                                                                  0.05)),
                                                            DWRITE_TEXT_ALIGNMENT_CENTER, fontSize2 +
                                                                                          Constraints::SpacingConstraint(
                                                                                                  -0.96f,
                                                                                                  keycardSize),
                                                            DWRITE_FONT_WEIGHT_NORMAL, textStates[Strokes::LMB], true);
                    } else
                        FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, FlarialGUI::to_wide(
                                                                std::to_string(CPSListener::GetLeftCPS())).c_str(),
                                                        keycardSize + (keycardSize / 2.0f) + spacing / 2.0f,
                                                        keycardSize - (keycardSize * 0.05),
                                                        DWRITE_TEXT_ALIGNMENT_CENTER, fontSize2 +
                                                                                      Constraints::SpacingConstraint(
                                                                                              0.48, keycardSize),
                                                        DWRITE_FONT_WEIGHT_NORMAL, textStates[Strokes::LMB], true);

                    // RMB
                    realcenter.x += 1.5f * (keycardSize + spacing);

                    if (settings.getSettingByName<bool>("BlurEffect") != nullptr)
                        if (settings.getSettingByName<bool>("BlurEffect")->value) FlarialGUI::BlurRect(
                                    D2D1::RoundedRect(D2D1::RectF(realcenter.x, realcenter.y,
                                                                  realcenter.x + keycardSize + (keycardSize / 2.0f) +
                                                                  spacing / 2.0f,
                                                                  realcenter.y + keycardSize - (keycardSize * 0.05)),
                                                      rounde.x, rounde.x),
                                    Client::settings.getSettingByName<float>("blurintensity")->value);
                    FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::RMB],
                                            keycardSize + (keycardSize / 2) + spacing / 2.0f,
                                            keycardSize - (keycardSize * 0.05),
                                            rounde.x,
                                            rounde.x);
                    if (settings.getSettingByName<bool>("lmbrmb")->value) {
                        FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, L"RMB",
                                                        keycardSize + (keycardSize / 2.0f) + spacing / 2.0f,
                                                        Constraints::SpacingConstraint(hideCPS ? 1.0f : 0.65f,
                                                                                       keycardSize -
                                                                                       (keycardSize * 0.05)),
                                                        DWRITE_TEXT_ALIGNMENT_CENTER,
                                                        fontSize2 + Constraints::SpacingConstraint(0.48, keycardSize),
                                                        DWRITE_FONT_WEIGHT_NORMAL, textStates[Strokes::RMB], true);
                        if (!hideCPS)
                            FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y +
                                                                          Constraints::SpacingConstraint(0.55,
                                                                                                         keycardSize -
                                                                                                         (keycardSize *
                                                                                                          0.05)),
                                                            FlarialGUI::to_wide(rmbText).c_str(),
                                                            keycardSize + (keycardSize / 2.0f) +
                                                            spacing / 2.0f,
                                                            Constraints::SpacingConstraint(0.35, keycardSize -
                                                                                                 (keycardSize *
                                                                                                  0.05)),
                                                            DWRITE_TEXT_ALIGNMENT_CENTER, fontSize2 +
                                                                                          Constraints::SpacingConstraint(
                                                                                                  -0.96f,
                                                                                                  keycardSize),
                                                            DWRITE_FONT_WEIGHT_NORMAL, textStates[Strokes::RMB], true);
                    } else
                        FlarialGUI::FlarialTextWithFont(realcenter.x, realcenter.y, FlarialGUI::to_wide(
                                                                std::to_string(CPSListener::GetRightCPS())).c_str(),
                                                        keycardSize + (keycardSize / 2.0f) + spacing / 2.0f,
                                                        keycardSize - (keycardSize * 0.05),
                                                        DWRITE_TEXT_ALIGNMENT_CENTER, fontSize2 +
                                                                                      Constraints::SpacingConstraint(
                                                                                              0.48, keycardSize),
                                                        DWRITE_FONT_WEIGHT_NORMAL, textStates[Strokes::RMB], true);
                    realcenter.y += keycardSize - keycardSize * 0.05 + spacing;
                    realcenter.x -= 1.5f * (keycardSize + spacing);
                }

                if (settings.getSettingByName<bool>("BlurEffect") != nullptr)
                    if (settings.getSettingByName<bool>("BlurEffect")->value) FlarialGUI::BlurRect(D2D1::RoundedRect(
                                                                                                           D2D1::RectF(realcenter.x, realcenter.y, realcenter.x + spacebarWidth,
                                                                                                                       realcenter.y + spacebarHeight), rounde.x, rounde.x),
                                                                                                   Client::settings.getSettingByName<float>(
                                                                                                           "blurintensity")->value);
                FlarialGUI::RoundedRect(realcenter.x, realcenter.y, states[Strokes::SPACEBAR], spacebarWidth,
                                        spacebarHeight, rounde.x, rounde.x);
                float childHeight = Constraints::SpacingConstraint(
                        this->settings.getSettingByName<float>("spacebarHeight")->value, spacebarHeight);
                float childWidth = Constraints::SpacingConstraint(
                        this->settings.getSettingByName<float>("spacebarWidth")->value, spacebarWidth);
                std::pair<float, float> centeredChild = centerChildRectangle(spacebarWidth, spacebarHeight, childWidth,
                                                                             childHeight);
                FlarialGUI::RoundedRect(realcenter.x + centeredChild.first, realcenter.y + centeredChild.second,
                                        textStates[Strokes::SPACEBAR], childWidth, childHeight, 0, 0);

                if (ModuleManager::getModule("ClickGUI")->isEnabled() ||
                    ClickGUIRenderer::editmenu)

                    FlarialGUI::UnsetWindowRect();

            }
        }
    }
};

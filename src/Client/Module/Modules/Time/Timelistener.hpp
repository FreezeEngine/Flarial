#pragma once

#include "../../../../SDK/SDK.hpp"
#include <format>
#include "../../../Events/Listener.hpp"
#include "../../../Events/Input/KeyEvent.hpp"
#include "../Module.hpp"
#include "../../../GUI/Engine/Engine.hpp"
#include <Windows.h>
#include <ctime>

inline std::tm localtime_xp(std::time_t timer) {
    std::tm bt{};
#if defined(__unix__)
    localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
    localtime_s(&bt, &timer);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    bt = *std::localtime(&timer);
#endif
    return bt;
}

class TimeListener : public Listener {

    Module *module;

    void onRender(RenderEvent &event) override {

            const auto now = std::time(nullptr);
            const std::tm calendarTime = localtime_xp(std::time(nullptr));

            std::string meridiem;
            std::string seperator;

            int hour = calendarTime.tm_hour;
            int minute = calendarTime.tm_min;

            if (hour - 12 < 0) {
                meridiem = "AM";
            } else if (hour == 0) {
                hour = 12;
                meridiem = "AM";
            } else if (hour == 12) {
                hour = 12;
                meridiem = "PM";
            } else {
                meridiem = "PM";
                hour -= 12;
            }
            if (module->settings.getSettingByName<bool>("24")->value && meridiem == "PM") {
                hour += 12;
                meridiem = "";
            } else if (module->settings.getSettingByName<bool>("24")->value && meridiem == "AM") meridiem = "";

            seperator = minute < 10 ? ":0" : ":";

            std::string time = std::to_string(hour) + seperator + std::to_string(minute) + " " + meridiem;

        this->module->normalRender(3, time);

            }


public:
    explicit TimeListener(const char string[5], Module *module) {
        this->name = string;
        this->module = module;
    }

};


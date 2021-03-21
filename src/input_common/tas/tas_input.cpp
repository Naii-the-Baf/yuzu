// Copyright 2021 yuzu Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.


#include <chrono>
#include <cstring>
#include <functional>
#include <random>
#include <thread>
#include <boost/asio.hpp>

#include "core/settings.h"
#include "common/logging/log.h"
#include "input_common/tas/tas_input.h"
#include "common/file_util.h"

namespace TasInput {

Tas::Tas() {
    update_thread = std::thread(&Tas::UpdateThread, this);
    std::string file="";
    Common::FS::ReadFileToString(true, "C:/Users/fabian/Desktop/naiidevelopment/example.txt", file);
    std::stringstream command_line(file);
    std::string line;
    while (std::getline(command_line, line, '\n')) {
        LOG_ERROR(Input, "{}", line);
    }
}

Tas::~Tas() {
    update_thread_running = false;
    if (update_thread.joinable()) {
        update_thread.join();
    }
}

void Tas::UpdateThread() {
    constexpr int update_time = 10;
    while (update_thread_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(update_time));
    }
}

InputCommon::ButtonMapping Tas::GetButtonMappingForDevice(
    const Common::ParamPackage& params) const {
    // This list is missing ZL/ZR since those are not considered buttons.
    // We will add those afterwards
    // This list also excludes any button that can't be really mapped
    static constexpr std::array<std::pair<Settings::NativeButton::Values, TasButton>, 20>
        switch_to_gcadapter_button = {
            std::pair{Settings::NativeButton::A, TasButton::BUTTON_B},
            {Settings::NativeButton::B, TasButton::BUTTON_A},
            {Settings::NativeButton::X, TasButton::BUTTON_Y},
            {Settings::NativeButton::Y, TasButton::BUTTON_X},
            {Settings::NativeButton::LStick, TasButton::STICK_L},
            {Settings::NativeButton::RStick, TasButton::STICK_R},
            {Settings::NativeButton::L, TasButton::TRIGGER_L},
            {Settings::NativeButton::R, TasButton::TRIGGER_R},
            {Settings::NativeButton::Plus, TasButton::BUTTON_PLUS},
            {Settings::NativeButton::Minus, TasButton::BUTTON_MINUS},
            {Settings::NativeButton::DLeft, TasButton::BUTTON_LEFT},
            {Settings::NativeButton::DUp, TasButton::BUTTON_UP},
            {Settings::NativeButton::DRight, TasButton::BUTTON_RIGHT},
            {Settings::NativeButton::DDown, TasButton::BUTTON_DOWN},
            {Settings::NativeButton::SL, TasButton::BUTTON_L_SL},
            {Settings::NativeButton::SR, TasButton::BUTTON_L_SR},
            {Settings::NativeButton::Screenshot, TasButton::BUTTON_CAPTURE},
            {Settings::NativeButton::Home, TasButton::BUTTON_HOME},
            {Settings::NativeButton::ZL, TasButton::TRIGGER_ZL},
            {Settings::NativeButton::ZR, TasButton::TRIGGER_ZR},
        };

    InputCommon::ButtonMapping mapping{};
    for (const auto& [switch_button, gcadapter_button] : switch_to_gcadapter_button) {
        Common::ParamPackage button_params({{"engine", "tas"}});
        button_params.Set("pad", params.Get("pad", 0));
        button_params.Set("button", static_cast<int>(gcadapter_button));
        mapping.insert_or_assign(switch_button, std::move(button_params));
    }

    return mapping;
}

InputCommon::AnalogMapping Tas::GetAnalogMappingForDevice(
    const Common::ParamPackage& params) const {

    InputCommon::AnalogMapping mapping = {};
    Common::ParamPackage left_analog_params;
    left_analog_params.Set("engine", "tas");
    left_analog_params.Set("pad", params.Get("pad", 0));
    left_analog_params.Set("axis_x", static_cast<int>(TasAxes::StickX));
    left_analog_params.Set("axis_y", static_cast<int>(TasAxes::StickY));
    mapping.insert_or_assign(Settings::NativeAnalog::LStick, std::move(left_analog_params));
    Common::ParamPackage right_analog_params;
    right_analog_params.Set("engine", "tas");
    right_analog_params.Set("pad", params.Get("pad", 0));
    right_analog_params.Set("axis_x", static_cast<int>(TasAxes::SubstickX));
    right_analog_params.Set("axis_y", static_cast<int>(TasAxes::SubstickY));
    mapping.insert_or_assign(Settings::NativeAnalog::RStick, std::move(right_analog_params));
    return mapping;
}

TasData& Tas::GetTasState(std::size_t pad) {
    return tas_data[pad];
}

const TasData& Tas::GetTasState(std::size_t pad) const {
    return tas_data[pad];
}
} // namespace MouseInput

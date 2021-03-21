// Copyright 2020 yuzu Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <mutex>
#include <thread>

#include "common/common_types.h"
#include "core/frontend/input.h"
#include "input_common/main.h"

namespace TasInput {

enum class TasButton {
    BUTTON_DOWN = 0x000001,
    BUTTON_UP = 0x000002,
    BUTTON_RIGHT = 0x000004,
    BUTTON_LEFT = 0x000008,
    BUTTON_L_SR = 0x000010,
    BUTTON_L_SL = 0x000020,
    TRIGGER_L = 0x000040,
    TRIGGER_ZL = 0x000080,
    BUTTON_Y = 0x000100,
    BUTTON_X = 0x000200,
    BUTTON_B = 0x000400,
    BUTTON_A = 0x000800,
    BUTTON_R_SL = 0x001000,
    BUTTON_R_SR = 0x002000,
    TRIGGER_R = 0x004000,
    TRIGGER_ZR = 0x008000,
    BUTTON_MINUS = 0x010000,
    BUTTON_PLUS = 0x020000,
    STICK_R = 0x040000,
    STICK_L = 0x080000,
    BUTTON_HOME = 0x100000,
    BUTTON_CAPTURE = 0x200000,
};

enum class TasAxes : u8 {
    StickX,
    StickY,
    SubstickX,
    SubstickY,
    Undefined,
};

struct TasData {
    u32 buttons{};
    std::array<float, 4> axis{};
};

class Tas {
public:
    Tas();
    ~Tas();

    InputCommon::ButtonMapping GetButtonMappingForDevice(const Common::ParamPackage& params) const;
    InputCommon::AnalogMapping GetAnalogMappingForDevice(const Common::ParamPackage& params) const;
    [[nodiscard]] TasData& GetTasState(std::size_t pad);
    [[nodiscard]] const TasData& GetTasState(std::size_t pad) const;

private:
    void UpdateThread();

    std::thread update_thread;
    std::array<TasData, 7> tas_data;
    bool update_thread_running{true};
};
} // namespace MouseInput

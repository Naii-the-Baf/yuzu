// Copyright 2018 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <atomic>
#include "common/bit_field.h"
#include "common/common_types.h"
#include "core/frontend/input.h"
#include "core/hle/kernel/object.h"
#include "core/hle/service/hid/controllers/controller_base.h"
#include "core/settings.h"

namespace Service::HID {
class Controller_ConsoleSixAxis final : public ControllerBase {
public:
    explicit Controller_ConsoleSixAxis(Core::System& system);
    ~Controller_ConsoleSixAxis() override;

    // Called when the controller is initialized
    void OnInit() override;

    // When the controller is released
    void OnRelease() override;

    // When the controller is requesting an update for the shared memory
    void OnUpdate(const Core::Timing::CoreTiming& core_timing, u8* data, size_t size) override;

    // Called when input devices should be loaded
    void OnLoadInputDevices() override;

private:
    struct SevenSixAxisState {
        INSERT_PADDING_BYTES(0x10); // unused
        s64_le sampling_number;
        s64_le sampling_number2;
        u64 unknown;
        std::array<f32, 10> data;
    };
    static_assert(sizeof(SevenSixAxisState) == 0x50, "SevenSixAxisState is an invalid size");

    struct SevenSixAxisMemory {
        CommonHeader header;
        std::array<SevenSixAxisState, 0x21> sevensixaxis_states;
    };
    static_assert(sizeof(SevenSixAxisMemory) == 0xA70, "SevenSixAxisMemory is an invalid size");

    struct SharedMemory {
        u64_le sampling_number;
        bool is_seven_six_axis_sensor_at_rest;
        f32 verticalization_error;
        Common::Vec3f gyro_bias{};
        // CommonHeader header;
        // std::array<GestureState, 17> gesture_states;
    };
    // static_assert(sizeof(SharedMemory) == 0x20, "SharedMemory is an invalid size");

    SharedMemory shared_memory{};
    SevenSixAxisMemory seven_six_axis_memory{};
};
} // namespace Service::HID
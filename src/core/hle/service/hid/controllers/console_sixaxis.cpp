// Copyright 2018 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <cstring>
#include "common/common_types.h"
#include "core/core_timing.h"
#include "core/hle/service/hid/controllers/console_sixaxis.h"

namespace Service::HID {
constexpr std::size_t SHARED_MEMORY_OFFSET = 0x3C200;

Controller_ConsoleSixAxis::Controller_ConsoleSixAxis(Core::System& system)
    : ControllerBase(system) {}
Controller_ConsoleSixAxis::~Controller_ConsoleSixAxis() = default;

void Controller_ConsoleSixAxis::OnInit() {}

void Controller_ConsoleSixAxis::OnRelease() {}

void Controller_ConsoleSixAxis::OnUpdate(const Core::Timing::CoreTiming& core_timing, u8* data,
                                         std::size_t size) {
    seven_six_axis_memory.header.timestamp = core_timing.GetCPUTicks();
    seven_six_axis_memory.header.total_entry_count = 17;

    if (!IsControllerActivated()) {
        seven_six_axis_memory.header.entry_count = 0;
        seven_six_axis_memory.header.last_entry_index = 0;
        return;
    }
    seven_six_axis_memory.header.entry_count = 16;

    const auto& last_entry =
        seven_six_axis_memory.sevensixaxis_states[seven_six_axis_memory.header.last_entry_index];
    seven_six_axis_memory.header.last_entry_index =
        (seven_six_axis_memory.header.last_entry_index + 1) % 17;
    auto& cur_entry =
        seven_six_axis_memory.sevensixaxis_states[seven_six_axis_memory.header.last_entry_index];

    cur_entry.sampling_number = last_entry.sampling_number + 1;
    cur_entry.sampling_number2 = cur_entry.sampling_number;
    // TODO(ogniK): Update gesture states

    shared_memory.sampling_number++;
    shared_memory.is_seven_six_axis_sensor_at_rest = true;
    shared_memory.verticalization_error = 0.1f;
    //   shared_memory.gyro = {0.1f, 0.2f, 0.3f};
    std::memcpy(data + SHARED_MEMORY_OFFSET, &shared_memory, sizeof(SharedMemory));
}

void Controller_ConsoleSixAxis::OnLoadInputDevices() {}
} // namespace Service::HID
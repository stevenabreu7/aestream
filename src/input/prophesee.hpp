#pragma once

#include <atomic>

#include <metavision/sdk/base/events/event_cd.h>
#include <metavision/sdk/driver/camera.h>

#include "../aedat.hpp"
#include "../generator.hpp"

Generator<AEDAT::PolarityEvent>
prophesee_event_generator(const std::optional<std::string> serial_number, const std::atomic<bool> &runFlag);
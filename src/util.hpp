#pragma once

#include <QString>

namespace util {

inline QString intToAutonomousState(const int state) {
    switch (state) {
    case 0:
        return "No Autonomous State";
    case 1:
        return "Off";
    case 2:
        return "Ready";
    case 3:
        return "Driving";
    case 4:
        return "Emergency";
    case 5:
        return "Finished";
    }
    return "INVALID";
}

inline QString intToMission(const int mission) {
    switch (mission) {
    case 0:
        return "No Mission Selected";
    case 1:
        return "Acceleration";
    case 2:
        return "SkidPad";
    case 3:
        return "Track Drive";
    case 4:
        return "Brake Test";
    case 5:
        return "Inspection";
    case 6:
        return "AutoCross";
    }
    return "INVALID";
}

inline QString intToEbsState(const int state) {
    switch (state) {
    case 0:
        return "No EBS State";
    case 1:
        return "Deactivated";
    case 2:
        return "Armed";
    case 3:
        return "Activated";
    }
    return "INVALID";
}

inline float normalizeSlider(int rawValue) {
    if (rawValue > 127) {
        rawValue -= 256;
    }
    return static_cast<float>(rawValue);
}

} // namespace util

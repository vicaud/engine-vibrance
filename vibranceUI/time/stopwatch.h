#pragma once

#include <cstdint>
#include <string>

enum class StopwatchStatus
{
    eIdle,
    eRunning,
    ePaused
};

struct StopwatchSnapshot
{
    StopwatchStatus status;
    std::uint32_t elapsedSeconds;
};

class Stopwatch
{
public:
    Stopwatch();

    bool start(double currentTimeSeconds);
    bool pause(double currentTimeSeconds);
    bool resume(double currentTimeSeconds);
    bool toggle_pause(double currentTimeSeconds);
    void reset();

    [[nodiscard]] StopwatchSnapshot snapshot(double currentTimeSeconds) const;
    [[nodiscard]] StopwatchStatus status() const;
    [[nodiscard]] bool active() const;

private:
    [[nodiscard]] double elapsed_exact(double currentTimeSeconds) const;

    StopwatchStatus stopwatchStatus{StopwatchStatus::eIdle};
    double elapsedAtAnchorSeconds{0.0};
    double anchorTimeSeconds{0.0};
};

std::string format_stopwatch_time(std::uint32_t totalSeconds);
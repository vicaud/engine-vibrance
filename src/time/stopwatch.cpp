#include <vibranceUI/time/stopwatch.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

Stopwatch::Stopwatch()
    : stopwatchStatus(StopwatchStatus::eIdle),
      elapsedAtAnchorSeconds(0.0),
      anchorTimeSeconds(0.0)
{
}

bool Stopwatch::start(double currentTimeSeconds)
{
    stopwatchStatus = StopwatchStatus::eRunning;
    elapsedAtAnchorSeconds = 0.0;
    anchorTimeSeconds = currentTimeSeconds;
    return true;
}

bool Stopwatch::pause(double currentTimeSeconds)
{
    if (stopwatchStatus != StopwatchStatus::eRunning)
    {
        return false;
    }
    elapsedAtAnchorSeconds = elapsed_exact(currentTimeSeconds);
    anchorTimeSeconds = currentTimeSeconds;
    stopwatchStatus = StopwatchStatus::ePaused;
    return true;
}

bool Stopwatch::resume(double currentTimeSeconds)
{
    if (stopwatchStatus != StopwatchStatus::ePaused)
    {
        return false;
    }
    anchorTimeSeconds = currentTimeSeconds;
    stopwatchStatus = StopwatchStatus::eRunning;
    return true;
}

bool Stopwatch::toggle_pause(double currentTimeSeconds)
{
    return stopwatchStatus == StopwatchStatus::ePaused ?
        resume(currentTimeSeconds) : pause(currentTimeSeconds);
}

void Stopwatch::reset()
{
    stopwatchStatus = StopwatchStatus::eIdle;
    elapsedAtAnchorSeconds = 0.0;
    anchorTimeSeconds = 0.0;
}

StopwatchSnapshot Stopwatch::snapshot(double currentTimeSeconds) const
{
    const double exact = elapsed_exact(currentTimeSeconds);
    const std::uint32_t elapsed = static_cast<std::uint32_t>(std::floor(exact));

    StopwatchSnapshot value = {};
    value.status = stopwatchStatus;
    value.elapsedSeconds = elapsed;
    return value;
}

StopwatchStatus Stopwatch::status() const
{
    return stopwatchStatus;
}

bool Stopwatch::active() const
{
    return stopwatchStatus != StopwatchStatus::eIdle;
}

double Stopwatch::elapsed_exact(double currentTimeSeconds) const
{
    if (stopwatchStatus == StopwatchStatus::eRunning)
    {
        return elapsedAtAnchorSeconds + std::max(currentTimeSeconds - anchorTimeSeconds, 0.0);
    }
    if (stopwatchStatus == StopwatchStatus::ePaused)
    {
        return elapsedAtAnchorSeconds;
    }
    return 0.0;
}

std::string format_stopwatch_time(std::uint32_t totalSeconds)
{
    const std::uint32_t hours = totalSeconds / 3600u;
    const std::uint32_t minutes = (totalSeconds % 3600u) / 60u;
    const std::uint32_t seconds = totalSeconds % 60u;

    std::ostringstream stream;
    if (hours > 0u)
    {
        stream << hours << ':'
               << std::setfill('0') << std::setw(2) << minutes << ':'
               << std::setw(2) << seconds;
    }
    else
    {
        stream << minutes << ':'
               << std::setfill('0') << std::setw(2) << seconds;
    }
    return stream.str();
}
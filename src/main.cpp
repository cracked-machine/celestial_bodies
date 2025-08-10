
#include <engine.hpp>

// set level before including logger
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#include <logging/basic_log_controller.hpp>

#include <memory>

int main()
{
    // logging: make sure errors - exceptions and failed asserts - go to log file
    fclose(stderr);

    using Logger = CelestialBodies::Logging::BasicLogController;
    std::unique_ptr<Logger> logger{ std::make_unique<Logger>("logger", "log.txt") };

    SPDLOG_INFO("Starting...");

    CelestialBodies::Engine engine;
    engine.run();

    return 0;
}
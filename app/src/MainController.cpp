#include "MainController.hpp"

#include "engine/platform/PlatformController.hpp"

void MainController::initialize() {
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}
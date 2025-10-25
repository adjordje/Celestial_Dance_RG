#include "MyApp.hpp"

#include "spdlog/spdlog.h"
#include "MainController.hpp"

#include "GUIController.hpp"

void MyApp::app_setup() {
    spdlog::info("App setup completed!");
    auto main_controller = register_controller<MainController>();
    auto gui_controller = register_controller<GUIController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    main_controller->before(gui_controller);
}
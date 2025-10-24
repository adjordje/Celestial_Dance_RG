#include "MyApp.hpp"

#include "spdlog/spdlog.h"
#include "MainController.hpp"

void MyApp::app_setup() {
    spdlog::info("App setup completed!");
    auto main_controller = register_controller<MainController>();
}
#include "GUIController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/platform/PlatformController.hpp"
#include "imgui.h"

#include "MainController.hpp"

void GUIController::initialize() {
    set_enable(false);
}

void GUIController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_F2).state() == engine::platform::Key::State::JustPressed) {
        set_enable(!is_enabled());
    }
}

void GUIController::draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    auto mainController = engine::core::Controller::get<MainController>();
    glm::vec3 earthPosition = mainController->get_earths_position();
    glm::vec3 moonPosition = mainController->get_moons_position();

    graphics->begin_gui();

    ImGui::Begin("Camera info");

    ImGui::Text("Camera position: (%f, %f, %f)", camera->Position.x, camera->Position.y, camera->Position.z);

    ImGui::Text("Earth position: (%f, %f, %f)", earthPosition.x, earthPosition.y, earthPosition.z);
    ImGui::Text("Moon position: (%f, %f, %f)", moonPosition.x, moonPosition.y, moonPosition.z);

    ImGui::Separator();

    ImGui::Text("Earth Rotation (around own axis) - {KEY1}: %s", mainController->is_earth_rotation_enabled() ? "ON" : "OFF");
    ImGui::Text("Earth Orbit (around Sun) - {KEY2}: %s", mainController->is_earth_orbit_enabled() ? "ON" : "OFF");
    ImGui::Text("Sun Rotation - {KEY3}: %s", mainController->is_sun_rotation_enabled() ? "ON" : "OFF");
    ImGui::Text("Moon Rotation (around own axis) - {KEY4}: %s", mainController->is_moon_rotation_enabled() ? "ON" : "OFF");
    ImGui::Text("Moon Rotation (around Earth) - {KEY5}: %s", mainController->is_moon_orbit_enabled() ? "ON" : "OFF");

    ImGui::End();

    graphics->end_gui();

}
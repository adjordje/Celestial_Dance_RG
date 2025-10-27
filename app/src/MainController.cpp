#include "MainController.hpp"

#include "GUIController.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"

class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    if (!gui_controller->is_enabled()) {
        camera->rotate_camera(position.dx, position.dy);
    }
}

void MainController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());

    m_sunPosition = glm::vec3(0.0f, -5.0f, -25.0f);
    m_earthStartPosition = glm::vec3(0.0f, -5.0f, -5.0f);

    glm::vec3 dirES = m_earthStartPosition - m_sunPosition;
    m_earthOrbitRadius = glm::length(dirES);

    m_earthRotationAngle = 0.0f;
    m_sunRotationAngle = 0.0f;
    m_earthOrbitAngle = atan2(dirES.z, dirES.x); //daje ugao u xy-ravni

    engine::graphics::OpenGL::enable_depth_testing();
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}

void MainController::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    float dt = platform->dt();
    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, 4 * dt);
    }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, 4 * dt);
    }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, 4 * dt);
    }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, 4 * dt);
    }
}

void MainController::update() {
    update_camera();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    float dt = platform->dt();

    // Rotacija Zemlje oko svoje ose
    m_earthRotationAngle += dt * glm::radians(15.0f);
    if (m_earthRotationAngle > glm::two_pi<float>())
        m_earthRotationAngle -= glm::two_pi<float>();

    // Rotacija Zemlje oko Sunca
    m_earthOrbitAngle += dt * glm::radians(5.0f);
    if (m_earthOrbitAngle > glm::two_pi<float>())
        m_earthOrbitAngle -= glm::two_pi<float>();

    m_earthPosition.x = m_sunPosition.x + m_earthOrbitRadius * cos(m_earthOrbitAngle);
    m_earthPosition.y = m_sunPosition.y;
    m_earthPosition.z = m_sunPosition.z + m_earthOrbitRadius * sin(m_earthOrbitAngle);

    // Rotacija Sunca oko svoje ose
    m_sunRotationAngle += dt * glm::radians(0.5f);
    if (m_sunRotationAngle > glm::two_pi<float>())
        m_sunRotationAngle -= glm::two_pi<float>();
}

void MainController::draw_earth() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model* earth = resources->model("earth");
    engine::resources::Shader* shader = resources->shader("earth_shader");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_earthPosition);
    model = glm::rotate(model, 3.14f, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, m_earthRotationAngle, glm::vec3(0.0f, 0.765f, 0.235f)); //rotacija oko svoje ose

    shader->set_mat4("model", model);

    earth->draw(shader);
}

void MainController::draw_sun() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *sun = resources->model("sun");

    engine::resources::Shader *shader = resources->shader("sun_shader");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -5.0f, -25.0f));
    model = glm::scale(model, glm::vec3(0.01f));
    model = glm::rotate(model, m_sunRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f)); //rotacija oko svoje ose

    shader->set_mat4("model", model);

    sun->draw(shader);
}

void MainController::draw_skybox() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resources->skybox("space_skybox");
    auto shader = resources->shader("skybox");
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->draw_skybox(shader, skybox);
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw() {
    draw_sun();
    draw_earth();
    draw_skybox();
}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}
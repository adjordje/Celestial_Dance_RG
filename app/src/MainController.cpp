#include "MainController.hpp"

#include "GUIController.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "../../engine/libs/glad/include/glad/glad.h"
#include "GLFW/glfw3.h"
#include <engine/platform/Window.hpp>

#include <iostream>

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
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();

    m_sun_position = glm::vec3(0.0f, -5.0f, -25.0f);
    m_earth_start_position = glm::vec3(0.0f, -5.0f, -5.0f);
    m_moon_start_position = glm::vec3(0.0f, -5.0f, -7.5f);

    m_earth_rotation_angle = 0.0f;
    m_sun_rotation_angle = 0.0f;
    m_moon_rotation_angle = 0.0f;

    glm::vec3 dirES = m_earth_start_position - m_sun_position;
    m_earth_orbit_radius = glm::length(dirES);

    m_earth_orbit_angle = atan2(dirES.z, dirES.x); //daje ugao u xy-ravni

    glm::vec3 dirME = m_moon_start_position - m_earth_start_position;
    m_moon_orbit_radius = glm::length(dirME);

    m_moon_orbit_angle = atan2(dirME.z, dirME.x);

    m_earth_orbit_enabled = true;
    m_earth_rotation_enabled = true;
    m_sun_rotation_enabled = true;
    m_moon_rotation_enabled = true;
    m_moon_orbit_enabled = true;

    m_moon_light_intensity = 1.0f;

    // za event
    m_draw_moon = true;
    m_light_color = glm::vec3(1.0f);

    // framebuffer pocetak
    m_use_framebuffer = true;

    m_post_process_shader = resources->shader("postprocess_shader");
    m_post_process_shader->use();
    m_post_process_shader->set_int("screen_texture", 0);

    float quadVertices[] = {
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    int screenWidth = platform->window()->width();
    int screenHeight = platform->window()->height();

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &m_texture_color_buffer);
    glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth,
                        screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_color_buffer, 0);

    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // quad
    glGenVertexArrays(1, &m_quad_vao);
    glGenBuffers(1, &m_quad_vbo);
    glBindVertexArray(m_quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
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

    // Zemlja rotacija
    if (platform->key(engine::platform::KeyId::KEY_1).state() == engine::platform::Key::State::JustPressed) {
        m_earth_rotation_enabled = !m_earth_rotation_enabled;
    }
    // Zemlja rotacija oko Sunca
    if (platform->key(engine::platform::KeyId::KEY_2).state() == engine::platform::Key::State::JustPressed) {
        m_earth_orbit_enabled = !m_earth_orbit_enabled;
    }
    // Sunce rotacija
    if (platform->key(engine::platform::KeyId::KEY_3).state() == engine::platform::Key::State::JustPressed) {
        m_sun_rotation_enabled = !m_sun_rotation_enabled;
    }
    // Mesec rotacija
    if (platform->key(engine::platform::KeyId::KEY_4).state() == engine::platform::Key::State::JustPressed) {
        m_moon_rotation_enabled = !m_moon_rotation_enabled;
    }
    // Mesec rotacija oko Zemlje
    if (platform->key(engine::platform::KeyId::KEY_5).state() == engine::platform::Key::State::JustPressed) {
        m_moon_orbit_enabled = !m_moon_orbit_enabled;
    }

    // event
    if (platform->key(engine::platform::KeyId::KEY_T).state() == engine::platform::Key::State::JustPressed) {
        start_timed_event(3.0, 6.0);
    }
    // reset event
    if (platform->key(engine::platform::KeyId::KEY_R).state() == engine::platform::Key::State::JustPressed) {
        reset_timed_event();
    }

    if (m_timed_event.active) {
        float elapsed = glfwGetTime() - m_timed_event.startTime;

        if (elapsed > m_timed_event.delayA && !m_timed_event.eventATriggered) {
            m_draw_moon = false;
            m_timed_event.eventATriggered = true;
        }

        if (elapsed > m_timed_event.delayB && !m_timed_event.eventBTriggered) {
            m_light_color = glm::vec3(1.0f, 0.5f, 0.5f);
            m_timed_event.eventBTriggered = true;
        }
    }

    // Rotacija Zemlje oko svoje ose
    if (m_earth_rotation_enabled) {
        m_earth_rotation_angle += dt * glm::radians(15.0f);
        if (m_earth_rotation_angle > glm::two_pi<float>())
            m_earth_rotation_angle -= glm::two_pi<float>();
    }

    // Rotacija Zemlje oko Sunca
    if (m_earth_orbit_enabled) {
        m_earth_orbit_angle += dt * glm::radians(5.0f);
        if (m_earth_orbit_angle > glm::two_pi<float>())
            m_earth_orbit_angle -= glm::two_pi<float>();
    }

    m_earth_position.x = m_sun_position.x + m_earth_orbit_radius * cos(m_earth_orbit_angle);
    m_earth_position.y = m_sun_position.y;
    m_earth_position.z = m_sun_position.z + m_earth_orbit_radius * sin(m_earth_orbit_angle);

    // Rotacija Sunca oko svoje ose
    if (m_sun_rotation_enabled) {
        m_sun_rotation_angle += dt * glm::radians(0.5f);
        if (m_sun_rotation_angle > glm::two_pi<float>())
            m_sun_rotation_angle -= glm::two_pi<float>();
    }

    // Rotacija Meseca oko svoje ose
    if (m_moon_rotation_enabled) {
        m_moon_rotation_angle += dt * glm::radians(80.0f);
        if (m_moon_rotation_angle > glm::two_pi<float>())
            m_moon_rotation_angle -= glm::two_pi<float>();
    }

    // Rotacija Meseca oko Zemlje
    if (m_moon_orbit_enabled) {
        m_moon_orbit_angle += dt * glm::radians(50.0f);
        if (m_moon_orbit_angle > glm::two_pi<float>())
            m_moon_orbit_angle -= glm::two_pi<float>();
    }

    m_moon_position.x = m_earth_position.x + m_moon_orbit_radius * cos(m_moon_orbit_angle);
    m_moon_position.y = m_earth_position.y;
    m_moon_position.z = m_earth_position.z + m_moon_orbit_radius * sin(m_moon_orbit_angle);
}

void MainController::start_timed_event(double a, double b) {
    m_timed_event.active = true;
    m_timed_event.delayA = a;
    m_timed_event.delayB = b;
    m_timed_event.startTime = glfwGetTime();
    m_timed_event.eventATriggered = false;
    m_timed_event.eventBTriggered = false;

    m_draw_moon = true;
    m_light_color = glm::vec3(1.0f);
}

void MainController::reset_timed_event() {
    m_timed_event.active = false;
    m_timed_event.delayA = 0.0;
    m_timed_event.delayB = 0.0;
    m_timed_event.startTime = 0.0;
    m_timed_event.eventATriggered = false;
    m_timed_event.eventBTriggered = false;

    m_draw_moon = true;
    m_light_color = glm::vec3(1.0f);
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
    model = glm::translate(model, m_earth_position);
    model = glm::rotate(model, 3.14f, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, m_earth_rotation_angle, glm::vec3(0.0f, 0.765f, 0.235f)); //rotacija oko svoje ose

    shader->set_mat4("model", model);

    glm::vec3 lightDir = glm::normalize(m_earth_position - m_sun_position);

    // za directional light
    shader->set_vec3("lightDir", lightDir);
    shader->set_vec3("viewPos", graphics->camera()->Position);
    shader->set_bool("blinn", true);

    // za point light
    shader->set_vec3("pointLightPos", m_moon_position);
    shader->set_vec3("pointLightColor", glm::vec3(0.1f) * m_moon_light_intensity);

    shader->set_vec3("lightColor", m_light_color);

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
    model = glm::rotate(model, m_sun_rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f)); //rotacija oko svoje ose

    shader->set_mat4("model", model);

    sun->draw(shader);
}

void MainController::draw_moon() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *moon = resources->model("moon");

    engine::resources::Shader *shader = resources->shader("moon_shader");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, m_moon_position);
    model = glm::rotate(model, m_moon_rotation_angle, glm::vec3(0.0f, 0.93f, 0.07f));
    model = glm::scale(model, glm::vec3(0.15f));

    glm::vec3 lightDir = glm::normalize(m_moon_position - m_sun_position);

    shader->set_vec3("lightDir", lightDir);
    shader->set_vec3("viewPos", graphics->camera()->Position);
    shader->set_bool("blinn", true);

    shader->set_vec3("lightColor", m_light_color);

    shader->set_mat4("model", model);

    moon->draw(shader);
}

void MainController::draw_skybox() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resources->skybox("space_skybox");
    auto shader = resources->shader("skybox");
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->draw_skybox(shader, skybox);
}

void MainController::begin_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if(m_use_framebuffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        resize_framebuffer(platform->window()->width(), platform->window()->height());
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    engine::graphics::OpenGL::clear_buffers();
}

void MainController::resize_framebuffer(int width, int height) {
    glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width,
                        height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void MainController::draw() {
    // render u framebuffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    draw_sun();
    draw_earth();
    if (m_draw_moon) {
        draw_moon();
    }
    draw_skybox();

    if(m_use_framebuffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_post_process_shader->use();
        glBindVertexArray(m_quad_vao);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

glm::vec3 MainController::get_earths_position() const {
    return m_earth_position;
}

glm::vec3 MainController::get_moons_position() const {
    return m_moon_position;
}

bool MainController::is_earth_rotation_enabled() const {
    return m_earth_rotation_enabled;
}

bool MainController::is_earth_orbit_enabled() const {
    return m_earth_orbit_enabled;
}

bool MainController::is_sun_rotation_enabled() const {
    return m_sun_rotation_enabled;
}

bool MainController::is_moon_rotation_enabled() const {
    return m_moon_rotation_enabled;
}

bool MainController::is_moon_orbit_enabled() const {
    return m_moon_orbit_enabled;
}

bool MainController::is_moon_visible() const {
    return m_draw_moon;
}

bool MainController::is_event_active() const {
    return m_timed_event.active;
}
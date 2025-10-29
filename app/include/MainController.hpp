#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include "engine/core/Controller.hpp"

#include <engine/resources/Shader.hpp>
#include <glm/vec3.hpp>

struct TimedEvent {
    bool active = false;
    double startTime = 0.0;
    double delayA = 0.0;
    double delayB = 0.0;
    bool eventATriggered = false;
    bool eventBTriggered = false;
};

class MainController : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "MainController";
    }

    float m_moon_light_intensity; // za gui
    bool m_use_framebuffer; // za gui

    glm::vec3 get_earths_position() const;
    glm::vec3 get_moons_position() const;
    bool is_earth_rotation_enabled() const;
    bool is_earth_orbit_enabled() const;
    bool is_sun_rotation_enabled() const;
    bool is_moon_rotation_enabled() const;
    bool is_moon_orbit_enabled() const;
    bool is_moon_visible() const;
    bool is_event_active() const;

private:
    void initialize() override;
    bool loop() override;

    void draw() override;
    void begin_draw() override;
    void end_draw() override;
    void draw_earth();
    void draw_sun();
    void draw_moon();
    void draw_skybox();

    void update_camera();
    void update() override;

    void start_timed_event(double a, double b);
    void reset_timed_event();

    void resize_framebuffer(int width, int height);

    glm::vec3 m_sun_position;
    glm::vec3 m_earth_start_position;
    glm::vec3 m_earth_position;
    glm::vec3 m_moon_start_position;
    glm::vec3 m_moon_position;

    float m_earth_rotation_angle;
    float m_sun_rotation_angle;
    float m_earth_orbit_radius;
    float m_earth_orbit_angle;
    float m_moon_rotation_angle;
    float m_moon_orbit_angle;
    float m_moon_orbit_radius;

    bool m_earth_rotation_enabled; // Zemlja rotira oko svoje ose
    bool m_earth_orbit_enabled; // Zemlja orbitira oko Sunca
    bool m_sun_rotation_enabled; // Sunce rotira oko svoje ose
    bool m_moon_rotation_enabled; // Mesec rotira oko svoje ose
    bool m_moon_orbit_enabled; // Mesec orbitira oko Zemlje

    // za event
    bool m_draw_moon;
    glm::vec3 m_light_color;
    TimedEvent m_timed_event;

    unsigned int m_fbo;
    unsigned int m_texture_color_buffer;
    unsigned int m_rbo;

    unsigned int m_quad_vao = 0;
    unsigned int m_quad_vbo = 0;

    engine::resources::Shader* m_post_process_shader = nullptr;
};



#endif //MAINCONTROLLER_HPP

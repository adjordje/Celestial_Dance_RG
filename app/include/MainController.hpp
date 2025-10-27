#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include "engine/core/Controller.hpp"

#include <glm/vec3.hpp>

class MainController : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "MainController";
    }

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

    glm::vec3 m_sunPosition;
    glm::vec3 m_earthStartPosition;
    glm::vec3 m_earthPosition;
    glm::vec3 m_moonStartPosition;
    glm::vec3 m_moonPosition;

    float m_earthRotationAngle;
    float m_sunRotationAngle;
    float m_earthOrbitRadius;
    float m_earthOrbitAngle;
    float m_moonRotationAngle;
    float m_moonOrbitAngle;
    float m_moonOrbitRadius;

    bool m_earthRotationEnabled; // Zemlja rotira oko svoje ose
    bool m_earthOrbitEnabled; // Zemlja orbitira oko Sunca
    bool m_sunRotationEnabled; // Sunce rotira oko svoje ose

};



#endif //MAINCONTROLLER_HPP

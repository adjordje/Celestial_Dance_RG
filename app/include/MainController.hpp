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
    void draw_skybox();

    void update_camera();

    void update() override;

    glm::vec3 m_sunPosition;
    glm::vec3 m_earthPosition;

};



#endif //MAINCONTROLLER_HPP

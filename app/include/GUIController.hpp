#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP
#include "engine/core/Controller.hpp"

#include <engine/graphics/PostProcess.hpp>

class GUIController : public engine::core::Controller {

public:
    std::string_view name() const override { return "GUIController"; }

private:
    void initialize() override;

    void poll_events() override;

    void draw() override;

    engine::graphics::PostProcess *m_post_process;
};

#endif //GUICONTROLLER_HPP

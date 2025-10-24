#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include "engine/core/Controller.hpp"

class MainController : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "MainController";
    }

private:
    void initialize() override;
    bool loop() override;
};



#endif //MAINCONTROLLER_HPP

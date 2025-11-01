#ifndef POST_PROCESS_HPP
#define POST_PROCESS_HPP
#include <engine/resources/Shader.hpp>

namespace engine::graphics {

class GraphicsController;

class PostProcess {
    friend class GraphicsController;

public:
    void draw();

    void begin_draw();

    void resize(int width, int height);

    void set_enabled(bool enabled);

private:
    PostProcess() = default;

    PostProcess(int width, int height, const resources::Shader *shader);

    unsigned int m_fbo;
    unsigned int m_rbo;
    unsigned int m_texture_color_buffer;
    unsigned int m_quad_vao = 0;
    unsigned int m_quad_vbo = 0;
    const resources::Shader *m_post_process_shader = nullptr;
    int m_width;
    int m_height;

    bool m_enabled;
};

}

#endif //POSTPROCESS_HPP

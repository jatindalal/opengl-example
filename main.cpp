#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

#include <iostream>

// Vertices coordinates
// ccords, colors
GLfloat vertices[] =
{
	-0.5f,  -0.5f * float(sqrt(3)) / 3,     0.0f, 0.8f, 0.3f, 0.02f, // Lower left corner
	 0.5f,  -0.5f * float(sqrt(3)) / 3,     0.0f, 0.8f, 0.3f, 0.02f, // Lower right corner
	 0.0f,   0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 1.0f, 0.6f, 0.32f, // Upper corner
	-0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f, 0.9f, 0.45f, 0.17f, // Inner left
	 0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f, 0.9f, 0.45f, 0.17f, // Inner right
	 0.0f,  -0.5f * float(sqrt(3)) / 3,     0.0f, 0.8f, 0.3f, 0.02f// Inner down
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    if (!glfwInit()) {
        std::cout << "error initializing glfw\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Opengl 3d",
                                          nullptr, nullptr);
    if (!window) {
        std::cout << "failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failed to initialize glad" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader("shaders/default.vert", "shaders/default.frag");
    VertexArray vao;
    vao.bind();

    VertexBuffer vbo(vertices, sizeof(vertices));
    ElementBuffer ebo(indices, sizeof(indices));

    vao.link_attrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao.link_attrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float),
                    (void*)(3 * sizeof(float)));

    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    auto uniform_id = glGetUniformLocation(shader.m_id, "scale");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.activate();
        glUniform1f(uniform_id, 0.5f);
        vao.bind();

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.delete_array();
    vbo.delete_buffer();
    ebo.delete_buffer();
    shader.delete_shader();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

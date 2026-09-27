#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "stb_image.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int width = 600;
const unsigned int height = 600;

GLfloat vertices[] {
    -0.5, -0.5, 0.0, 1.0, 1.0, 1.0,
    0.0, 0.5, 0.0, 1.0, 1.0, 1.0,
    0.5, -0.5, 0.0, 1.0, 1.0, 1.0
};

unsigned int indices[] {
    0, 1, 2
};

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Membuat Konteks Versi OpenGL yang di Gunakan
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // membuat Sebuah Jendela dan di simpan ke window
    GLFWwindow* window = glfwCreateWindow(width, height, "Graphic Engine - Test", nullptr, nullptr);
    
    // Jika window tidak berhasil di muat
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // Mengirim perintah openg ke window
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    
    // Source Shader
    Shader shader("reasource/shader/shader.vert", "reasource/shader/shader.frag");

    // Membuat Object Vertex 
    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0);
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
    
    while (!glfwWindowShouldClose(window)) {
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int width = 600;
const unsigned int height = 600;

GLfloat vertices[] {
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // kiri bawah
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // kanan bawah
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // kiri atas
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f // kanan atas
};

unsigned int indices[] {
    0, 1, 2,
    1, 2, 3 
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
    
    //membuat Texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Mengatur cara kerja texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Mengambil sumber texture dan di ikat ke GL_TEXTURE_2D
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("reasource/texture/pepe.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture \n";
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Membuat Shader
    Shader shader("reasource/shader/shader.vert", "reasource/shader/shader.frag");

    // Membuat Object Vertex 
    VAO VAO1;
    VAO1.Bind();

    // Membuat dan mengikat VBO dan EBO
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    // Memberi tau GPU gimana cara baca data vertices
    VAO1.LinkVBO(VBO1, 0, 3, 8 * sizeof(GLfloat), nullptr);
    VAO1.LinkVBO(VBO1, 1, 3, 8 * sizeof(GLfloat), reinterpret_cast<const void*>(3 * sizeof(GLfloat)));
    VAO1.LinkVBO(VBO1, 2, 2, 8 * sizeof(GLfloat), reinterpret_cast<const void*>(6 * sizeof(GLfloat)));
    
    // Unbind setelah di gunakan untuk menghindari memory leak
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
    
    // Looping
    while (!glfwWindowShouldClose(window)) {
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //float timeValue = glfwGetTime();
        //float colorValue = (sin(timeValue) / 2.0) + 0.5;
        //int vertexColorLocation = glGetUniformLocation(shader.ID, "testColor");
        //glUniform4f(vertexColorLocation, 0.0, colorValue, 0.0, 1.0);
        
        // Mengaktifkan Texture di Slot 0 dan Memanggil Texture yang di siapkan
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        // Menggunakan Shader
        shader.use();
        
        //glUniform1i(glGetUniformLocation(shader.ID, "textureSampler"), 0);
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &texture);
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
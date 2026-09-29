#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

/*
    Program ini membuat quad 2D sederhana dengan:
    1. GLFW untuk jendela dan input
    2. GLEW untuk menghubungkan OpenGL
    3. VAO, VBO, EBO untuk mengirim data geometri ke GPU
    4. Shader untuk memproses vertex dan fragment
    5. Texture untuk mengisi warna/pola pada quad

    Inti alur program: buat window -> inisialisasi OpenGL -> buat data vertex ->
    buat buffer GPU -> render loop -> swap buffer -> bersihkan resource.
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int width = 600;
const unsigned int height = 600;

// Data geometri quad: 4 vertex, masing-masing punya 8 float:
// x, y, z, r, g, b, u, v
// x/y/z = posisi 3D, r/g/b = warna, u/v = koordinat texture
GLfloat vertices[] {
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // kiri bawah
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // kanan bawah
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // kiri atas
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f // kanan atas
};

unsigned int indices[] {
    // Dua segitiga untuk membentuk quad:
    // 0 - 1 - 2  lalu 1 - 2 - 3
    0, 1, 2,
    1, 2, 3 
};

int main() {
    // 1. Inisialisasi library jendela dan context OpenGL.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set versi OpenGL yang kita pakai: 3.3 core profile.
    // Ini membantu agar API yang dipakai konsisten dan tidak memakai fungsi deprecated.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. Buat jendela render.
    GLFWwindow* window = glfwCreateWindow(width, height, "Graphic Engine - Test", nullptr, nullptr);
    
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // Setelah jendela dibuat, jadikan context ini sebagai context aktif OpenGL.
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // 3. Inisialisasi GLEW agar fungsi OpenGL modern dapat dipanggil.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    
    // 4. Siapkan texture 2D yang akan dipakai untuk quad.
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Atur bagaimana texture dibaca saat koordinat di luar range [0,1].
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Baca gambar dari file ke memory lalu kirim ke GPU.
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

    // 5. Muat dan kompilasi shader vertex + fragment.
    Shader shader("reasource/shader/shader.vert", "reasource/shader/shader.frag");

    // 6. Buat object VAO (Vertex Array Object) untuk mengelola layout vertex.
    VAO VAO1;
    VAO1.Bind();

    // 7. Buat buffer GPU untuk vertex dan indeks.
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    // 8. Jelaskan ke GPU cara membaca data di VBO per atribut vertex.
    // Atribut 0 = posisi (vec3) -> x,y,z
    // Atribut 1 = warna (vec3) -> r,g,b
    // Atribut 2 = UV texture (vec2) -> u,v
    VAO1.LinkVBO(VBO1, 0, 3, 8 * sizeof(GLfloat), nullptr);
    VAO1.LinkVBO(VBO1, 1, 3, 8 * sizeof(GLfloat), reinterpret_cast<const void*>(3 * sizeof(GLfloat)));
    VAO1.LinkVBO(VBO1, 2, 2, 8 * sizeof(GLfloat), reinterpret_cast<const void*>(6 * sizeof(GLfloat)));
    
    // Unbind agar tidak lupa binding state lain saat render.
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
    
    // 9. Render loop: terus cek apakah window ditutup, lalu gambar ulang.
    while (!glfwWindowShouldClose(window)) {
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //float timeValue = glfwGetTime();
        //std::cout << timeValue << std::endl;
        //float colorValue = (sin(timeValue) / 2.0f) + 0.5;
        //std::cout << colorValue << std::endl;
        //int vertexColorLocation = glGetUniformLocation(shader.ID, "testColor");
        //glUniform4f(vertexColorLocation, 0.0, colorValue, 0.0, 1.0);
        
        // Aktifkan texture di slot 0 lalu bind texture yang sudah kita buat.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        // Pilih shader yang akan dipakai untuk render frame ini.
        shader.use();
        
        // Bind VAO agar atribut vertex yang sudah kita atur aktif kembali.
        VAO1.Bind();

        // Gambar 2 segitiga (6 indeks total) yang membentuk quad.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Tampilkan hasil render ke window dan cek event input.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 10. Cleanup resource saat window ditutup.
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
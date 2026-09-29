#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

/*
    Kelas Shader berfungsi untuk:
    - membaca file .vert dan .frag
    - compile shader ke GPU
    - menghubungkan keduanya dalam satu program OpenGL
    - memudahkan pemanggilan shader saat render
*/
class Shader{
    public:
        // ID program shader yang dibuat oleh OpenGL.
        unsigned int ID;

        // Constructor: baca path file shader, compile, lalu link.
        Shader(const char* vertexPath, const char* fragmentPath);

        // Aktifkan program shader saat render.
        void use();

        // Helper untuk meng-set uniform secara cepat.
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
};

#endif
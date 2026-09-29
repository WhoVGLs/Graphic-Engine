#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <GL/glew.h>

/*
    VBO = Vertex Buffer Object.
    Fungsinya adalah menyimpan data vertex di memori video (GPU).
    Data ini biasanya berisi posisi, warna, UV, normal, dst.
*/
class VBO {
    public:
        // ID buffer di OpenGL.
        GLuint ID;

        // Konstruktor: buat buffer lalu isi dengan data vertex.
        VBO(GLfloat* vertices, GLsizeiptr size);

        // Bind / unbind / delete buffer.
        void Bind();
        void Unbind();
        void Delete();
};

#endif
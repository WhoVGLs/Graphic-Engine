#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <GL/glew.h>

/*
    EBO = Element Buffer Object.
    Fungsinya adalah menyimpan indeks vertex yang membentuk polygon/triangle.
    Contoh: [0, 1, 2, 1, 2, 3] untuk membuat 2 segitiga.
*/
class EBO
{
public:
	// ID buffer indeks di OpenGL.
	GLuint ID;

	// Konstruktor: buat buffer EBO lalu isi dengan data indeks.
	EBO(GLuint* indices, GLsizeiptr size);

	// Bind / unbind / delete buffer.
	void Bind();
	void Unbind();
	void Delete();
};

#endif
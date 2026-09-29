#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <GL/glew.h>
#include"VBO.h"

/*
    VAO = Vertex Array Object.
    Fungsinya adalah menyimpan konfigurasi bagaimana vertex data dibaca oleh GPU.
    Jadi semuanya yang berhubungan dengan format vertex disimpan di sini.
*/
class VAO
{
public:
	// ID object VAO yang dibuat OpenGL.
	GLuint ID;

	// Constructor: buat VAO baru.
	VAO();

	// Hubungkan VBO ke VAO dengan format atribut tertentu.
	void LinkVBO(VBO& VBO, GLuint layout, GLuint size, GLsizei stride, const void* offset);

	// Bind / unbind / delete VAO.
	void Bind();
	void Unbind();
	void Delete();
};
#endif
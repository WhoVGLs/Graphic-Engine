#include"VAO.h"

/*
    VAO::VAO
    - glGenVertexArrays: bikin object VAO baru
    - VAO ini nantinya bertindak seperti 'template' untuk atribut vertex
*/
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

/*
    LinkVBO ini menjelaskan ke OpenGL:
    - atribut ke berapa yang kita pakai (layout)
    - jumlah komponen per atribut (size)
    - tipe data (GL_FLOAT)
    - stride: jarak antar data vertex
    - offset: mulai dari indeks ke berapa dalam struktur data

    Misalnya untuk vertex {x, y, z, r, g, b, u, v},
    atribut 0 bisa baca x,y,z, atribut 1 baca r,g,b, atribut 2 baca u,v.
*/
void VAO::LinkVBO(VBO& VBO, GLuint layout, GLuint size, GLsizei stride, const void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
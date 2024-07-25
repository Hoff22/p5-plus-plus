#pragma once
#include "MainWindow.h"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std;

struct DrawObject {
	GLuint VAO;
	GLuint VBO_positions;
	GLuint VBO_normals;
	GLuint VBO_tangents;
	GLuint VBO_uv;
	GLuint VBO_index;
	GLuint VBO_instancedM0;
	GLuint VBO_instancedM1;
	GLuint VBO_instancedM2;
	GLuint VBO_instancedM3;
	GLuint indexes_size;

	DrawObject() :
		VAO(0),
		VBO_positions(0),
		VBO_index(0),
		VBO_normals(0),
		VBO_tangents(0),
		VBO_uv(0),
		VBO_instancedM0(0),
		VBO_instancedM1(0),
		VBO_instancedM2(0),
		VBO_instancedM3(0),
		indexes_size(0)
	{}

	bool operator < (const DrawObject& b) const {
		return (indexes_size < b.indexes_size);
	}
};

class Renderer
{
public:

	static GLsizei FRAME_WIDTH;
	static GLsizei FRAME_HEIGHT;
	static Shader screen_shader;
	static DrawObject sprite_primitive;
	static DrawObject line_primitive;
	static GLuint shapes[400];
	static GLuint shapes_end;

	// returns a VAO for the specified vertex array and indice array;
	static void   buildTrianglesVAO(const vector<float>& model_coefficients, const vector<float>& nromal_coefficients, const vector<float>& uv_coefficients, const vector<GLuint>& indices, DrawObject* obj);
	static GLuint buildTrianglesVAO(const vector<float>& vertex_position, const vector<GLuint>& face_indexes);

	static void rect(unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y);

	static void initFrame(glm::vec4 bg_color, bool clear_color = false);

	static void drawFrame(glm::vec4 bg_color);
	
	static void setupPrimitives();

	static int setup_GLAD_GLFW_OpenGL_Shard(string program_window_name);
};


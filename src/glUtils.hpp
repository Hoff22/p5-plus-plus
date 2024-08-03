#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glu{

	const int N = 1000000;

	struct shaderData{
		GLint cell_count;
		GLubyte state[N * 4];
	};
	
	GLuint buildShaderDataBuffer(GLuint binding = 0){
		GLuint DBO;
		glGenBuffers(1, &DBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, DBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, DBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		return DBO;
	}

	GLuint buildTrianglesVAO(const std::vector<float>& model_coefficients, const std::vector<GLuint>& indices){
		GLuint VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		GLuint VBO_model_coefficients_id;
		glGenBuffers(1, &VBO_model_coefficients_id);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
		glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
		GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
		GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
		glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(location);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint indices_id;
		glGenBuffers(1, &indices_id);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());

		glBindVertexArray(0);

		return VAO;
	}

}

#include "Renderer.h"

GLsizei Renderer::FRAME_WIDTH;
GLsizei Renderer::FRAME_HEIGHT;

DrawObject Renderer::line_primitive;
DrawObject Renderer::sprite_primitive;

shaderData Renderer::sd;

GLuint Renderer::dataBuffer = 0;

Shader Renderer::screen_shader;

void Renderer::initFrame(glm::vec4 bg_color, bool clear_color) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (clear_color) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(bg_color[0], bg_color[1], bg_color[2], 1.0);
	}
	else {
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
}

void Renderer::drawFrame(glm::vec4 bg_color) {
	
	Renderer::FRAME_WIDTH = MainWindow::SCR_WIDTH;
	Renderer::FRAME_HEIGHT = MainWindow::SCR_HEIGHT;

	initFrame(bg_color, true);

	Renderer::screen_shader.use();
	Renderer::screen_shader.setFloat2("screen_size", glm::vec2(Renderer::FRAME_WIDTH, Renderer::FRAME_HEIGHT));

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Renderer::dataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(shaderData), &Renderer::sd, GL_DYNAMIC_DRAW);

	// glDrawTriangles
	// draw triangle to screen:
	glBindVertexArray(Renderer::sprite_primitive.VAO);

	glDrawElements(GL_TRIANGLES, Renderer::sprite_primitive.indexes_size, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glViewport(0, 0, FRAME_WIDTH, FRAME_HEIGHT);

	assert(MainWindow::window != NULL);

	MainWindow::drawUI();

	glfwSwapBuffers(MainWindow::window);
}

void Renderer::rect(int x, int y, int size_x, int size_y, glm::vec4 color){
	sd.shapes[sd.shapes_end] = glm::ivec4(x,y,size_x,size_y);
	sd.shapes_color[sd.shapes_end] = color;
	sd.shapes_end++;
}

void Renderer::setupPrimitives() {
	vector<float> line_model_coefficients = {
		0.0,0.0,0.0,1.0,
		0.0,0.0,-1.0,1.0
	};
	vector<GLuint> line_indices= {
		0,1
	};

	line_primitive.indexes_size = (int)line_indices.size();
	line_primitive.VAO = buildTrianglesVAO(
		line_model_coefficients,
		line_indices
	);

	vector<float> sprite_model_coefficients = {
		-1.0, -1.0, 0.0, 1.0,
		 1.0, -1.0, 0.0, 1.0,
		-1.0,  1.0, 0.0, 1.0,
		 1.0,  1.0, 0.0, 1.0
	};
	vector<float> sprite_normal_coefficients = {
		 0.0, 0.0, 1.0, 0.0,
		 0.0, 0.0, 1.0, 0.0,
		 0.0, 0.0, 1.0, 0.0,
		 0.0, 0.0, 1.0, 0.0
	};
	vector<float> sprite_texture_coefficients = {
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 1.0
	};
	vector<GLuint> sprite_indices = {
		0,1,2,
		2,1,3
	};

	sprite_primitive.indexes_size = (int)sprite_indices.size();
	buildTrianglesVAO(
		sprite_model_coefficients,
		sprite_normal_coefficients,
		sprite_texture_coefficients,
		sprite_indices,
		&sprite_primitive
	);

	screen_shader = Shader("shaders/vert.glsl", "shaders/frag.glsl");
}

void Renderer::initShaderDataBuffer() {
	GLuint binding = 0;
	glGenBuffers(1, &Renderer::dataBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Renderer::dataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, Renderer::dataBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

int Renderer::setup_GLAD_GLFW_OpenGL_Shard(string program_window_name) {
	glfwSetErrorCallback(MainWindow::glfw_error_callback);
	if (!glfwInit()) return 1;

	glfwWindowHint(GLFW_SAMPLES, 4);

	MainWindow::window = glfwCreateWindow(MainWindow::SCR_WIDTH, MainWindow::SCR_HEIGHT, program_window_name.c_str(), NULL, NULL);
	if (MainWindow::window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(MainWindow::window);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(MainWindow::window, MainWindow::framebuffer_size_callback);
	glfwSetScrollCallback(MainWindow::window, MainWindow::scroll_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// init ui
	MainWindow::initUI();
	setupPrimitives();

	return 0;
}

void Renderer::buildTrianglesVAO(const vector<float>& model_coefficients, const vector<float>& nromal_coefficients, const vector<float>& uv_coefficients, const vector<GLuint>& indices, DrawObject* obj)
{
#if 1
	if (obj->VAO == 0) {
		glGenVertexArrays(1, &obj->VAO);
	}
	glBindVertexArray(obj->VAO);

	if (obj->VBO_positions == 0) {
		glGenBuffers(1, &obj->VBO_positions);
	}
	if (obj->VBO_normals == 0) {
		glGenBuffers(1, &obj->VBO_normals);
	}
	if (obj->VBO_uv == 0) {
		glGenBuffers(1, &obj->VBO_uv);
	}
	if (obj->VBO_index == 0) {
		glGenBuffers(1, &obj->VBO_index);
	}

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
	GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
	GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, nromal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nromal_coefficients.size() * sizeof(float), nromal_coefficients.data());
	location = 1; // "(location = 0)" em "shader_vertex.glsl"
	number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj->VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, uv_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, uv_coefficients.size() * sizeof(float), uv_coefficients.data());
	location = 3; // "(location = 0)" em "shader_vertex.glsl"
	number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint indices_id = obj->VBO_index;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());

	glBindVertexArray(0);
#endif
}

GLuint Renderer::buildTrianglesVAO(const vector<float>& model_coefficients, const vector<GLuint>& indices)
{
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
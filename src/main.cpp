#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <set>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
         // std::mutex


#include "glUtils.hpp"
#include "flood_fill.hpp"
#include "MainWindow.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

int done = 0;

void process();

int setup_GLAD_GLFW_OpenGL_Shard(string program_window_name);

// void step(queue<tuple<int,int,int,int>>& q, glu::shaderData& sd, map<unsigned char, pair<int,int>>& seeds);

int main() {

	using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

	setup_GLAD_GLFW_OpenGL_Shard("p5++");

	MainWindow::initUI();

	// CW vertex order
	vector<float> mc = {
	     1.0f,  1.0f, 0.0f, 1.0f,  // UR
	     1.0f, -1.0f, 0.0f, 1.0f,  // DR
	    -1.0f, -1.0f, 0.0f, 1.0f,  // DL
	    -1.0f,  1.0f, 0.0f, 1.0f   // UL 
	};
	vector<GLuint> id = {
	    0, 1, 3,   // first triangle
	    1, 2, 3    // second triangle
	};

	GLuint VAO = glu::buildTrianglesVAO(mc, id);
	GLuint DBO = glu::buildShaderDataBuffer();

	Shader sh = Shader("shaders/vert.glsl", "shaders/frag.glsl");

	flf::init(MainWindow::input_values[0], MainWindow::input_values[1]);

    thread processThread = thread(process);

	while (MainWindow::is_open()) {
		MainWindow::handle_input();

		if(MainWindow::input_values[4]){
			MainWindow::input_values[4] = 0;
			sh = Shader("shaders/vert.glsl", "shaders/frag.glsl");
		}
		if(MainWindow::input_values[3]){
			done = 0;
			MainWindow::input_values[3] = 0;
			flf::init(MainWindow::input_values[0],MainWindow::input_values[1]);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(0.2, 0.0, 0.4, 1.0);
	
		glEnable(GL_MULTISAMPLE);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);

		glPolygonMode(GL_BACK, GL_LINE);

		sh.use();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, DBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glu::shaderData), &flf::sd, GL_DYNAMIC_DRAW);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, id.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(VAO);

		MainWindow::drawUI();
		glViewport(0,0,MainWindow::SCR_WIDTH, MainWindow::SCR_HEIGHT);
		glfwSwapBuffers(MainWindow::window);
	}
	// clean up

    processThread.join();

	MainWindow::cleanupUI();

	return 0;
}

void process(){
	while (MainWindow::is_open()){
		flf::step();
	}
}

int setup_GLAD_GLFW_OpenGL_Shard(string program_window_name) {
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

	return 0;
}
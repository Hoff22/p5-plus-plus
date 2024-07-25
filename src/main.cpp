#include <iostream>
#include <string>
#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

float	p_f[14] = {
	25.0f / 255.0f,
	25.0f / 255.0f,
	25.0f / 255.0f,
	255.0f / 255.0f,
	0.5f,
	45.0,
	27.63,
	500.0,
	12.0,
	20.0,
	0.0f,
	0.015f,
	0.94f,
	8.96f
};
int		p_i[8] = { 10, 0, 0, 0, 0, 0, 0, 0 };
bool	p_b[8] = { true, true, false, false, false, false, false, false };
bool	i_p[6] = { false, false ,false ,false ,false ,false };
bool	click[3] = { false, false ,false };
bool	drag[3] = { false, false ,false };
bool	cursor;
double	curTime;

void initScene() {
	Scene::paramsf = p_f;
	Scene::paramsi = p_i;
	Scene::paramsb = p_b;
	Scene::is_pressed = i_p;
	Scene::click = click;
	Scene::mouse_drag = drag;
	Scene::Zoom = 10.0f;
	Scene::deltaTime = 0.0;
	Scene::lastMouseX = 0.0f;
	Scene::lastMouseY = 0.0f;
	Scene::mouseDeltaX = 0.0f;
	Scene::mouseDeltaY = 0.0f;
}

int main() {

	Renderer::setup_GLAD_GLFW_OpenGL_Shard("Shard Sample 2");
	initScene();

	curTime = glfwGetTime();

	while (MainWindow::is_open()) {
		Scene::deltaTime = glfwGetTime() - curTime;
		curTime = glfwGetTime();

		MainWindow::handle_input();

		glm::vec4 bg_color = glm::vec4(p_f[0], p_f[1], p_f[2], p_f[3]);

		Renderer::drawFrame(bg_color);
	}
	// clean up
	MainWindow::cleanupUI();

	return 0;
}
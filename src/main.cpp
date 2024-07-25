#include <iostream>
#include <string>
#include "Renderer.h"
#include "Ocean.h"

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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void initScene() {
	Scene::paramsf = p_f;
	Scene::paramsi = p_i;
	Scene::paramsb = p_b;
	Scene::is_pressed = i_p;
	Scene::click = click;
	Scene::mouse_drag = drag;
	Scene::main_camera = new LookAtCamera(glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0), 90, -15);
	Scene::Zoom = 10.0f;
	Scene::deltaTime = 0.0;
	Scene::lastMouseX = 0.0f;
	Scene::lastMouseY = 0.0f;
	Scene::mouseDeltaX = 0.0f;
	Scene::mouseDeltaY = 0.0f;
}

void updateCamera() {
	Scene::main_camera->ScreenRatio = 1.0f * MainWindow::SCR_WIDTH / MainWindow::SCR_HEIGHT;
	((LookAtCamera*)(Scene::main_camera))->target = glm::vec3(0.0, 0.0, 0.0);
	Scene::main_camera->Zoom = p_f[5];
	((LookAtCamera*)(Scene::main_camera))->distanceFromTarget = Scene::Zoom;
	if (cursor) {
		((LookAtCamera*)(Scene::main_camera))->ProcessMouseMovement(Scene::mouseDeltaX, Scene::mouseDeltaY);
	}
	((LookAtCamera*)(Scene::main_camera))->updateCameraVectors();
}

int main() {

	Renderer::Setup_GLAD_GLFW_OpenGL_Shard("Shard Sample 2");
	initScene();

	curTime = glfwGetTime();

	Ocean ocean_obj;

	while (MainWindow::is_open()) {
		Scene::deltaTime = glfwGetTime() - curTime;
		curTime = glfwGetTime();

		MainWindow::handle_input();

		if (p_b[1]) {
			p_b[1] = 0;
			Renderer::noShadingShader = Shader("shaders/default_vertex.glsl", "shaders/noShading_fragment.glsl");
			Renderer::PBRShader = Shader("shaders/pbr_vertex.glsl", "shaders/pbr_fragment.glsl");
			Renderer::gizmoShader = Shader("shaders/default_vertex.glsl", "shaders/gizmo_fragment.glsl");
		}
		if (Scene::is_pressed[4]) {
			cursor = true;
			glfwSetInputMode(MainWindow::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		if (Scene::is_pressed[5]) {
			cursor = false;
			glfwSetInputMode(MainWindow::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		ocean_obj.update();
		ocean_obj.draw();

		glm::vec4 bg_color = glm::vec4(p_f[0], p_f[1], p_f[2], p_f[3]);

		updateCamera();
		Renderer::DrawFrame(bg_color, Scene::main_camera, RenderFlag::DrawEnvironmentFlag);
	}
	// clean up
	Renderer::UnloadTextures();
	MainWindow::cleanupUI();

	return 0;
}
#include "MainWindow.h"

int MainWindow::SCR_WIDTH = 1000;
int MainWindow::SCR_HEIGHT = 1000;
bool MainWindow::is_open_bool;
GLFWwindow* MainWindow::window;
bool options_hovered;

int MainWindow::initUI() {
	// FAZER ISSO NA MAIN
	
	/*
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) return 1;

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Program", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}*/
	
	is_open_bool = 1;

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;    // Enable mouse Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
	io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 20);
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	//	ImGuiDockNodeFlags_PassthruCentralNode
	return 0;
}

void MainWindow::drawUI() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// start UI frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// setup dockspace of main glfw window
	// this makes glfw window dockable but still see-thru. remove flag if opaque background is needed
	// also must come befor any window declaration
	ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);
	
	// drawOptions MUST come before drawViewport
	drawOptions();
	
	// Renders the ImGUI elements
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void MainWindow::cleanupUI() {
	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

int MainWindow::is_open() {
	return (!glfwWindowShouldClose(window) && is_open_bool);
}

void MainWindow::handle_input() {
	glfwPollEvents();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
}

void MainWindow::drawOptions() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	static std::vector<std::pair<int,int>> points;
	static double curTime = glfwGetTime();
	static int count = 0;
	static double sum_dt = 0;
	static double avg_dt = 0;
	// MainWindow::ShowExampleAppCustomRendering();

	ImGui::SetNextWindowSize(ImVec2(400,400));
	int windowFlags = ImGuiWindowFlags_NoResize;
	// int windowFalgs = 0;
	ImGui::Begin("Options", 0, windowFlags);
	{
		double deltaTime = glfwGetTime() - curTime;
		curTime = glfwGetTime();

		count++;
		sum_dt += deltaTime;
		if(count == 100){
			avg_dt = sum_dt / count;
			count = 0;
			sum_dt = 0;
		}
		
	
		ImGui::Text("FPS: %d", (int)(1.0 / avg_dt));

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available
        if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
        if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
        draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(50, 50, 50, 255), IM_COL32(50, 50, 60, 255), IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));
        draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(255, 255, 255, 255));

        ImGui::InvisibleButton("canvas", canvas_size);
        ImVec2 mpc = ImVec2(io.MousePos.x - canvas_pos.x, io.MousePos.y - canvas_pos.y);
        if (ImGui::IsItemHovered())
        {
            if (ImGui::IsMouseClicked(0) && points.size() < 4)
            {
                points.push_back({mpc.x, mpc.y});
                std::cout << "points: " << std::endl;
                for(auto [x,y] : points){
	        		std::cout << x << " " << y << std::endl;
	        	}
	        	std::cout << std::endl;
            }
            if (ImGui::IsMouseClicked(1) && !points.empty())
            {
                points.pop_back();
            }
        }

        for(auto [x, y] : points){
        	ImVec2 p1(canvas_pos.x + x, canvas_pos.y + y);
        	draw_list->AddCircleFilled(p1, 7.0f, IM_COL32(255, 255, 0, 255), 32); // Circle
        }	

        for (int i = 0; i < (int)points.size() - 1; i++){
        	ImVec2 p1(canvas_pos.x + points[i].first, canvas_pos.y + points[i].second);
        	ImVec2 p2(canvas_pos.x + points[i + 1].first, canvas_pos.y + points[i + 1].second);
            draw_list->AddLine(p1, p2, IM_COL32(255, 255, 0, 255), 2.0f);
        }

        if(points.size() == 4){
        	ImVec2 p1(canvas_pos.x + points[0].first, canvas_pos.y + points[0].second);
        	ImVec2 p2(canvas_pos.x + points[3].first, canvas_pos.y + points[3].second);
            draw_list->AddLine(p1, p2, IM_COL32(255, 255, 0, 255), 2.0f);
        }
	}
	ImGui::End();
}

void MainWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{   // make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.

	MainWindow::SCR_HEIGHT = height;
	MainWindow::SCR_WIDTH = width;

	return;
}
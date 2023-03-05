#include "Application.h"

#include <chrono>
#include "../Application/Source/OpenGL/MeshBuilder.h"
#include "AppOptions.h"

Application::Application()
{
	//window variables
	window = nullptr; //the window
	width = AppOptions::width; //window width
	height = AppOptions::height; //window height
	cam = new Camera(glm::vec3(2.0f, 0.0f, 0.0f), 60.0f, 0.01f, 100.0f, width / height);
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool Application::Init()
{
	// Set error callback
	glfwSetErrorCallback(error_callback);

	// Init glfw
	if (!glfwInit())
	{
		// Initialization failed
		return false;
	}

	// Enable anti-aliasing
	glfwWindowHint(GLFW_SAMPLES, 16);

	// Create the window
	window = glfwCreateWindow(width, height, "MyOpenGLProject", NULL, NULL);
	if (!window)
	{
		// Window or OpenGL context creation failed
		return false;
	}
	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

	// Make window current context
	glfwMakeContextCurrent(window);

	// Load opengl with glad
	gladLoadGL(glfwGetProcAddress);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LESS);

	return true;
}

void Application::Run()
{
	using namespace std;
	double deltaTime;
	chrono::steady_clock::time_point lastUpdate = chrono::steady_clock::now();
	chrono::steady_clock::time_point now = chrono::steady_clock::now();

	Sprite sprite("Images/stone_texture.jpg", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// Main game loop
	while (!glfwWindowShouldClose(window))
	{
		now = chrono::steady_clock::now();
		// Get deltaTime by getting the difference of time between the current frame and the previous frame
		deltaTime = chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0; // divide by 1000000.0 to make deltaTime in seconds

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Quit", "Alt+F4")) 
					{
						glfwSetWindowShouldClose(window, true);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			static bool showDemo = false;
			if (ImGui::IsKeyPressed(ImGuiKey_F1, false))
			{
				if (!showDemo)
					showDemo = true;
				else
					showDemo = false;
			}
			if(showDemo)
				ImGui::ShowDemoWindow();
		}

		// Clear the screen and buffer
		glClearColor(0.2f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		sprite.Render(cam, deltaTime);

		// Put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);

		// Update other events like input handling 
		glfwPollEvents();

		// Save the previous chrono::steady_clock::now() value
		lastUpdate = now; 
	}

	return;
}


Application::~Application()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	delete cam;
	cam = nullptr;

	//destroy the window
	glfwDestroyWindow(window);
	//terminate glfw
	glfwTerminate();	
}
#include "Application.h"

#include <chrono>
#include "OpenGL/MeshBuilder.h"
#include "AppOptions.h"

Application::Application()
{
	//window variables
	window = nullptr; //the window
	width = AppOptions::width; //window width
	height = AppOptions::height; //window height
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool Application::Init()
{
	//set error callback
	glfwSetErrorCallback(error_callback);

	//init glfw
	if (!glfwInit())
	{
		//Initialization failed
		return false;
	}


	//create the window
	window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
	if (!window)
	{
		//Window or OpenGL context creation failed
		return false;
	}
	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

	//make window current context
	glfwMakeContextCurrent(window);

	//load opengl with glad
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
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	return true;
}

void Application::Run()
{
	using namespace std;
	double deltaTime;
	chrono::steady_clock::time_point lastUpdate = chrono::steady_clock::now();
	chrono::steady_clock::time_point now = chrono::steady_clock::now();

	Sprite sprite("Images/Untitled.png", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//the game loop
	while (!glfwWindowShouldClose(window))
	{
		//set now to chrono::steady_clock::now()
		now = chrono::steady_clock::now();
		//we minus the now and lastUpdate to get the deltaTime
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

		//clear the screen and buffer
		glClearColor(0.2f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		//rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		sprite.Render();

		//put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);

		//update other events like input handling 
		glfwPollEvents();

		lastUpdate = now; //save the previous chrono::steady_clock::now() value
	}
	return;
}


Application::~Application()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//destroy the window
	glfwDestroyWindow(window);
	//terminate glfw
	glfwTerminate();
}

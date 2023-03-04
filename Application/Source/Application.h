#pragma once
#include <GLAD/gl.h>
#include <GLFW/glfw3.h>

// Include IMGUI
// Important: GLEW and GLFW must be included before IMGUI
#ifndef IMGUI_ACTIVE
#include "imgui.h"
#include "Camera.h"
#include "backends\imgui_impl_glfw.h"
#include "backends\imgui_impl_opengl3.h"
#define IMGUI_ACTIVE
#endif

class Application
{
public:
	//application is a singleton
	Application();
	~Application();

	/// <summary>
	/// inits the application and returns false if fail
	/// </summary>
	bool Init();

	/// <summary>
	/// run the application
	/// </summary>
	void Run();

private:

	//window varaibles
	GLFWwindow* window; //the window
	unsigned int width; //window width
	unsigned int height; //window height

	Camera* cam;
};


#pragma once
#define STB_IMAGE_IMPLEMENTATION
//inlcude glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>

#include<GLAD/gl.h>
#include"Vertex.h"
#include"ShaderManager.h"

#include<iostream>
#include <vector>

#include "../AppOptions.h"

///This is where all the mesh structs are stored
struct Transform
{
	glm::vec3 transform;
	glm::vec3 rotation;
};

struct Sprite
{
private:
	// Positions          
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
	unsigned int indexSize;
	unsigned int texture;
	GLuint vao = 0;
	glm::vec4 color;
	//Get both shader code from Basic.shader
	ShaderProgramSource source = ShaderManager::ParseShader("Resources/Shaders/Basic.shader");


public:
	Sprite(const char imagePath[], glm::vec4 color)
	{
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

		if (data == NULL)
		{
			data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		float fWidth = 1.f;
		float fHeight = fWidth * ((float)AppOptions::width / (float)AppOptions::height);
		Vertex v;
		std::vector<Vertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;

		v.position = glm::vec3(-0.5f * fWidth, -0.5f * fHeight, 0);
		v.color = color;
		v.texCoord = glm::vec2(0.0f, 1.0f);
		vertex_buffer_data.push_back(v);

		v.position = glm::vec3(0.5f * fWidth, -0.5f * fHeight, 0);
		v.color = color;
		v.texCoord = glm::vec2(1.0f, 1.0f);
		vertex_buffer_data.push_back(v);

		v.position = glm::vec3(0.5f * fWidth, 0.5f * fHeight, 0);
		v.color = color;
		v.texCoord = glm::vec2(1.0f, 0.0f);
		vertex_buffer_data.push_back(v);

		v.position = glm::vec3(-0.5f * fWidth, 0.5f * fHeight, 0);
		v.color = color;
		v.texCoord = glm::vec2(0, 0);
		vertex_buffer_data.push_back(v);

		index_buffer_data.push_back(3);
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(1);
		index_buffer_data.push_back(2);
		index_buffer_data.push_back(0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

		indexSize = index_buffer_data.size();

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//set runtime color
		this->color = color;
	}

	~Sprite() 
	{
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
	}

	void Render()
	{
		std::cout << "VERTEX SHADER" << std::endl;
		std::cout << source.VertexSource << std::endl;
		std::cout << "FRAGMENT SHADER" << std::endl;
		std::cout << source.FragmentSource << std::endl;

		// Give a unique identifier for the shader to be referenced
		GLuint shaderProgram = ShaderManager::CreateShader(source.VertexSource, source.FragmentSource);
		glUseProgram(shaderProgram);

		glm::mat4 transform;
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
		unsigned int colorLoc = glGetUniformLocation(shaderProgram, "runtimeColor");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// Make sure to initialize matrix to identity matrix first
		transform = glm::mat4(1.0f); 
		transform = glm::translate(transform, glm::vec3(0.f, 0.f, 0.0f));

		// Update the shaders with the latest transform
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		glUniform4fv(colorLoc, 1, glm::value_ptr(color));

		// Get the texture to be rendered
		glBindTexture(GL_TEXTURE_2D, texture);
		   
		// Render the tile
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec4)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	};
};
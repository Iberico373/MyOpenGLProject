#pragma once
#include<glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoord;
	Vertex()
	{
		position = glm::vec3();
		color = glm::vec4(1.0);
		texCoord = glm::vec2();
	}
};
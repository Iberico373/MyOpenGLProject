#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
protected:
	glm::vec3 Position;
	glm::vec3 Target;
	glm::vec3 Up;
	glm::mat4 View;
	glm::mat4 Projection;
	float Fov;
	float NearPlane;
	float FarPlane;
	float AspectRatio;

public:
	Camera(glm::vec3 _Position, float _Fov, float _NearPlane, float _FarPlane, float _AspectRatio);
	Camera(float _Fov, float _NearPlane, float _FarPlane, float _AspectRatio);
	~Camera();

	glm::vec3 GetPosition();
	glm::mat4 GetView();
	glm::mat4 GetProjection();

	void SetPosition(glm::vec3 _Position);
	void SetTarget(glm::vec3 _Target);
	void SetUp(glm::vec3 _Up);
};


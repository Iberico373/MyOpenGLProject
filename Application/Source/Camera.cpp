#include "Camera.h"

Camera::Camera(glm::vec3 _Position, float _Fov, float _NearPlane, float _FarPlane, float _AspectRatio)
{
	Position = _Position;
	Target = glm::vec3(0, 0, 0);
	Up = glm::vec3(0, 1, 0);
	View = glm::lookAt(Position, Target, Up);
	Fov = _Fov;
	NearPlane = _NearPlane;
	FarPlane = _FarPlane;
	AspectRatio = _AspectRatio;
	Projection = glm::perspective(glm::radians(Fov), AspectRatio, NearPlane, FarPlane);
}

Camera::Camera(float _Fov, float _NearPlane, float _FarPlane, float _AspectRatio)
{
	Position = glm::vec3 (0, 0, 0);
	Target = glm::vec3(0, 0, 0);
	Up = glm::vec3(0, 1, 0);
	View = glm::lookAt(Position, Target, Up);
	Fov = _Fov;
	NearPlane = _NearPlane;
	FarPlane = _FarPlane;
	AspectRatio = _AspectRatio;
	Projection = glm::perspective(glm::radians(Fov), AspectRatio, NearPlane, FarPlane);
}

Camera::~Camera()
{
}

glm::vec3 Camera::GetPosition()
{
	return Position;
}

glm::mat4 Camera::GetView()
{
	return View;
}

glm::mat4 Camera::GetProjection()
{
	return Projection;
}

void Camera::SetPosition(glm::vec3 _Position)
{
	Position = _Position;
	View = glm::translate(glm::mat4(), Position);
	return;
}

void Camera::SetTarget(glm::vec3 _Target)
{
	Target = _Target;
	return;
}

void Camera::SetUp(glm::vec3 _Up)
{
	Up = _Up;
	return;
}
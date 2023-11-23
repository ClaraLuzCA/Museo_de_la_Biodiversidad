#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	frontTemp = glm::vec3(0.0f, -1.0f, 0.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
		position.y = 8.0f;
		if (position.z > 20.0f) {
			position.z = 20.0f;
		}else if (position.z < -20.0f) {
			position.z = -20.0f;
		}
		if (position.x > 120.0f) {
			position.x = 120.0f;
		}
		else if (position.x < -120.0f) {
			position.x = -120.0f;
		}
		if ((position.x > -10 && position.x < 10) && (position.z > -10 && position.z < 10)) {
			if (position.x >= -10 && position.x <= 0.0f) {
				position.x = -10;
			}else if (position.x <= 10 && position.x >= 0.0f) {
				position.x = 10;
			}else if (position.z >= -10 && position.z <= 0.0f) {
				position.z = -10;
			}else if (position.z <= 10 && position.z >= 0.0f) {
				position.z = 10;
			}
		}
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
		position.y = 8.0f;
		if (position.z > 20.0f) {
			position.z = 20.0f;
		}
		else if (position.z < -20.0f) {
			position.z = -20.0f;
		}
		if (position.x > 120.0f) {
			position.x = 120.0f;
		}
		else if (position.x < -120.0f) {
			position.x = -120.0f;
		}
		if ((position.x > -10 && position.x < 10) && (position.z > -10 && position.z < 10)) {
			if (position.x >= -10 && position.x <= 0.0f) {
				position.x = -10;
			}
			else if (position.x <= 10 && position.x >= 0.0f) {
				position.x = 10;
			}
			else if (position.z >= -10 && position.z <= 0.0f) {
				position.z = -10;
			}
			else if (position.z <= 10 && position.z >= 0.0f) {
				position.z = 10;
			}
		}
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
		position.y = 8.0f;
		if (position.z > 20.0f) {
			position.z = 20.0f;
		}
		else if (position.z < -20.0f) {
			position.z = -20.0f;
		}
		if (position.x > 120.0f) {
			position.x = 120.0f;
		}
		else if (position.x < -120.0f) {
			position.x = -120.0f;
		}
		if ((position.x > -10 && position.x < 10) && (position.z > -10 && position.z < 10)) {
			if (position.x >= -10 && position.x <= 0.0f) {
				position.x = -10;
			}
			else if (position.x <= 10 && position.x >= 0.0f) {
				position.x = 10;
			}
			else if (position.z >= -10 && position.z <= 0.0f) {
				position.z = -10;
			}
			else if (position.z <= 10 && position.z >= 0.0f) {
				position.z = 10;
			}
		}
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
		position.y = 8.0f;
		if (position.z > 20.0f) {
			position.z = 20.0f;
		}
		else if (position.z < -20.0f) {
			position.z = -20.0f;
		}
		if (position.x > 120.0f) {
			position.x = 120.0f;
		}
		else if (position.x < -120.0f) {
			position.x = -120.0f;
		}
		if ((position.x > -10 && position.x < 10) && (position.z > -10 && position.z < 10)) {
			if (position.x >= -10 && position.x <= 0.0f) {
				position.x = -10;
			}else if (position.x <= 10 && position.x >= 0.0f) {
				position.x = 10;
			}else if (position.z >= -10 && position.z <= 0.0f) {
				position.z = -10;
			}else if (position.z <= 10 && position.z >= 0.0f) {
				position.z = 10;
			}
		}
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::startAnimationCamera(float x, float z, float vista,float inclinacion) {
	position.x = x;
	position.z = z;
	yaw = vista; 
	pitch = inclinacion;
}

void Camera::setCameraPositionMuseo() {
	position.x = -120.0f;
	position.y = 8.0f;
	position.z = 0.0f;
	pitch = 0.0f;
}

void Camera::setCameraPositionDesierto() {
	position.x = -70.0f;
	position.y = 60.0f;
	position.z = 0.0f;
	if (pitch <= -160.0f) {
		pitch = -160.0f;
	}
	else if (pitch >= -20.0f) {
		pitch = -20.0f;
	}
	update();
	
}

void Camera::setCameraPositionSelva() {
	position.x = 70.0f;
	position.y = 60.0f;
	position.z = 0.0f;
	if (pitch <= -160.0f) {
		pitch = -160.0f;
	}
	else if (pitch >= -20.0f) {
		pitch = -20.0f;
	}
	update();

}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}

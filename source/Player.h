#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>


#include"shaderClass.h"
#include <algorithm>



class Player
{
	public:
		// stores the main vectors of the camera
		glm::vec3 CameraPosition;
		glm::vec3 PlayerPosition;

		glm::vec3 CameraOrientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 PlayerOrientation = glm::vec3(0.0f, 0.0f, -1.0f);

		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 cameraMatrix = glm::mat4(1.0f);

		// prevents the camera from jumping around when first clicking left click
		bool firstClick = true;

		// stores the width and height of the window
		int width;
		int height;

		// camera speed and mouse sensitivity
		float defaultSpeed = 2.5f;
		float speed = 2.5f;
		float sprintSpeed = speed * 2.0f;
		float sensitivity = 100.0f;

		// collision detection stuff
		glm::vec3 previousPlayerPosition;
		bool illegalPlayerPosition;

		// gravity stuff
		float fallspeed = 3;
		
		bool grounded{};

		// acceleration
		float accel = 0.0f;
		//float playerSpeed;

		// camera constructor to set up initial values
		Player(int width, int height, glm::vec3 position);

		// updates and exports the camera matrix to the vertex shader
		void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
		void Matrix(Shader& shader, const char* uniform);
		// handles camera inputs
		void Controller(GLFWwindow* window, float deltaTime);
};
#endif

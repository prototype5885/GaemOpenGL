#include"Player.h"

Player::Player(int width, int height, glm::vec3 position)
{
	Player::width = width;
	Player::height = height;
	PlayerPosition = position;
	CameraPosition = position;
}


void Player::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(CameraPosition, CameraPosition + CameraOrientation, Up);


	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Player::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}
float lerp(float from, float to, float t)
{
	return (from * (1 - t) + to * t);
}

void Player::Controller(GLFWwindow* window, float deltaTime)
{
	float currentTime;
	float accel;
	

	//keyboard input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward
	{
		currentTime = glfwGetTime();
		accel = lerp(0.0f, 1.0f, 0.5f);
		PlayerPosition += deltaTime * speed * accel * PlayerOrientation;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // backward
	{
		PlayerPosition += deltaTime * speed * -PlayerOrientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left
	{
		PlayerPosition += deltaTime * speed * -glm::normalize(glm::cross(PlayerOrientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
	{
		PlayerPosition += deltaTime * speed * glm::normalize(glm::cross(PlayerOrientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // up
	{
		PlayerPosition += deltaTime * speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // down
	{
		PlayerPosition += deltaTime * speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // faster start
	{
		speed = sprintSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) // faster end
	{
		speed = defaultSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // backward
	{
		PlayerPosition.y = 5.0f;
	}

	// gravity
	float fallspeed = 3;
	float velocity;

	bool grounded{};
	bool fallStarted{};
	bool fallEnded{};

	if (!grounded && PlayerPosition.y < 0.0f)
	{
		PlayerPosition.y = 0.0f;
		grounded = true;
	}
	else
	{
		grounded = false;
		speed = defaultSpeed / 2;
		PlayerPosition += deltaTime * fallspeed * -Up;
	}



	// mouse input new
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 	// hides mouse cursor

	double mouseX; 	// Stores the coordinates of the cursor
	double mouseY;

	glfwGetCursorPos(window, &mouseX, &mouseY); // Fetches the coordinates of the cursor

	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height; // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width; // and then "transforms" them into degrees
	
	glm::vec3 newOrientation = glm::rotate(CameraOrientation, glm::radians(-rotX), glm::normalize(glm::cross(CameraOrientation, Up))); // Calculates upcoming vertical change in the Orientation

	if (!((glm::angle(newOrientation, Up) <= glm::radians(10.0f)) || glm::angle(newOrientation, -Up) <= glm::radians(10.0f))) // Decides whether or not 
	{																														// the next vertical Orientation is legal or not
		CameraOrientation = newOrientation; 
	}

	PlayerOrientation = glm::rotate(PlayerOrientation, glm::radians(-rotY), Up); // Rotates the Orientation left and right
	CameraOrientation = glm::rotate(CameraOrientation, glm::radians(-rotY), Up); // some temporary fix so camera rotates with the body

	glfwSetCursorPos(window, (width / 2), (height / 2)); // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	// mouse input old
	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) // on mouse press
	//{
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 	// hides mouse cursor

	//	// Prevents camera from jumping on the first click
	//	if (firstClick)
	//	{
	//		glfwSetCursorPos(window, (width / 2), (height / 2));
	//		firstClick = false;
	//	}

	//	// Stores the coordinates of the cursor
	//	double mouseX;
	//	double mouseY;

	//	// Fetches the coordinates of the cursor
	//	glfwGetCursorPos(window, &mouseX, &mouseY);

	//	// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
	//	// and then "transforms" them into degrees
	//	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
	//	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

	//	// Calculates upcoming vertical change in the Orientation
	//	glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

	//	// Decides whether or not the next vertical Orientation is legal or not
	//	if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0f)) or glm::angle(newOrientation, -Up) <= glm::radians(5.0f)))
	//	{
	//		Orientation = newOrientation;

	//	}

	//	// Rotates the Orientation left and right
	//	Orientation = glm:: rotate(Orientation, glm::radians(-rotY), Up);

	//	horizontalOrientation = Orientation;
	//	horizontalOrientation.y = 0.0f;
	//	horizontalOrientation.g = 0.0f;
	//	horizontalOrientation.t = 0.0f;

	//	// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	//	glfwSetCursorPos(window, (width / 2), (height / 2));
	//}
	//else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) // on mouse release
	//{
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // unhides mouse cursor

	//	// Makes sure the next time the camera looks around it doesn't jump
	//	firstClick = true;
	//}
}

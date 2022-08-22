#include <Windows.h>

#include"Model.h"

int width = 1920;
int height = 1080;

unsigned int samples = 0;

int main() 
{
	// obvious what it does
	void HideConsole();
	{
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	}

	// Initialize GLFW
	glfwInit();

	// tell GLFW what version of opengl we have/use (3.3 in this case)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, samples);

	// tell GLFW what opengl profile we use (core here)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creates windowed GLFWwindow
	GLFWwindow* window = glfwCreateWindow(width, height, "XDDDD", NULL, NULL);

	// creates fullscreen GLFW window
	//GLFWwindow* window = glfwCreateWindow(width, height, "XDDDD", glfwGetPrimaryMonitor(), NULL);

	// checks if there is error
	if (window == NULL)
	{
		std::cout << "Fail" << std::endl;
		glfwTerminate();
		return -1;
	}

	// introduce the window into the current context
	glfwMakeContextCurrent(window);

	// load GLAD
	gladLoadGL();

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(2.5f, 0.5f, 3.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// enables depth buffer
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	glEnable(GL_MULTISAMPLE);

	// face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	// creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Load in a model
	//Model ground("models/ground/scene.gltf");
	//Model trees("models/trees/scene.gltf");
	Model modeus("models/modeus/modeus.gltf");

	float deltaTime;
	float lastFrame = 0.0;
	float currentFrame = 0.0;
	unsigned int counter = 0;

	float deltaTime60;
	float lastFrame60 = 0.0;
	unsigned int counter60 = 0;


	// disables vsync
	glfwSwapInterval(0);

	// main while loop
	while (!glfwWindowShouldClose(window))
	{
		glfwGetWindowSize(window, &height, &width);

		// specify the opengl area in the window
		glViewport(0, 0, height, width);

		// runs things 1/2 of the second
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		counter++;

		if (deltaTime >= 1.0 / 2.0)
		{
			std::string FPS = std::to_string((1.0 / deltaTime) * counter);
			std::string ms = std::to_string((deltaTime / counter) * 1000);
			std::string newTitle = "XDDDD - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			lastFrame = currentFrame;
			counter = 0;
		}

		// runs things 1/60 of the second
		deltaTime60 = currentFrame - lastFrame60;
		counter60++;

		if (deltaTime60 >= 1.0 / 60.0)
		{
			lastFrame60 = currentFrame;
			counter60 = 0;

			// handles camera inputs
			camera.Inputs(window);
		}
		

		// specify background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// updates and exports the camera matrix to the vertex shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		//ground.Draw(shaderProgram, camera);
		//trees.Draw(shaderProgram, camera);
		modeus.Draw(shaderProgram, camera);

		// swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// take core of all GLFW events
		glfwPollEvents();
	}
	// Delete all the objects we've created
	shaderProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
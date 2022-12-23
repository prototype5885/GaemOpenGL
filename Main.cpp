#include <Windows.h>

#include"Model.h"

#include <iostream>
#include <fstream>  



// using declartion for cout, endl and string
using namespace std;
//using std::cout;
//using std::endl;
//using std::string;

unsigned int width;
unsigned int height;

unsigned int vsync;
unsigned int fullscreen;

unsigned int AAsamples = 0;

float deltaTime;
float lastFrame = 0.0;
float currentFrame = 0.0;
unsigned int counter = 0;

void game()
{
	// obvious what it does
	//void HideConsole();
	//{
	//	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	//}

	// Initialize GLFW
	glfwInit();

	// tell GLFW what version of opengl we have/use (3.3 in this case)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, AAsamples);

	// tell GLFW what opengl profile we use (core here)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;

	if (fullscreen)
	{
		// creates windowed GLFWwindow
		window = glfwCreateWindow(width, height, "XDDDD", glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		// creates fullscreen GLFW window
		window = glfwCreateWindow(width, height, "XDDDD", NULL, NULL);
	}

	// checks if there is error
	if (window == NULL)
	{
		cout << "Fail" << endl;
		glfwTerminate();
		//return -1;
	}

	// introduce the window into the current context
	glfwMakeContextCurrent(window);

	// load GLAD
	gladLoadGL();

	// specify the opengl area in the window
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(2.5f, 2.5f, 3.5f);
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
	Camera camera(width, height, glm::vec3(0.0f, 1.65f, 3.0f));


	// Load in a model
	Model base("models/godottest/base.gltf");
	Model basewall("models/godottest/basewall.gltf");
	Model modeus("models/godottest/modeus.gltf");


	// vsync
	glfwSwapInterval(vsync);

	// main while loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		counter++;

		string FPS = std::to_string((1.0 / deltaTime) * counter);
		string ms = std::to_string((deltaTime / counter) * 1000);
		string newTitle = "XDDDD - " + FPS + "FPS / " + ms + "ms";
		glfwSetWindowTitle(window, newTitle.c_str());
		counter = 0;

		camera.Inputs(window, deltaTime);

		// specify background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// updates and exports the camera matrix to the vertex shader, FOV, near and far plane
		camera.updateMatrix(68.0f, 0.1f, 100.0f);

		base.Draw(shaderProgram, camera);
		basewall.Draw(shaderProgram, camera);
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
}

void configFile()
{
	// reads config file
	ifstream file("./config/config.cfg");
	string cname;
	int cvalue;
	while (file >> cname >> cvalue)
	{
		if (cname == "vsync") 
		{
			if (cvalue != 0) 
			{
				vsync = 1;
			}
			else
			{
				vsync = 0;
			}
		}
		if (cname == "width")
		{
			width = cvalue;
		}
		if (cname == "height")
		{
			height = cvalue;
		}
		if (cname == "fullscreen")
		{
			if (cvalue != 0)
			{
				fullscreen = 1;
			}
			else
			{
				fullscreen = 0;
			}
		}
	}
	file.close();
}

void checkIfConfigExists() 
{
	// creates config file if it doesnt exist
	ifstream file("./config/config.cfg");
	if (!file)
	{
		ofstream file("./config/config.cfg");
		file << "vsync 0\nwidth 1920\nheight 1080\nfullscreen 0" << endl;
		file.close();
	}
	// goes to read config file
	configFile();
}

int main() 
{
	checkIfConfigExists();
	game();
	return 0;
}


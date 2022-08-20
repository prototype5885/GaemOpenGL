#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"texture.h"
#include<stb/stb_image.h>
#include"shaderClass.h"
#include"VAO.h"
#include"VAO.h"
#include"EBO.h"

// vertices and coordinates
GLfloat vertices[] =
{
	//		COORDINATES	  /		 COLORS       /   UV map?
	-0.5f,	-0.5f,	0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// lower left corner
	-0.5f,	0.5f,	0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// upper left corner
	0.5f,	0.5f,	0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	// upper right corner
	0.5f,	-0.5f,	0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f	// lower left corner
};

GLuint indices[] =
{
	0, 2, 1, // upper triangle
	0, 3, 2  // lower triangle
};

int main() 
{
	// init GLFW
	glfwInit();

	// tell GLFW what version of opengl we have/use (3.3 in this case)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// tell GLFW what opengl profile we use

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create GLFWwindow
	GLFWwindow* window = glfwCreateWindow(800, 800, "XDDDD", NULL, NULL);

	// checks if there is error
	if (window == NULL)
	{
		std::cout << "Fail" << std::endl;
		glfwTerminate();
		return -1;
	}

	// introduce the window into the current context
	glfwMakeContextCurrent(window);

	// disables vsync
	glfwSwapInterval(0);

	// load GLAD
	gladLoadGL();

	// specify the opengl area in the window
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();
	// generates vertex buffer object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// generates element buffer object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture

	Texture modeus("modeus_d.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);


	// main while loop
	while (!glfwWindowShouldClose(window))
	{
		// specify background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clean the back buffer and assign a new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// tell the opengl which shader program we want to use
		shaderProgram.Activate();
		// assigns a value to the uniform (scale)
		glUniform1f(uniID, 1.0f);
		modeus.Bind();
		// bind the VAO so openGL knows how to use it
		VAO1.Bind();
		// draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// take core of all GLFW events
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	modeus.Delete();
	shaderProgram.Delete();

	// delete window 
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
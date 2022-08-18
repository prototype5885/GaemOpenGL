#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() 
{
	// init GLFW
	glfwInit();

	// tell GLFW what version of opengl we use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// tell GLFW what opengl profile we want
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // compatibility mode gives more fps idk
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// vertic es coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // upper corner
	};

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

	// create vertex shader object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach vertex shader source to the vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile the vertex shader
	glCompileShader(vertexShader);

	//same as above but for fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// create shader program object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// attach the vertex and fragment shaders to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// wrap-up/link all the shaders together into the shader program
	glLinkProgram(shaderProgram);

	// delete now useless shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// specify background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// clean the back buffer
	glClear(GL_COLOR_BUFFER_BIT);
	// swap back buffer with front buffer
	glfwSwapBuffers(window);

	// main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// delete window 
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
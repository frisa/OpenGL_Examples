#include <string>
#include <iostream>
#include <fstream>
#include <../../glad/include/glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void openGLRendering(const GLuint shaderProgram, const GLuint VBO, const GLuint EBO, const GLuint VAO, const GLuint texture);
void openGLPrepare(GLuint& VBO, GLuint& EBO, GLuint& VAO, GLuint& texture);
GLuint uiLoadShadersToProgram(const char* cVertexShaderPath, const char* cFragmentShaderPath, bool bMakeDefault);

int main()
{
	GLuint uiVAO;
	GLuint uiEBO;
	GLuint uiVBO;
	GLuint uiTexture;
	GLuint uiShaderProgram;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "The OpenGL example", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	else
	{
		std::cout << "The window has been shown correctly" << std::endl;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "The GLAD has been initialized correctly" << std::endl;
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	uiShaderProgram = uiLoadShadersToProgram("../OpenGL_Examples/shader.vert", "../OpenGL_Examples/shader.frag", false);
	openGLPrepare(uiVBO, uiEBO, uiVAO, uiTexture);
	/* This is the main rendering loop */
	while (!glfwWindowShouldClose(window))
	{
		/* Processing of the input */
		processInput(window);

		/* Rendering commands */
		openGLRendering(uiShaderProgram, uiVBO, uiEBO, uiVAO, uiTexture);

		/* Get the event and swap buffer */
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &uiVAO);
	glDeleteBuffers(1, &uiVBO);
	glDeleteBuffers(1, &uiEBO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void openGLRendering(const GLuint shaderProgram, const GLuint VBO, const GLuint EBO, const GLuint VAO, const GLuint texture)
{
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = 0; 

	/* Start the shading program and set the uniform */
	glUseProgram(shaderProgram);
	vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	/* Clear the window */
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Bind the Buffers */
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);

	/* Set the wireframe mode GL_LINE or GL_FILL*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Draw the Elements */
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void openGLPrepare(GLuint& VBO, GLuint& EBO, GLuint& VAO, GLuint& texture)
{
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	/* Element Array for Recangle */
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,				// first triangle
		1, 2, 3					// second triangle
	};

	float texCoords[] = {
		0.0f, 0.0f,  // lower-left corner  
		1.0f, 0.0f,  // lower-right corner
		0.5f, 1.0f   // top-center corner
	};

	/* Set the texture for the vertexes */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Generate the texture from the file */
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Cannot load the texture" << std::endl;
	}

	/* Get the amount of Vertex Attributes supported by hardware */
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	/* Prepare the vertex/elements arrays */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	/* Create Vertex Buffer */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Create Element Array */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* The vertex position attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* The vertex color attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	/* The vertex texture attribute */
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

GLuint uiLoadShadersToProgram(const char* cVertexShaderPath, const char* cFragmentShaderPath, bool bMakeDefault)
{
	GLuint uiVertexShader;
	GLuint uiFragmentShader;

	std::ifstream vertexShaderFileInBuffer;
	std::ifstream fragmentShaderFileInBuffer;

	uiVertexShader = glCreateShader(GL_VERTEX_SHADER);
	uiFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	if (true == bMakeDefault)
	{
		std::ofstream shaderOutBuffer;

		std::string vertexShaderFile(R"raw(shader.vert)raw");
		std::string fragmentShaderFile(R"raw(shader.frag)raw");

		const char* vertexShaderCode = "#version 440 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}";
		const char* fragmentShaderCode = "#version 440 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"\tFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}";

		/* Create default Vertex Shader */
		shaderOutBuffer = std::ofstream(vertexShaderFile, std::ios::binary);
		if (!shaderOutBuffer.is_open())
		{
			std::cout << "The shader file: " << vertexShaderFile << " can not be created" << std::endl;
			return 0;
		}
		shaderOutBuffer << vertexShaderCode;
		shaderOutBuffer.close();

		/* Create default Fragment Shader */
		shaderOutBuffer = std::ofstream(fragmentShaderFile, std::ios::binary);
		if (!shaderOutBuffer.is_open())
		{
			std::cout << "The shader file: " << vertexShaderFile << " can not be created" << std::endl;
			return 0;
		}
		shaderOutBuffer << fragmentShaderCode;
		shaderOutBuffer.close();
		/* Get the Shaders Input Buffers for defualt files */
		vertexShaderFileInBuffer = std::ifstream(cVertexShaderPath, std::ios::binary);
		fragmentShaderFileInBuffer = std::ifstream(cFragmentShaderPath, std::ios::binary);
	}
	else
	{
		/* Get the Shaders Input Buffers */
		vertexShaderFileInBuffer = std::ifstream(cVertexShaderPath, std::ios::binary);
		fragmentShaderFileInBuffer = std::ifstream(cFragmentShaderPath, std::ios::binary);
	}
	std::string sVertexShaderSource((std::istreambuf_iterator<char>(vertexShaderFileInBuffer)), std::istreambuf_iterator<char>());
	std::string sFragmentShaderSource((std::istreambuf_iterator<char>(fragmentShaderFileInBuffer)), std::istreambuf_iterator<char>());
	const char* pcVertexShaderSource = sVertexShaderSource.c_str();
	const char* pcFragmentShaderSource = sFragmentShaderSource.c_str();

	/* Compile Vertex Shader */
	std::cout << "Compile Vertex Shader: \n" << pcVertexShaderSource << std::endl;
	glShaderSource(uiVertexShader, 1, &pcVertexShaderSource, NULL);
	glCompileShader(uiVertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(uiVertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(uiVertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	else
	{
		std::cout << "\nCompilation successfull" << std::endl;
	}

	/* Compile Fragment Shader */
	std::cout << "Compile Fragment Shader: \n" << pcFragmentShaderSource << std::endl;
	glShaderSource(uiFragmentShader, 1, &pcFragmentShaderSource, NULL);
	glCompileShader(uiFragmentShader);
	glGetShaderiv(uiFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(uiFragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	else
	{
		std::cout << "\nCompilation successfull" << std::endl;
	}

	/* Create Shader Program */
	GLuint uiShaderProgram;
	uiShaderProgram = glCreateProgram();
	glAttachShader(uiShaderProgram, uiVertexShader);
	glAttachShader(uiShaderProgram, uiFragmentShader);
	glLinkProgram(uiShaderProgram);
	glGetProgramiv(uiShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(uiShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	else
	{
		std::cout << "\nLinking successfull" << std::endl;
	}
	glDeleteShader(uiVertexShader);
	glDeleteShader(uiFragmentShader);
	return uiShaderProgram;
}
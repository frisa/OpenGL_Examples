#include <iostream>
#include <../../glad/include/glad/glad.h>
#include <GLFW/glfw3.h>



const char* vertexShaderSource = 
"#version 440 core \n\
layout(location = 0) in vec3 aPos;\
void main()\
{\
gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\
}";

const char* fragmentShaderSource = 
"#version 330 core\n\
out vec4 FragColor;\
\
void main()\
{\
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\
}";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void openGLRendering(const unsigned int shaderProgram, const unsigned int VBO, const unsigned int EBO, const unsigned int VAO);
void openGLPrepare(unsigned int& shaderProgram, unsigned int& VBO, unsigned int& EBO, unsigned int& VAO);

int main()
{
	unsigned int u32ShaderProgram;
	unsigned int u32VAO;
	unsigned int u32EBO;
	unsigned int u32VBO;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
	openGLPrepare(u32ShaderProgram, u32VBO, u32EBO, u32VAO);
	/* This is the main rendering loop */
	while (!glfwWindowShouldClose(window))
	{
		/* Processing of the input */
		processInput(window);

		/* Rendering commands */
		openGLRendering(u32ShaderProgram, u32VBO, u32EBO, u32VAO);

		/* Get the event and swap buffer */
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
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

void openGLRendering(const unsigned int shaderProgram, const unsigned int VBO, const unsigned int EBO, const unsigned int VAO)
{
	/* Clear the window */
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	/* Start the shading programs */
	glUseProgram(shaderProgram);
	/* Bind the Buffers */
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	/* Set the wireframe mode GL_LINE or GL_FILL*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/* Draw the Elements */
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void openGLPrepare(unsigned int& shaderProgram, unsigned int& VBO, unsigned int& EBO, unsigned int& VAO)
{
	/* Vertex Array for rectangle */
	float verticesRectangle[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	/* Element Array for Recangle */
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	unsigned int vertexShader;
	unsigned int fragmentShader;

	/* Create Vertex Shader */
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/* Create Fragment Shader */
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/* Create Shader Program */
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* Create Vertex Array for Rectangle */
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRectangle), verticesRectangle, GL_STATIC_DRAW);

	/* Create Element Array for Rectangle */
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* Create Vertex Attribute Object for Rectangle */
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
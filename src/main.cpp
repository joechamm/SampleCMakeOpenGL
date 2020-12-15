#include "rendering/LoadShaders.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>

#define BUFFER_OFFSET(x) ((void*)(x))

GLuint vao[1];
GLuint vbo[1];

GLuint 
	shaderProgram,
	aPositionIdx,
	aColorIdx;

GLFWwindow * window = nullptr;

struct vertex
{
	GLfloat pos[2];
	GLfloat color[3];
};

/// <summary>
/// GLFW error callback function 
/// </summary>
/// <param name="err"></param>
/// <param name="description"></param>
static void error_callback(int err, const char* description) 
{
	fprintf(stderr, "Error: %s\n", description);
}

/// <summary>
/// GLFW key callback function
/// </summary>
/// <param name="window"></param>
/// <param name="key"></param>
/// <param name="scancode"></param>
/// <param name="action"></param>
/// <param name="mods"></param>
static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

/// <summary>
/// Initialize GLFW and generate buffer for our vertices
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool init(void)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		fprintf(stderr, "Error: Failed to initialize GLFW!");
		return false;
	}
	window = glfwCreateWindow(800, 600, "Sample CMake OpenGL", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		fprintf(stderr, "Error: glfwCreateWindow returned null");
		return false;
	}

	glfwMakeContextCurrent(window);
	gl3wInit();

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "res/shaders/simple.vert.glsl" },
		{ GL_FRAGMENT_SHADER, "res/shaders/simple.frag.glsl" },
		{ GL_NONE, nullptr }
	};

	shaderProgram = LoadShaders(shaders);
	glUseProgram(shaderProgram);
	aPositionIdx = glGetAttribLocation(shaderProgram, "aPosition");
	aColorIdx = glGetAttribLocation(shaderProgram, "aColor");

	const vertex vertices[3] = {
		{
			{ -1.0f, -0.5f }, { 1.0f, 0.0f, 0.0f }
		},
		{
			{ 1.0f, -0.5f }, { 0.0f, 1.0f, 0.0f }
		},
		{
			{ 0.0f, 0.5f }, { 0.0f, 0.0f, 1.0f }
		}
	};

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glCreateBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);

	glUseProgram(shaderProgram);
	glVertexAttribPointer(aPositionIdx, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
	glVertexAttribPointer(aColorIdx, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(sizeof(vertices[0].pos)));

	glEnableVertexAttribArray(aPositionIdx);
	glEnableVertexAttribArray(aColorIdx);

	return true;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, char** argv)
{
	init();

	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
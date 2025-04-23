#include <GL/gl.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>

#include "shaders/vertex_shader.h"
#include "shaders/fragment_shader.h"

extern unsigned char font[96*96];

GLuint build_shader_program(const char *vertex_shader_source, const char *fragment_shader_source);

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
		int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Hello", NULL, NULL);
	assert(window);

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// Build shader program
	GLuint program = build_shader_program(vertex_shader_source, fragment_shader_source);
	assert(program != 0);
	glUseProgram(program);


	// Load texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 96, 96, 0, GL_ALPHA, GL_UNSIGNED_BYTE, font);


	// Setup viewport
	glViewport(0, 0, 640, 480);
	glUniform2f(glGetUniformLocation(program, "u_Viewport"), 2.0f / 640, 2.0f / 480);

	// x, y, s, t
	GLshort vertices[] = {
		0, 0, 0, 0,
		0, 50, 0, 96,
		50, 0, 96, 0,
		50, 50, 96, 96,
	};


	// Coords
	glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 4 * sizeof(GLshort), (void *) vertices);
	glVertexAttribPointer(1, 2, GL_SHORT, GL_FALSE, 4 * sizeof(GLshort), (void *) vertices + 2 * sizeof(GLshort));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Colors
	float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
	};
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *) colors);
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(2);

	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 0.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform3f(glGetUniformLocation(program, "u_Color"), 0.2f, 0.0f, 1.0f);
		glUniform2f(glGetUniformLocation(program, "u_Offset"), 0, 0);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

GLuint build_shader_program(const char *vertex_shader_source, const char *fragment_shader_source)
{
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	GLint compiled;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		GLchar log[1024];
		glGetShaderInfoLog(vertex_shader, 1024, NULL, log);
		fprintf(stderr, "Vertex shader compilation error:\n%s\n", log);
		glDeleteShader(vertex_shader);
		return 0;
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE) {
		GLchar log[1024];
		glGetShaderInfoLog(fragment_shader, 1024, NULL, log);
		fprintf(stderr, "Fragment shader compilation error:\n%s\n", log);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return 0;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (linked != GL_TRUE) {
		GLchar log[1024];
		glGetProgramInfoLog(program, 1024, NULL, log);
		fprintf(stderr, "Shader program linking error:\n%s\n", log);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		glDeleteProgram(program);
		return 0;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return program;
}

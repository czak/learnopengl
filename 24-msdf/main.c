#include <assert.h>
#include <GLFW/glfw3.h>
#include <GLES2/gl2.h>

#include "shaders/vertex_shader.h"
#include "shaders/fragment_shader.h"

extern unsigned char font[512*512*3];

static GLuint program;
static GLuint texture;

static struct vertex {
	GLshort x, y;
	GLushort s, t;
} vertices[4];

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Hello", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);


	// Build shader program
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	assert(success);

	glUseProgram(program);


	// Load texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, font);


	// Setup viewport
	glViewport(0, 0, 640, 480);
	glUniform2f(glGetUniformLocation(program, "u_Viewport"), 2.0f / 640, 2.0f / 480);


	// Build geometry
	vertices[0] = (struct vertex){ 0, 0, 0, 0 };
	vertices[1] = (struct vertex){ 0, 256, 0, 512 };
	vertices[2] = (struct vertex){ 256, 0, 512, 0 };
	vertices[3] = (struct vertex){ 256, 256, 512, 512 };


	// Prepare to draw quads with texture coords
	glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, sizeof(struct vertex), (void *) vertices);
	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(struct vertex), (void *) vertices + 2 * sizeof(GLushort));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	// Alpha blending
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 0.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform3f(glGetUniformLocation(program, "u_Color"), 0.2f, 0.0f, 1.0f);
		glUniform2f(glGetUniformLocation(program, "u_Offset"), 0, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

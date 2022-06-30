#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char *vertexShaderSource = R"glsl(
#version 330 core
layout (location=0) in vec3 aPos;
uniform mat4 model;
void main(){
gl_Position = model* vec4(aPos, 1.0);
}
)glsl";
const char *fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
uniform vec3 aColor;
void main(){
FragColor = vec4(aColor, 1.0f);
}
)glsl";

const unsigned int WINDOW_WIDTH = 600;
const unsigned int WINDOW_HEIGHT = 600;

int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
	                                      "opengl-lines", NULL, NULL);
	if (window == NULL) {
		std::cout << "Unable to create window!" << std::endl;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Unable to load glad!" << std::endl;
	}

	int noOfSides = 40;
	float outerRadius = 1.0f, innerRadius = 0.8f;

	float outerVertices[(noOfSides + 1) * 3 + 3];
	float innerVertices[(noOfSides + 1) * 3 + 3];

	outerVertices[0] = 0.0f;
	outerVertices[1] = 0.0f;
	outerVertices[2] = 0.0f;

	innerVertices[0] = 0.0f;
	innerVertices[1] = 0.0f;
	innerVertices[2] = 0.0f;

	for (int i = 0; i <= noOfSides; i++) {
		float theta = i / float(noOfSides) * (M_PI * 2);
		outerVertices[(i + 1) * 3] = outerRadius * std::cos(theta);
		outerVertices[(i + 1) * 3 + 1] = outerRadius * std::sin(theta);
		outerVertices[(i + 1) * 3 + 2] = 0;

		innerVertices[(i + 1) * 3] = innerRadius * std::cos(theta);
		innerVertices[(i + 1) * 3 + 1] = innerRadius * std::sin(theta);
		innerVertices[(i + 1) * 3 + 2] = 0;
	}

	float handVertices[] = {0.0f,  0.0f,   0.0f, //
	                        0.05f, 0.01f,  0.0f, //
	                        0.05f, -0.01f, 0.0f, //
	                        0.05f, 0.01f,  0.0f, //
	                        0.8f,  0.01f,  0.0f, //
	                        0.8f,  -0.01f, 0.0f, //
	                        0.8f,  -0.01f, 0.0f, //
	                        0.05f, -0.01f, 0.0f, //
	                        0.05f, 0.01f,  0.0f};

	unsigned int VAO[3], VBO[3];
	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(outerVertices), outerVertices,
	             GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
	                      (void *)0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(innerVertices), innerVertices,
	             GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
	                      (void *)0);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(handVertices), handVertices,
	             GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
	                      (void *)0);

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
		          << infoLog << std::endl;
	}
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
		          << infoLog << std::endl;
	}
	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
		          << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);
	int r = 0, g = 0, b = 0;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO[0]);

		glm::mat4 model(1.0f);

		glUniform3fv(glGetUniformLocation(shaderProgram, "aColor"), 1,
		             glm::value_ptr(glm::vec3(0.9, 0.5, 0.5)));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
		                   GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLE_FAN, 0, noOfSides + 2);

		glBindVertexArray(VAO[1]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "aColor"), 1,
		             glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		glDrawArrays(GL_TRIANGLE_FAN, 0, noOfSides + 2);

		time_t now = time(0);
		tm *ltm = localtime(&now);

		float hour_rotate = M_PI_2 - 2 * M_PI * (ltm->tm_hour % 12) / 12.0f;
		float min_rotate = M_PI_2 - 2 * M_PI * ltm->tm_min / 60.0f;
		float second_rotate = M_PI_2 - 2 * M_PI * ltm->tm_sec / 60.0f;

		glBindVertexArray(VAO[2]);

		// hour
		glUniform3fv(glGetUniformLocation(shaderProgram, "aColor"), 1,
		             glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));
		model = glm::mat4(1.0f);
		model = glm::rotate(model, hour_rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
		                   GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 9);
		// -----

		// min
		glUniform3fv(glGetUniformLocation(shaderProgram, "aColor"), 1,
		             glm::value_ptr(glm::vec3(0.0, 1.0, 0.0)));
		model = glm::mat4(1.0f);
		model = glm::rotate(model, min_rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.7f, 1.2f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
		                   GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 9);
		// ----

		// second
		model = glm::mat4(1.0f);
		glUniform3fv(glGetUniformLocation(shaderProgram, "aColor"), 1,
		             glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
		model = glm::rotate(model, second_rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
		                   GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 9);
		// ----
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

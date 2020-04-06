#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "shader.h"
#include "rk4solver.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

unsigned int VAO, EBO;
unsigned int texture1;
unsigned int texture2;
unsigned int linesVAO;

float vertices[] = {
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f)
};

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float pitch = 0.0f, yaw = -90.0f;
bool firstMouse = true;
float lastX = 400, lastY = 300;

int shader, lineShader;
bool isCameraMoving = false;

const int num_points = 3;
struct point {
	glm::vec3 position;
};
point points[num_points];

void init() {
	//float vertices[] = {
	//	// positions          // colors           // texture coords
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
	//};
	//unsigned int indices[] = {  // note that we start from 0!
	//	0, 1, 3,   // first triangle
	//	1, 2, 3    // second triangle
	//};

	shader = Shader::createShader("shaders/shader.vs", "shaders/shader.fs");

	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// copy vertices into a vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// copy indices into an element buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	// colors
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	// unbind the VBO from the GL_ARRAY_BUFFER target by binding 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// same for vertex array, but not really necessary
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// load and create textures
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	lineShader = Shader::createShader("shaders/lineShader.vs", "shaders/lineShader.fs");

	glGenVertexArrays(1, &linesVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(linesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/*float line_vertices[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};*/

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void update(RK4Solver solver) {
	//if (solver.state[0].y < 0.0f) {
	//	solver.state[1].y = -solver.state[1].y;
	//}
	solver.update(deltaTime);
	points[0].position = solver.state[0];
	points[1].position = solver.state[2];
	points[2].position = solver.state[4];
}

void render(RK4Solver solver) {
	float worldScale = 0.5f;

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	Shader::setMat4(shader, "view", view);

	glm::mat4 projection;
	projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
	Shader::setMat4(shader, "projection", projection);
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);

	// draw origin point
	Shader::use(lineShader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	Shader::setMat4(shader, "model", model);
	Shader::setMat4(shader, "view", view);
	Shader::setMat4(shader, "projection", projection);
	Shader::setVec4(lineShader, "color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	glBindVertexArray(linesVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	for (int i = 0; i < num_points; i++) {
		if (i == num_points - 1) {
			Shader::use(shader);
			Shader::setInt(shader, "texture1", 0);
			Shader::setInt(shader, "texture2", 1);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(worldScale, worldScale, worldScale));
			model = glm::translate(model, points[i].position);
			Shader::setMat4(shader, "model", model);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else {
			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3 ab = points[i + 1].position - points[i].position;
			glm::vec3 dir = glm::normalize(ab);
			float len = worldScale * glm::length(ab);

			model = glm::translate(model, worldScale * points[i].position + (len / 2.0f) * dir);
			if (yAxis != dir) {
				float cosAngle = glm::dot(yAxis, dir);
				if (cosAngle != -1.0f) {
					glm::vec3 c = glm::cross(yAxis, dir);
					//printf("%f %f %f\n", c.x, c.y, c.z);
					float theta = glm::degrees(glm::acos(glm::dot(yAxis, dir)));
					model = glm::rotate(model, theta, c);
				}
				else {
					model = glm::rotate(model, 180.0f, xAxis);
				}
			}
			model = glm::scale(model, glm::vec3(0.05f, len, 0.05f));

			Shader::setMat4(shader, "model", model);
			Shader::setMat4(shader, "view", view);
			Shader::setMat4(shader, "projection", projection);
			Shader::setVec4(lineShader, "color", glm::vec4((i + 1)/num_points, 0.0f, 0.0f, 1.0f));
			glBindVertexArray(linesVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

glm::vec3 dxdt(float t, glm::vec3* state) {
	return state[1];
}

glm::vec3* update_f(float t, int n_vars, glm::vec3* state) {
	glm::vec3* ret = new glm::vec3[n_vars];
	ret[0] = state[1];
	ret[1] = glm::vec3(0.0f, -9.8f, 0.0f);

	return ret;
}

glm::vec3* update_f2(float t, int n_vars, glm::vec3* state) {
	float spring_k_1 = 3.0f;
	float spring_k_2 = 3.0f;
	float spring_rest_1 = 5.0f;
	float spring_rest_2 = 5.0f;
	float c = 0.5f;

	glm::vec3* ret = new glm::vec3[n_vars];

	// calculate force on first joint
	glm::vec3 joint_1_v = state[3];
	glm::vec3 pos = state[2];
	glm::vec3 origin = state[0];
	float stretch_len_1 = glm::length(pos - origin) - spring_rest_1;
	glm::vec3 dir1 = glm::normalize(pos - origin);
	float stretch_len_2 = glm::length(state[4] - pos) - spring_rest_2;
	glm::vec3 dir2 = glm::normalize(state[4] - pos);
	glm::vec3 joint_1_f = (-spring_k_1 * stretch_len_1 * dir1) + (spring_k_2 * stretch_len_2 * dir2) - joint_1_v*c;

	// calculate force on last block
	glm::vec3 block_v = state[5];
	pos = state[4];
	origin = state[2];
	stretch_len_1 = glm::length(pos - origin) - spring_rest_2;
	dir1 = glm::normalize(pos - origin);
	glm::vec3 block_f = (-spring_k_2 * stretch_len_1 * dir1) + glm::vec3(0.0f, -9.8f, 0.0f) - block_v * c;

	ret[0] = state[1] + (isCameraMoving ? glm::vec3(5.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 0.0f, 0.0f));
	ret[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	ret[2] = joint_1_v;
	ret[3] = joint_1_f / 1.0f;
	ret[4] = block_v;
	ret[5] = block_f / 1.0f;

	//float spring_constant = 3.0f;
	//float c = 0.5f;
	//float spring_rest = 5.0f;

	//glm::vec3* ret = new glm::vec3[n_vars];
	//float stretch = glm::length(state[2] - state[0]) - spring_rest;
	//glm::vec3 spring_dir = glm::normalize(state[2] - state[0]);
	//glm::vec3 force = -spring_constant * stretch * spring_dir - (state[3] * c) + glm::vec3(0.0f, -9.8f, 0.0f);
	//ret[0] = state[1] + (isCameraMoving ? glm::vec3(5.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 0.0f, 0.0f));
	//ret[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	//ret[2] = state[3];
	//ret[3] = force;

	return ret;
}

int main() {
	//glm::vec3 state[] = {
	//	glm::vec3(0.0f, 10.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f)
	//};

	glm::vec3 state[] = {
		// spring origin
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		// spring joint 1
		glm::vec3(4.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		// spring end
		glm::vec3(8.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	};

	RK4Solver solver = RK4Solver(6, state, update_f2, 0.0f);
	//printf(
	//	"%f %f %f\n%f %f %f",
	//	solver.state[0].x, solver.state[0].y, solver.state[0].z,
	//	solver.state[1].x, solver.state[1].y, solver.state[1].z
	//);
	//solver.update();
	//printf(
	//	"\n%f %f %f\n%f %f %f",
	//	solver.state[0].x, solver.state[0].y, solver.state[0].z,
	//	solver.state[1].x, solver.state[1].y, solver.state[1].z
	//);
	//return 0;
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		printf("Error starting GLEW: %s\n", glewGetErrorString(error));
		exit(0);
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	init();

	//unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		update(solver);
		render(solver);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	isCameraMoving = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;
}

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
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
unsigned int linesVAO, water_vao;

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

float water_vertices[] = {
-0.5f, -0.5f, -0.5f, 0.09, 0.15, 0.34,
 0.5f, -0.5f, -0.5f, 0.09, 0.15, 0.34,
 0.5f,  0.5f, -0.5f, 0.30, 0.84, 0.82,
 0.5f,  0.5f, -0.5f, 0.30, 0.84, 0.82,
-0.5f,  0.5f, -0.5f, 0.30, 0.84, 0.82,
-0.5f, -0.5f, -0.5f, 0.09, 0.15, 0.34,

-0.5f,  0.5f,  0.5f, 0.30, 0.84, 0.82,
-0.5f,  0.5f, -0.5f, 0.30, 0.84, 0.82,
-0.5f, -0.5f, -0.5f, 0.09, 0.15, 0.34,
-0.5f, -0.5f, -0.5f, 0.09, 0.15, 0.34,
-0.5f, -0.5f,  0.5f, 0.09, 0.15, 0.34,
-0.5f,  0.5f,  0.5f, 0.30, 0.84, 0.82,

 0.5f,  0.5f,  0.5f, 0.30, 0.84, 0.82,
 0.5f,  0.5f, -0.5f, 0.30, 0.84, 0.82,
 0.5f, -0.5f, -0.5f, 0.09, 0.15, 0.34,
 0.5f, -0.5f, -0.5f, 0.09, 0.15, 0.34,
 0.5f, -0.5f,  0.5f, 0.09, 0.15, 0.34,
 0.5f,  0.5f,  0.5f, 0.30, 0.84, 0.82,

-0.5f, -0.5f, -0.5f, 0.09, 0.15, 0.34,
 0.5f, -0.5f, -0.5f, 0.09, 0.15, 0.34,
 0.5f, -0.5f,  0.5f, 0.09, 0.15, 0.34,
 0.5f, -0.5f,  0.5f, 0.09, 0.15, 0.34,
-0.5f, -0.5f,  0.5f, 0.09, 0.15, 0.34,
-0.5f, -0.5f, -0.5f, 0.09, 0.15, 0.34
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f)
};

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraVel = glm::vec3(0.0f, 0.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float pitch = 0.0f, yaw = -90.0f;
bool firstMouse = true;
float lastX = 400, lastY = 300;

int shader, lineShader, waterShader;
bool isCameraMoving = false;

float box_boundary_y1 = -2.0f;
float box_boundary_y2 = -4.0f;
float box_boundary_x = 2.5f;

const int num_points = 30;
const int num_fish = 10;
struct point {
	glm::vec3 position;
};
point points[num_points];

int caught_fish_idx = -1;

struct fish {
	float last_update;
	glm::vec3 position;
	glm::vec3 target;
	float strength;
};
fish fishes[num_fish];

glm::vec3 random_vec3() {
	float x = (float)rand() / RAND_MAX - 0.5f;
	float y = (float)rand() / RAND_MAX - 0.5f;
	float z = (float)rand() / RAND_MAX - 0.5f;

	return glm::normalize(glm::vec3(x, y, z));
}

void init_fish() {
	for (int i = 0; i < num_fish; i++) {
		fishes[i].last_update = glfwGetTime();
		fishes[i].strength = (float)rand() / RAND_MAX;
	}
}

void init() {
	srand(time(NULL));

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	waterShader = Shader::createShader("shaders/waterShader.vs", "shaders/waterShader.fs");
	// water
	glGenVertexArrays(1, &water_vao);
	glGenBuffers(1, &VBO);
	glBindVertexArray(water_vao);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(water_vertices), water_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	init_fish();
}

void update(RK4Solver solver) {
	//if (solver.state[0].y < 0.0f) {
	//	solver.state[1].y = -solver.state[1].y;
	//}
	solver.update(deltaTime);
	for (int i = 0; i < num_points; i++) {
		if (glm::abs(solver.state[2 * i].x) > box_boundary_x && solver.state[2 * i].y < box_boundary_y1) {
			float neg = solver.state[2 * i].x / glm::abs(solver.state[2 * i].x);
			solver.state[2 * i].x = neg * box_boundary_x;
		}
		if (glm::abs(solver.state[2 * i].z) > box_boundary_x && solver.state[2 * i].y < box_boundary_y1) {
			float neg = solver.state[2 * i].z / glm::abs(solver.state[2 * i].z);
			solver.state[2 * i].z = neg * box_boundary_x;
		}
		if (solver.state[2 * i].y < box_boundary_y2) {
			solver.state[2 * i].y = box_boundary_y2;
		}
		points[i].position = solver.state[2 * i];
	}

	for (int i = num_points; i < num_points + num_fish; i++) {
		if (glm::abs(solver.state[2 * i].x) > box_boundary_x) {
			float neg = solver.state[2 * i].x / glm::abs(solver.state[2 * i].x);
			solver.state[2 * i].x = neg * box_boundary_x;
			//solver.state[2 * i + 1] = -solver.state[2 * i + 1];
			//fishes[i].velocity = -solver.state[2 * i + 1];
		}
		if (glm::abs(solver.state[2 * i].z) > box_boundary_x) {
			float neg = solver.state[2 * i].z / glm::abs(solver.state[2 * i].z);
			solver.state[2 * i].z = neg * box_boundary_x;
			//solver.state[2 * i + 1] = -solver.state[2 * i + 1];
			//fishes[i].velocity = -solver.state[2 * i + 1];
		}
		if (solver.state[2 * i].y > box_boundary_y1) {
			solver.state[2 * i].y = box_boundary_y1;
			//solver.state[2 * i + 1] = -solver.state[2 * i + 1];
			//fishes[i].velocity = -solver.state[2 * i + 1];
		}
		if (solver.state[2 * i].y < box_boundary_y2) {
			solver.state[2 * i].y = box_boundary_y2;
			//solver.state[2 * i + 1] = -solver.state[2 * i + 1];
			//fishes[i].velocity = -solver.state[2 * i + 1];
		}

		bool is_hook_occupied = caught_fish_idx >= 0;
		int fish_i = i - num_points;

		// update current fish's target
		float current_time = glfwGetTime();
		float update_interval = 2.0f;
		// update every 1 second if the fish is hooked
		if (fish_i == caught_fish_idx) update_interval = 0.5f;

		if (current_time - fishes[fish_i].last_update > update_interval) {
			fishes[fish_i].target = random_vec3();
			fishes[fish_i].last_update = current_time;
		}

		glm::vec3 hook_pos = points[num_points - 1].position;
		glm::vec3 pos = solver.state[2 * i];
		if (!is_hook_occupied && glm::length(pos - hook_pos) < 0.1f) {
			solver.state[2 * i] = hook_pos;
			// TODO: set the target to somewhere interesting...
			caught_fish_idx = fish_i;
			fishes[caught_fish_idx].target = random_vec3();
			fishes[caught_fish_idx].last_update = current_time;
			fishes[caught_fish_idx].strength += 10.0f;
		}
		else {
			if (is_hook_occupied) {
				if (fish_i == caught_fish_idx) {
					solver.state[2 * i] = hook_pos;
				}
				//else fishes[fish_i].target = -glm::normalize(hook_pos - pos);
			}
			else {
				fishes[fish_i].target = glm::normalize((hook_pos - pos) + fishes[fish_i].target);
			}
		}

		fishes[fish_i].position = solver.state[2 * i];
		//fishes[fish_i].target = solver.state[2 * i + 1];
	}
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
	Shader::setMat4(lineShader, "model", model);
	Shader::setMat4(lineShader, "view", view);
	Shader::setMat4(lineShader, "projection", projection);
	Shader::setVec4(lineShader, "color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	glBindVertexArray(linesVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// draw grid points
	//Shader::setVec4(lineShader, "color", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	//for (int x = -5; x <= 5; x++) {
	//	for (int y = -5; y <= 5; y++) {
	//		for (int z = -5; z <= 5; z++) {
	//			model = glm::mat4(1.0f);
	//			model = glm::translate(model, glm::vec3((float)x, (float)y, (float)z));
	//			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	//			Shader::setMat4(lineShader, "model", model);
	//			glDrawArrays(GL_TRIANGLES, 0, 36);
	//		}
	//	}
	//}

	for (int i = 0; i < num_points - 1; i++) {
		//if (i == num_points - 1) {
		//	Shader::use(shader);
		//	Shader::setInt(shader, "texture1", 0);
		//	Shader::setInt(shader, "texture2", 1);
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::scale(model, glm::vec3(worldScale, worldScale, worldScale));
		//	model = glm::translate(model, points[i].position);
		//	Shader::setMat4(shader, "model", model);

		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture1);
		//	glActiveTexture(GL_TEXTURE1);
		//	glBindTexture(GL_TEXTURE_2D, texture2);
		//	glBindVertexArray(VAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 ab = points[i + 1].position - points[i].position;
		glm::vec3 dir = glm::normalize(ab);
		float len =  glm::length(ab);

		model = glm::translate(model, points[i].position + (len / 2.0f) * dir);
		float cosAngle = glm::dot(yAxis, dir);
		float tol = 0.000001f;
		if (glm::abs(glm::abs(cosAngle) - 1.0f) > tol) {
			glm::vec3 c = glm::cross(yAxis, dir);
			//printf("%f %f %f\n", c.x, c.y, c.z);
			float theta = glm::degrees(glm::acos(glm::dot(yAxis, dir)));

			model = glm::rotate(model, theta, c);
		}
		else {
			model = glm::rotate(model, 180.0f, zAxis);
		}

		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		if (i > num_points - 10) {
			// draw worm
			color = glm::vec4(1.0f, 0.70f, 0.58f, 1.0f);
			model = glm::scale(model, glm::vec3(0.05f, len, 0.05f));
		}
		else {
			model = glm::scale(model, glm::vec3(0.02f, len, 0.02f));
		}

		Shader::setMat4(lineShader, "model", model);
		Shader::setMat4(lineShader, "view", view);
		Shader::setMat4(lineShader, "projection", projection);

		Shader::setVec4(lineShader, "color", color);
		glBindVertexArray(linesVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	// draw fishes
	for (int i = 0; i < num_fish; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, fishes[i].position);

		glm::vec3 fish_velocity = solver.state[(num_points * 2) + (i * 2) + 1];
		glm::vec3 dir = glm::normalize(fish_velocity);
		float cosAngle = glm::dot(xAxis, dir);
		float tol = 0.000001f;
		if (glm::abs(glm::abs(cosAngle) - 1.0f) > tol) {
			glm::vec3 c = glm::cross(xAxis, dir);
			//printf("%f %f %f\n", c.x, c.y, c.z);
			float theta = glm::degrees(glm::acos(glm::dot(xAxis, dir)));

			model = glm::rotate(model, theta, c);
		}
		else {
			if (cosAngle < 0) model = glm::rotate(model, 180.0f, yAxis);
		}

		model = glm::scale(model, glm::vec3(0.5f, 0.1f, 0.1f));

		Shader::setMat4(lineShader, "model", model);
		Shader::setVec4(lineShader, "color", glm::vec4(1.0f, 0.74f, 0.37f, 1.0f));

		glBindVertexArray(linesVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// draw water box
	Shader::use(waterShader);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 2.0f, 5.0f));
	Shader::setMat4(waterShader, "model", model);
	Shader::setMat4(waterShader, "view", view);
	Shader::setMat4(waterShader, "projection", projection);
	glBindVertexArray(water_vao);
	glDrawArrays(GL_TRIANGLES, 0, 24);
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
	glm::vec3* ret = new glm::vec3[n_vars];

	// origin point
	ret[0] = state[1] + cameraVel;
	//(isCameraMoving ? glm::vec3(5.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 0.0f, 0.0f));
	ret[1] = glm::vec3(0.0f, 0.0f, 0.0f);

	float ks[num_points-1] = {
		200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f
	};
	float spring_rests[num_points-1] = {
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
	};

	float c = 0.5f;
	float rest_length = 0.001f;
	float k = 75.0f;

	// generate the derivatives for the joints
	for (int i = 2; i < num_points * 2; i += 2) {
		glm::vec3 v = state[i + 1];
		glm::vec3 pos = state[i];

		glm::vec3 prevPos = state[i - 2];
		//float stretch1 = glm::length(pos - prevPos) - spring_rests[i / 2 - 1];
		float stretch1 = glm::length(pos - prevPos) - rest_length;
		glm::vec3 dir1 = glm::normalize(pos - prevPos);
		//float k1 = ks[i / 2 - 1];
		float k1 = k;

		glm::vec3 force;
		if (i < (num_points * 2) - 2) {
			glm::vec3 nextPos = state[i + 2];
			//float stretch2 = glm::length(nextPos - pos) - spring_rests[i / 2];
			float stretch2 = glm::length(nextPos - pos) - rest_length;
			glm::vec3 dir2 = glm::normalize(nextPos - pos);
			//float k2 = ks[i / 2];
			float k2 = k;

			float mass = 0.05f;
			force = (-k1 * stretch1 * dir1) + (k2 * stretch2 * dir2) + glm::vec3(0.0f, -9.8f*mass, 0.0f) - v * c;
			force /= mass;
		}
		else {
			// handle end of spring
			float x_force = 10.0f;
			float mass = 0.005f;
			if (caught_fish_idx >= 0) mass += 0.5f;

			float y_force = -9.8f * mass;
			//if (state[i].y <= -3.0f) y_force = 0.0f;
			force = (-k1 * stretch1 * dir1) + glm::vec3(0.0f, y_force, 0.0f) - v * c;
			if (caught_fish_idx >= 0) force += fishes[caught_fish_idx].strength * fishes[caught_fish_idx].target;
			force /= mass;

			//if (caught_fish_idx >= 0) {
			//	force += glm::vec3(0.0f, -9.8f * 0.5f, 0.0f);
			//	force /= mass + 0.5f;
			//}
			//else {
			//	force /= mass;
			//}
		}

		if (glm::abs(state[i].x) > box_boundary_x && state[i].y < box_boundary_y1) {
			v.x = 0;
			force.x = 0;
		}

		if (glm::abs(state[i].z) > box_boundary_x && state[i].y < box_boundary_y1) {
			v.z = 0;
			force.z = 0;
		}

		ret[i] = v;
		//if (state[i].y <= -3.0f) ret[i].y = 0;
		ret[i + 1] = force;
	}

	for (int i = num_points * 2; i < n_vars; i += 2) {
		glm::vec3 v = state[i + 1];
		glm::vec3 force;
		float c = 0.2f;

		fish current_fish = fishes[(i - (num_points * 2)) / 2];

		/*if ((i / 2) == caught_fish_idx) {
			v = state[i + 1];
			force = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		else {
			force = fishes[i / 2].strength * fishes[i / 2].target - c * v;
		}*/
		force = current_fish.strength * current_fish.target - c * v;

		ret[i] = v;
		ret[i + 1] = force;
	}

	return ret;
}

//glm::vec3* fish_update_f(float t, int n_vars, glm::vec3* state) {
//	glm::vec3* ret = new glm::vec3[n_vars];
//
//	//ret[0] = state[1];
//	//ret[1] = glm::vec3(0.0f, 0.0f, 0.0f);
//	//ret[1] = 0.1f * fishes[0].velocity - 0.2f * state[1];
//
//	return ret;
//}

int main() {
	//glm::vec3 state[] = {
	//	glm::vec3(0.0f, 10.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f)
	//};
	// TODO: add a worm - DONE
	// TODO: collision detection - DONE
	// TODO: fish sim
	// TODO: catching fish

	//glm::vec3 state[] = {
	//	// spring origin
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	// spring joint 1
	//	glm::vec3(0.0f, 1.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	// spring joint 2
	//	glm::vec3(0.0f, 2.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	// spring end
	//	glm::vec3(0.0f, 3.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 4.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 5.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 6.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f)
	//};

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

	glm::vec3 segment_length = glm::vec3(0.001f, 0.0f, 0.0f);
	const int total_num_vars = num_points * 2 + num_fish * 2;
	glm::vec3 state[total_num_vars];
	for (int i = 0; i < (num_points) * 2; i += 2) {
		state[i] = segment_length * ((float)i / 2);
		state[i + 1] = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	for (int i = num_points * 2; i < total_num_vars; i += 2) {
		// TODO: better start positions
		state[i] = glm::vec3(0.0f, -3.5f, 0.0f);
		state[i + 1] = random_vec3();
	}
	RK4Solver solver = RK4Solver(total_num_vars, state, update_f2, 0.0f);

	//glm::vec3 fish_state[num_fish * 2];
	//for (int i = 0; i < num_fish * 2; i += 2) {
	//	fish_state[i] = glm::vec3(0.0f, -3.5f, 0.0f);
	//	fish_state[i + 1] = random_vec3();
	//	//fish_state[i + 1] = glm::vec3(0.0f, 1.0f, 0.0f);
	//}
	//RK4Solver fish_solver = RK4Solver(num_fish * 2, fish_state, fish_update_f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	cameraVel.x = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS ? 5.0f : 0.0f;
	cameraVel.x = glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS ? -5.0f : cameraVel.x;
	cameraVel.z = glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS ? -5.0f : 0.0f;
	cameraVel.z = glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS ? 5.0f: cameraVel.z;
}

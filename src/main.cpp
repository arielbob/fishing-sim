#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "include/glew.h"
#include "include/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "shader.h"
#include "rk4solver.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

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
    -0.5f, -0.5f, -0.5f, 0.09f, 0.15f, 0.34f,
    0.5f, -0.5f, -0.5f, 0.09f, 0.15f, 0.34f,
    0.5f,  0.5f, -0.5f, 0.30f, 0.84f, 0.82f,
    0.5f,  0.5f, -0.5f, 0.30f, 0.84f, 0.82f,
    -0.5f,  0.5f, -0.5f, 0.30f, 0.84f, 0.82f,
    -0.5f, -0.5f, -0.5f, 0.09f, 0.15f, 0.34f,

    0.5f,  0.5f,  0.5f, 0.30f, 0.84f, 0.82f,
    0.5f,  0.5f, -0.5f, 0.30f, 0.84f, 0.82f,
    0.5f, -0.5f, -0.5f, 0.09f, 0.15f, 0.34f,
    0.5f, -0.5f, -0.5f, 0.09f, 0.15f, 0.34f,
    0.5f, -0.5f,  0.5f, 0.09f, 0.15f, 0.34f,
    0.5f,  0.5f,  0.5f, 0.30f, 0.84f, 0.82f,

    -0.5f, -0.5f, -0.5f, 0.09f, 0.15f, 0.34f,
    0.5f, -0.5f, -0.5f, 0.09f, 0.15f, 0.34f,
    0.5f, -0.5f,  0.5f, 0.09f, 0.15f, 0.34f,
    0.5f, -0.5f,  0.5f, 0.09f, 0.15f, 0.34f,
    -0.5f, -0.5f,  0.5f, 0.09f, 0.15f, 0.34f,
    -0.5f, -0.5f, -0.5f, 0.09f, 0.15f, 0.34f
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f)
};

glm::vec3 cameraPos = glm::vec3(-7.0f, 1.0f, 7.0f);
glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraVel = glm::vec3(0.0f, 0.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float pitch = 0.0f, yaw = -45.0f;
bool firstMouse = true;
float lastX = 400, lastY = 300;

int shader, lineShader, waterShader;
bool isCameraMoving = false;

float box_boundary_y1 = -2.0f;
float box_boundary_y2 = -4.0f;
float box_boundary_x = 5.0f;

const int num_points = 30;
const int num_fish = 100;
struct point {
    glm::vec3 position;
};
point points[num_points];

int caught_fish_idx = -1;
float last_catch_time;

struct fish {
    float last_update;
    glm::vec3 position;
    glm::vec3 target;
    float strength;
    float mass;
    glm::vec4 color;
};
fish fishes[num_fish];

glm::vec3 random_vec3() {
    float x = (float)rand() / RAND_MAX - 0.5f;
    float y = (float)rand() / RAND_MAX - 0.5f;
    float z = (float)rand() / RAND_MAX - 0.5f;

    return glm::normalize(glm::vec3(x, y, z));
}

void init_fish() {
    last_catch_time = (float) glfwGetTime();
    for (int i = 0; i < num_fish; i++) {
        fishes[i].last_update = (float) glfwGetTime();
        fishes[i].strength = 0.5f; //glm::max((float)rand() / RAND_MAX, 0.5f);
        fishes[i].color = glm::vec4(
            1.0f,
            glm::max(0.6f, (float)rand() / RAND_MAX),
            glm::max(0.1f, (float)rand() / RAND_MAX),
            1.0f
            );
        fishes[i].mass = glm::max((float)rand() / RAND_MAX, 0.1f);
        fishes[i].target = random_vec3();
    }
}

void init() {
    srand((unsigned int) time(NULL));

    unsigned int VBO;

    lineShader = Shader::createShader("../data/shaders/lineShader.vs", "../data/shaders/lineShader.fs");

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

    waterShader = Shader::createShader("../data/shaders/waterShader.vs", "../data/shaders/waterShader.fs");
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

glm::vec3 get_away_vec(int fish_i) {
    glm::vec3 accum = glm::vec3(0.0f, 0.0f, 0.0f);
    int num_close = 0;
    for (int j = 0; j < num_fish; j++) {
        if (j == fish_i) continue;
        glm::vec3 current_to_other = fishes[j].position - fishes[fish_i].position;
        float len = glm::length(current_to_other);
        // check if it's greater than some small number to prevent
        // an attempt to normalize zero vector (which is nan and messes things up)
        if (len < 0.3f) {
            // if they're too close, then point away
            //printf("too close\n");
            accum += -glm::normalize(current_to_other);
            num_close++;
        }
    }

    glm::vec3 new_target = glm::vec3(0.0f, 0.0f, 0.0f);
    if (num_close > 0) {
        new_target = glm::normalize(accum / (float)num_close);
    }

    if (!isnan(new_target.x)) return new_target;
    else return glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 get_orient_vec(int fish_i) {
    glm::vec3 accum = glm::vec3(0.0f, 0.0f, 0.0f);
    int num_close = 0;
    for (int j = 0; j < num_fish; j++) {
        if (j == fish_i) continue;
        glm::vec3 current_to_other = fishes[j].position - fishes[fish_i].position;
        float len = glm::length(current_to_other);
        // check if it's greater than some small number to prevent
        // an attempt to normalize zero vector (which is nan and messes things up)
        if (len < 0.5f) {
            accum += glm::normalize(fishes[j].target);
            num_close++;
        }
    }

    glm::vec3 new_target = glm::vec3(0.0f, 0.0f, 0.0f);
    if (num_close > 0) {
        new_target = glm::normalize(accum / (float)num_close);
    }

    if (!isnan(new_target.x)) return new_target;
    else return glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 get_cohesion_vec(int fish_i) {
    glm::vec3 accum = glm::vec3(0.0f, 0.0f, 0.0f);
    int num_close = 0;
    for (int j = 0; j < num_fish; j++) {
        if (j == fish_i) continue;
        glm::vec3 current_to_other = fishes[j].position - fishes[fish_i].position;
        float len = glm::length(current_to_other);
        // check if it's greater than some small number to prevent
        // an attempt to normalize zero vector (which is nan and messes things up)
        if (len < 1.0f) {
            // if they're too close, then point away
            //printf("too close\n");
            accum += fishes[j].position;
            num_close++;
        }
    }

    glm::vec3 new_target = glm::vec3(0.0f, 0.0f, 0.0f);
    if (num_close > 0) {
        glm::vec3 average_pos = accum / (float)num_close;
        new_target = glm::normalize(average_pos - fishes[fish_i].position);
    }

    if (!isnan(new_target.x)) return new_target;
    else return glm::vec3(0.0f, 0.0f, 0.0f);
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

    // update fish
    for (int i = num_points; i < num_points + num_fish; i++) {
        int fish_i = i - num_points;

        if (glm::abs(solver.state[2 * i].x) > box_boundary_x) {
            float neg = solver.state[2 * i].x / glm::abs(solver.state[2 * i].x);
            solver.state[2 * i].x = -neg * box_boundary_x;
        }
        if (glm::abs(solver.state[2 * i].z) > box_boundary_x) {
            float neg = solver.state[2 * i].z / glm::abs(solver.state[2 * i].z);
            solver.state[2 * i].z = -neg * box_boundary_x;
        }
        if (solver.state[2 * i].y < box_boundary_y2) {
            solver.state[2 * i].y = box_boundary_y2;
        }

        bool is_hook_occupied = caught_fish_idx >= 0;

        float current_time = (float) glfwGetTime();

        glm::vec3 hook_pos = points[num_points - 1].position;
        glm::vec3 pos = solver.state[2 * i];
        // check if enough time has passed since letting go of a fish, so that
        // when we let go of a fish, we don't automatically catch it again (we let it get away first)
        bool can_catch = current_time - last_catch_time > 1.0f;
        if (!is_hook_occupied && can_catch && glm::length(pos - hook_pos) < 0.1f) {
            // hook the fish if it's too close to the hook
            solver.state[2 * i] = hook_pos;
            caught_fish_idx = fish_i;
            fishes[caught_fish_idx].target = random_vec3();
            fishes[caught_fish_idx].last_update = current_time;
            fishes[caught_fish_idx].strength += 10.0f;
        }
        else {
            if (fish_i == caught_fish_idx) {
                // we make it seem like the fish is attached to the hook by just
                // setting the position of the caught fish to the position of the
                // hook position and the hook's position's movement just takes into
                // account the fish's force and weight when we solve for the hook's
                // position
                // in essence, when a fish is hooked, we no longer simulate that
                // individual fish, but instead act as if the hook is now that fish
                // and just sync the caught fish's position the hook
                solver.state[2 * i] = hook_pos;
                if (current_time - fishes[fish_i].last_update > 0.5f) {
                    fishes[fish_i].target = random_vec3();
                    fishes[fish_i].last_update = current_time;
                }
            }
            else {
                glm::vec3 food_target = glm::normalize(hook_pos - pos);

                glm::vec3 move_away = get_away_vec(fish_i);
                glm::vec3 orient_avg = get_orient_vec(fish_i);
                glm::vec3 cohesion_vec = get_cohesion_vec(fish_i);
                glm::vec3 weighted_total = glm::vec3(0.0f, 0.0f, 0.0f);

                float weights_sum = 0.0f;

                float food_target_weight = is_hook_occupied ? 0.1f : 0.5f;
                if (!glm::isnan(food_target.x)) {
                    weighted_total += food_target_weight * food_target;
                    weights_sum = food_target_weight;
                }

                weighted_total += 0.2f * fishes[fish_i].target;
                weights_sum += 0.2f;

                // NOTE: since at the bottom, you can't have a point beneath you,
                // all the fish tend to group at the bottom
                if (!glm::isnan(move_away.x)) {
                    weighted_total += 0.6f * move_away;
                    weights_sum += 0.6f;
                }

                if (!glm::isnan(orient_avg.x)) {
                    weighted_total += 0.4f * orient_avg;
                    weights_sum += 0.4f;
                }

                if (!glm::isnan(cohesion_vec.x)) {
                    weighted_total += 0.4f * cohesion_vec;
                    weights_sum += 0.4f;
                }
                //weighted_total += 0.5f * food_target;
                //printf("%f %f %f\n", weighted_total.x, weighted_total.y, weighted_total.z);
				
                fishes[fish_i].target = glm::normalize(weighted_total / weights_sum);

            }
            //else fishes[fish_i].target = -glm::normalize(hook_pos - pos);
        }

        //printf("fish %i: %f %f %f\n", fish_i, fishes[fish_i].position.x, fishes[fish_i].position.y, fishes[fish_i].position.z);
        //if (glm::isnan(fishes[fish_i].position.x)) exit(1);
        fishes[fish_i].position = solver.state[2 * i];
        //fishes[fish_i].target = solver.state[2 * i + 1];
    }
}

void render(RK4Solver solver) {
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    Shader::setMat4(shader, "view", view);

    glm::mat4 projection;
    projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    Shader::setMat4(shader, "projection", projection);
	
    glClearColor(0.82f, 0.71f, 0.60f, 1.0f);
    //glClearColor(0.64f, 0.51f, 0.45f, 1.0f);
    //glClearColor(0.90f, 0.29f, 0.42f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);

    // draw origin point
    Shader::use(lineShader);
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 rod_start = solver.state[0] + glm::vec3(1.0f, -2.0f, 1.0f);
    float rod_length = glm::length(solver.state[0] - rod_start);
    glm::vec3 dir = glm::normalize(solver.state[0] - rod_start);
    model = glm::mat4(1.0f);
    model = glm::translate(model, rod_start + (rod_length / 2.0f) * dir);
    glm::vec3 c = glm::cross(yAxis, dir);
    //printf("%f %f %f\n", c.x, c.y, c.z);
    float theta = glm::degrees(glm::acos(glm::dot(yAxis, dir)));
    model = glm::rotate(model, theta, c);
    model = glm::scale(model, glm::vec3(0.1f, rod_length, 0.1f));
    Shader::setMat4(lineShader, "model", model);
    Shader::setVec4(lineShader, "color", glm::vec4(0.53f, 0.35f, 0.20f, 1.0f));
    glBindVertexArray(linesVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    for (int i = 0; i < num_points - 1; i++) {
        model = glm::mat4(1.0f);
        glm::vec3 ab = points[i + 1].position - points[i].position;
        dir = glm::normalize(ab);
        float len =  glm::length(ab);

        model = glm::translate(model, points[i].position + (len / 2.0f) * dir);
        float cosAngle = glm::dot(yAxis, dir);
        float tol = 0.000001f;
        if (glm::abs(glm::abs(cosAngle) - 1.0f) > tol) {
            c = glm::cross(yAxis, dir);
            //printf("%f %f %f\n", c.x, c.y, c.z);
            theta = glm::acos(glm::dot(yAxis, dir));

            model = glm::rotate(model, theta, c);
        }
        else {
            if (cosAngle < 0.0f) glm::rotate(model, 180.0f, zAxis);
        }

        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        if (i > num_points - 10) {
            // draw worm
            if (i > num_points - 6 && i < num_points - 3) {
                color = glm::vec4(0.82f, 0.52f, 0.44f, 1.0f);
                model = glm::scale(model, glm::vec3(0.08f, len, 0.05f));
            }
            else {
                color = glm::vec4(1.0f, 0.70f, 0.58f, 1.0f);
                model = glm::scale(model, glm::vec3(0.05f, len, 0.05f));
            }
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
        dir = glm::normalize(fish_velocity);
        if (caught_fish_idx == i) dir = glm::normalize(fishes[i].target);
        //glm::vec3 dir = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f));
        float cosAngle = glm::dot(xAxis, dir);
        float tol = 0.000001f;
        if (glm::abs(glm::abs(cosAngle) - 1.0f) > tol) {
            c = glm::cross(xAxis, dir);
            //printf("%f %f %f\n", c.x, c.y, c.z);
            theta = glm::acos(glm::dot(xAxis, dir));

            model = glm::rotate(model, theta, c);
        }
        else {
            if (cosAngle < 0) model = glm::rotate(model, 180.0f, yAxis);
        }

        //model = glm::rotate(model, 10.0f * (float)sin(glfwGetTime()*0.4f*glm::length(fish_velocity) + fishes[i].color.x*100), yAxis);
        model = glm::scale(model, glm::vec3(0.3f, fishes[i].mass * 0.2, fishes[i].mass * 0.2));

        Shader::setMat4(lineShader, "model", model);
        Shader::setVec4(lineShader, "color", fishes[i].color);

        glBindVertexArray(linesVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // draw water box
    Shader::use(waterShader);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f, 2.0f, 10.0f));
    Shader::setMat4(waterShader, "model", model);
    Shader::setMat4(waterShader, "view", view);
    Shader::setMat4(waterShader, "projection", projection);
    glBindVertexArray(water_vao);
    glDrawArrays(GL_TRIANGLES, 0, 18);
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

    /*for (int i = 0; i < num_points; i++) {
      printf("point pos %i: %f\n", i, state[i * 2]);
      }
      printf("\n");*/

    // generate the derivatives for the joints
    for (int i = 2; i < (num_points) * 2; i += 2) {
        glm::vec3 v = state[i + 1];
        glm::vec3 pos = state[i];

        glm::vec3 prevPos = state[i - 2];
        //float stretch1 = glm::length(pos - prevPos) - spring_rests[i / 2 - 1];
        float stretch1 = glm::length(pos - prevPos) - rest_length;
        glm::vec3 dir1 = glm::normalize(pos - prevPos);
        //float k1 = ks[i / 2 - 1];
        float k1 = k;

        glm::vec3 force;
        if (i < (num_points - 1) * 2) {
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
            if (caught_fish_idx >= 0) mass += fishes[caught_fish_idx].mass;

            float y_force = -9.8f * mass;
            //if (state[i].y <= -3.0f) y_force = 0.0f;
            force = (-k1 * stretch1 * dir1) + glm::vec3(0.0f, y_force, 0.0f) - v * c;
            if (caught_fish_idx >= 0 && state[i].y < box_boundary_y1) {
                // only let the fish exert force if it's outside of water
                // (since they can't move in air)
                force += fishes[caught_fish_idx].strength * fishes[caught_fish_idx].target;
                //if (state[i].y > box_boundary_y1) force += current_fish.mass * glm::vec3(0.0f, -9.8f, 0.0f);
            }
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
        c = 0.2f;

        fish current_fish = fishes[(i - (num_points * 2)) / 2];

        /*if ((i / 2) == caught_fish_idx) {
          v = state[i + 1];
          force = glm::vec3(0.0f, 0.0f, 0.0f);
          }
          else {
          force = fishes[i / 2].strength * fishes[i / 2].target - c * v;
          }*/
        if (state[i].y < box_boundary_y1) {
            force = current_fish.strength * current_fish.target - c * v;
        }
        else {
            force = glm::vec3(0.0f, -9.8f * current_fish.mass, 0.0f);
        }

        //force = current_fish.strength * current_fish.target - c * v;

        if (caught_fish_idx == (i - (num_points * 2)) / 2) {
            v = glm::vec3(0.0f, 0.0f, 0.0f);
            force = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        ret[i] = v;
        ret[i + 1] = force;
    }

    return ret;
}

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Fishing Simulation", NULL, NULL);
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
        //state[i + 1] = random_vec3();
        state[i + 1] = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    RK4Solver solver = RK4Solver(total_num_vars, state, update_f2, 0.0f);

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
        lastX = (float) xpos;
        lastY = (float) ypos;
        firstMouse = false;
    }

    float xoffset = (float) xpos - lastX;
    float yoffset = lastY -  (float) ypos;

    lastX = (float) xpos;
    lastY = (float) ypos;

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

    float currentFrame = (float) glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    cameraVel.x = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? 5.0f : 0.0f;
    cameraVel.x = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? -5.0f : cameraVel.x;
    cameraVel.z = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? -5.0f : 0.0f;
    cameraVel.z = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? 5.0f : cameraVel.z;
    cameraVel.y = glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS ? 5.0f : 0.0f;
    cameraVel.y = glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS ? -5.0f : cameraVel.y;

    if (caught_fish_idx >= 0 && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        last_catch_time = currentFrame;
        fishes[caught_fish_idx].target = random_vec3();
        fishes[caught_fish_idx].last_update = currentFrame;
        fishes[caught_fish_idx].strength -= 10.0f;
        caught_fish_idx = -1;
    }
}

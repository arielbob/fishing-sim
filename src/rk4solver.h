#ifndef RK4_SOLVER_H
#define RK4_SOLVER_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

typedef glm::vec3* (*update_func) (float t, int n_vars, glm::vec3* state);

class RK4Solver {
public:
    int n_vars;
    glm::vec3* state;
    update_func update_f;
    float t;

    RK4Solver(int n_vars, glm::vec3* state, update_func update_f, float initial_t) {
        this->n_vars = n_vars;
        this->state = state;
        this->update_f = update_f;
        this->t = initial_t;
    }

    void update(float dt) {
        glm::vec3* k1;
        glm::vec3* k2;
        glm::vec3* k3;
        glm::vec3* k4;

        glm::vec3* s1;
        glm::vec3* s2;
        glm::vec3* s3;

        float half_t = t + 0.5f * dt;

        k1 = update_f(t, n_vars, state);

        s1 = new glm::vec3[n_vars];
        for (int i = 0; i < n_vars; i++) {
            s1[i] = state[i] + (dt/2) * k1[i];
        }
        k2 = update_f(t + 0.5f * dt, n_vars, s1);

        s2 = new glm::vec3[n_vars];
        for (int i = 0; i < n_vars; i++) {
            s2[i] = state[i] + (dt/2) * k2[i];
        }
        k3 = update_f(t + 0.5f * dt, n_vars, s2);

        s3 = new glm::vec3[n_vars];
        for (int i = 0; i < n_vars; i++) {
            s3[i] = state[i] + dt * k3[i];
        }
        k4 = update_f(t + 0.5f * dt, n_vars, s3);

        for (int i = 0; i < n_vars; i++) {
            state[i] += (dt / 6) * (k1[i] + 2.0f * k2[i] + 2.0f * k3[i] + k4[i]);
        }
        t += dt;

        delete[] k1;
        delete[] k2;
        delete[] k3;
        delete[] k4;
        delete[] s1;
        delete[] s2;
        delete[] s3;
    }
};

#endif

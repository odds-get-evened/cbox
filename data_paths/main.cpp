#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define _USE_MATH_DEFINES  // for some reason CONSTANTS won't pull in if this is not declared
#include <cmath>
#include <zlib.h>
#include <cstdint>
#include <random>
#include <vector>
#include <mlpack/core.hpp>
#include <mlpack/methods/linear_regression/linear_regression.hpp>


using namespace std;
using namespace mlpack;

void drawCircle(float centerX, float centerY, float radius) {
    glColor3f(1, 0, 0);

    glBegin(GL_POLYGON);
    for(int i=0; i<360; i++) {
        float angle = i * M_PI / 180.0f;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);

        glVertex2f(x, y);
    }
    glEnd();
}

namespace odds {
    // define type for matrix of vectors
    // using MatrixVec = vector<vector<double>>;
    typedef vector<vector<double>> MatrixVec;

    MatrixVec gen_rand_matrix(size_t len) {
        // seed random number generator
        random_device rd;
        mt19937 gen(rd());
        // pick random numbers between -1 and 1
        uniform_real_distribution<double> dist(-1.0, 1.0);

        MatrixVec vec(len, vector<double>(3));

        // fill matrix with random numbers
        for(size_t i=0; i<len; i++) {
            for(size_t j=0; j<3; j++) {
                vec[i][j] = dist(gen);
            }
        }

        return vec;
    }

    vector<uint8_t> gen_rand_bytes(size_t len) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<uint8_t> dist(0, 255);

        vector<uint8_t> arr(len);
        for(size_t i=0; i<len; i++) {
            arr[i] = dist(gen);
        }

        return arr;
    }

    uLong crc32_hash(const vector<uint8_t> &d) {
        uLong c = crc32(0L, Z_NULL, 0); // initialize crc32
        c = crc32(c, d.data(), d.size());

        return c;
    }

    LinearRegression train_model(const vector<vector<double>> &d) {
        // create matrix from data
        arma::mat X(d.size(), 2);
        // create vector from data
        arma::rowvec y(d.size());

        // fill matrix and vector
        for(size_t i=0; i<d.size(); i++) {
            X(i, 0) = d[i][0];
            X(i, 1) = d[i][1];
            y(i) = d[i][2];
        }

        LinearRegression lr(X, y);

        return lr;
    }

    vector<vector<double>> predict_new_matrix(LinearRegression &lr, size_t len) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dist(-1.0, 1.0);

        vector<vector<double>> new_mtx(len, vector<double>(3));

        // fill matrix with random numbers
        arma::mat X(len, 2);

        for(size_t i=0; i<len; i++) {
            new_mtx[i][0] = dist(gen);
            new_mtx[i][1] = dist(gen);
            X(i, 0) = new_mtx[i][0];
            X(i, 1) = new_mtx[i][1];
        }

        arma::rowvec predictions;
        lr.Predict(X, predictions);

        for(size_t i=0; i<len; i++) {
            new_mtx[i][2] = predictions(i);
        }

        return new_mtx;
    }

    void print_matrix(const vector<vector<double>> &mtx) {
        for(const auto &vec : mtx) {
            for(const auto &v : vec) {
                cout << setw(10) << v << " ";
            }
            
            cout << endl;
        }
    }

    class MatrixPlot {
        private:
        MatrixVec matrix;
        GLFWwindow *window;
        float angleX, angleY;
        float lastX, lastY;
        bool firstMouse;

        static void mouse_cb(GLFWwindow *window, double xpos, double ypos) {
            // get pointer to MatrixPlot object
            MatrixPlot *plot = static_cast<MatrixPlot *>(glfwGetWindowUserPointer(window));

            // check if left mouse button is pressed
            if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                // check if first mouse movement
                if(plot->firstMouse) {
                    plot->lastX = xpos;
                    plot->lastY = ypos;
                    plot->firstMouse = false;
                }

                float xoffset = xpos - plot->lastX;
                // float yoffset = plot->lastY - ypos;
                float yoffset = ypos - plot->lastY;
                plot->lastX = xpos;
                plot->lastY = ypos;

                plot->angleX += yoffset * 0.1f;
                plot->angleY += xoffset * 0.1f;
            } else {
                plot->firstMouse = true;        // reset firstMouse when button is released
            }
        }
        public:
        MatrixPlot(const MatrixVec &d) : matrix(d), window(nullptr), angleX(0.0f), angleY(0.0f), lastX(0.0f), lastY(0.0f), firstMouse(true) {}

        void init() {
            if(!glfwInit()) {
                cerr << "failed to initialize GLFW" << endl;
                exit(-1);
            }

            // set window non-resizeable
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            // create window
            window = glfwCreateWindow(640, 480, "matrix plot", nullptr, nullptr);
            if(!window) {
                cerr << "failed to create window" << endl;
                glfwTerminate();
                exit(-1);
            }

            glfwMakeContextCurrent(window);
            glfwSetWindowUserPointer(window, this);
            glfwSetCursorPosCallback(window, this->mouse_cb);

            // initialize GLEW
            if(glewInit() != GLEW_OK) {
                cerr << "failed to initialize GLEW" << endl;
                exit(-1);
            }

            glEnable(GL_DEPTH_TEST);

            glViewport(0, 0, 640, 480);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
        }

        void run() {
            this->init();

            while(!glfwWindowShouldClose(window)) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glLoadIdentity();
                glTranslatef(0.0f, 0.0f, -1.0f);
                glRotatef(angleX, 1.0f, 0.0f, 0.0f);
                glRotatef(angleY, 0.0f, 1.0f, 0.0f);

                glPointSize(5.0f);
                glColor3f(1.0f, 1.0f, 1.0f);

                glBegin(GL_POINTS);
                for(const auto &vec : matrix) {
                    glVertex3d(vec[0], vec[1], vec[2]);
                }
                glEnd();

                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            glfwDestroyWindow(window);
            glfwTerminate();
        }
    };
}



int main() {
    odds::MatrixVec mtx = odds::gen_rand_matrix(32);
    odds::MatrixPlot plt(mtx);
    odds::print_matrix(mtx);
    plt.run();

    return 0;
}
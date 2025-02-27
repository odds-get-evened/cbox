#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define _USE_MATH_DEFINES  // for some reason CONSTANTS won't pull in if this is not declared
#include <cmath>
#include <zlib.h>
#include <cstdint>
#include <random>
#include <vector>
#include <string>
#include <sstream>
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

const size_t MATRIX_SIZE = 32;

namespace odds {
    string uLong_to_hex(unsigned long num) {
        stringstream ss;
        ss << hex << num;

        return ss.str();
    }

    // define type for matrix of vectors
    // using MatrixVec = vector<vector<double>>;
    typedef vector<vector<double>> MatrixVec;

    bool is_col_len_eq(const vector<vector<double>> &mtx) {
        // 0 is equal to 0
        if(mtx.empty()) return true;

        // establish forst row length
        size_t col_len = mtx[0].size();

        for(const auto &row : mtx) {
            if(row.size() != col_len) return false;
        }

        return true;
    }

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

    vector<uint8_t> double_to_uint8_vec(vector<double> &d) {
        vector<uint8_t> u8v(d.size() * sizeof(double));

        memcpy(u8v.data(), d.data(), u8v.size());

        return u8v;
    }

    LinearRegression train_model(const vector<vector<double>> &d) {
        /*
        ensures all inner vectors lengths are equal
        so we can select anyone for column length in
        training.
        */
        bool eq_cols = is_col_len_eq(d);
        if(!eq_cols) exit(-1);

        // since all are equal just grab column length from first row.
        size_t n_cols = d[0].size();
        // create matrix from data
        arma::mat X(d.size(), n_cols);
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

    /**
     * @brief Predict new values using a trained linear regression model
     * @param lr LinearRegression object
     * @param len Number of new values to predict
     */
    vector<vector<double>> predict_new_matrix(LinearRegression &lr, size_t len) {
        // seed random number generator
        random_device rd;
        mt19937 gen(rd());
        // pick random numbers between -1 and 1
        uniform_real_distribution<double> dist(-1.0, 1.0);

        // create matrix to store new data
        vector<vector<double>> new_mtx(len, vector<double>(3));

        // fill matrix with random numbers
        arma::mat X(len, 2);
        for(size_t i=0; i<len; i++) {
            new_mtx[i][0] = dist(gen);
            new_mtx[i][1] = dist(gen);
            X(i, 0) = new_mtx[i][0];
            X(i, 1) = new_mtx[i][1];
        }

        // predict new values
        arma::rowvec predictions;
        lr.Predict(X, predictions);

        // fill new matrix with predictions
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

                const float dr = 0.2f;
                plot->angleX += yoffset * dr;
                plot->angleY += xoffset * dr;
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
            // glEnable(GL_COLOR_MATERIAL);

            glViewport(0, 0, 640, 480);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
        }

        void draw_plots() {
            glPointSize(10.0f);

            glBegin(GL_POINTS);
            glColor3f(0.86f, 0.1f, 0.07f);
            for(const auto &vec : matrix) {
                glVertex3d(vec[0], vec[1], vec[2]);
            }
            glEnd();
        }

        void draw_line_arrow(float x1, float y1, float x2, float y2) {
            float arrow_len = 0.1f;
            // angle of the arrowhead
            float arrow_angle = M_PI / 6;

            float angle = atan2(y2 - y1, x2 - x1);

            float x3 = x2 - arrow_len * cos(angle - arrow_angle);
            float y3 = y2 - arrow_len * sin(angle - arrow_angle);

            float x4 = x2 - arrow_len * cos(angle + arrow_angle);
            float y4 = y2 - arrow_len * sin(angle + arrow_angle);

            glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glEnd();

            glBegin(GL_TRIANGLES);
            glVertex2f(x2, y2);
            glVertex2f(x3, y3);
            glVertex2f(x4, y4);
            glEnd();
        }

        void draw_direction_lines() {
            glLineWidth(1.0);
            
            for(size_t i=0; i<matrix.size(); i++) {
                float x1 = matrix[i][0];
                float y1 = matrix[i][1];
                float z1 = matrix[i][2];
                // cout << x1 << ", " << y1 << ", " << z1 << endl;

                float x2, y2, z2;
                // avoid segfault
                if(i+1<matrix.size()) {
                    x2 = matrix[i+1][0];
                    y2 = matrix[i+1][1];
                    z2 = matrix[i+1][2];
                }

                // draw the line
                glBegin(GL_LINES);
                glColor3f(0.08f, 0.86f, 0.07f);
                glVertex3f(x1, y1, z1);
                glVertex3f(x2, y2, z2);
                glEnd();

                // draw the directional arrow
                float midX = (x1 + x2) / 2.0f;
                float midY = (y1 + y2) / 2.0f;
                float midZ = (z1 + z2) / 2.0f;
                draw_line_arrow(midX, midY, x2, y2);
            }
        }

        void run() {
            this->init();

            while(!glfwWindowShouldClose(window)) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glLoadIdentity();
                glTranslatef(0.0f, 0.0f, -1.0f);
                glRotatef(angleX, 1.0f, 0.0f, 0.0f);
                glRotatef(angleY, 0.0f, 1.0f, 0.0f);

                draw_plots();
                draw_direction_lines();

                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            glfwDestroyWindow(window);
            glfwTerminate();
        }
    };
}

int main() {
    odds::MatrixVec mtx = odds::gen_rand_matrix(MATRIX_SIZE);

    odds::MatrixPlot plt(mtx);
    // odds::print_matrix(mtx);
    plt.run();

    /*
    vector<uint8_t> umtx = odds::double_to_uint8_vec(mtx.at(0));
    uLong ch = odds::crc32_hash(umtx);
    string chh = odds::uLong_to_hex(ch);
    cout << chh << endl;
    */

    return 0;
}
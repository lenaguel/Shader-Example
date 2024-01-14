#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

static unsigned int Shader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();

    // Read vertex shader code from the file
    std::string vs_code;
    std::ifstream VertexShaderStream(vertexShader, std::ios::in);
    std::string line = "";
    while (getline(VertexShaderStream, line))
        vs_code += "\n" + line;
    VertexShaderStream.close();

    // Read fragment shader code from the file
    std::string fs_code;
    std::ifstream FragmentShaderStream(fragmentShader, std::ios::in);
    while (getline(FragmentShaderStream, line))
        fs_code += "\n" + line;
    FragmentShaderStream.close();

    // Create and compile vertex shader
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    const char* vs_src = vs_code.c_str();
    glShaderSource(vs, 1, &vs_src, nullptr);
    glCompileShader(vs);

    // Create and compile fragment shader
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fs_src = fs_code.c_str();
    glShaderSource(fs, 1, &fs_src, nullptr);
    glCompileShader(fs);

    // Link program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1200, 900, "Shader Example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    // Triangle vertices
    const float vertices[6]{
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5
    };

    // Triangle colors
    const float color[] = {
        0.686f,  0.0f,    1.0f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
    };

    // Create vao
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create color buffer
    unsigned int colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    // Create vertex buffer
    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

    // Load shaders
    unsigned int shader = Shader("VertexShader.vert", "FragmentShader.frag");

    const unsigned int positionLocation = glGetAttribLocation(shader, "position");
    glEnableVertexAttribArray(positionLocation);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    const unsigned int colorLocation = glGetAttribLocation(shader, "vertexColor");
    glEnableVertexAttribArray(colorLocation);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(colorLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glUseProgram(shader);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
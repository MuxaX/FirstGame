#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern "C" { // Чтобы выбиралась не "Intel(R) HD Graphics", а "NVidia GeForce" (или какая там у вас видеокарта)
    _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    // Для NVidia
       // __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; // Это для AMD, но не проверял.
}

GLfloat point[] = {
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f

};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f
};

const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);"
"}";

int g_WindowsSizeX = 640;
int g_WindowsSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* ptr_Window, int height, int width)
{
    g_WindowsSizeX = width;
    g_WindowsSizeY = height;
    glViewport(0, 0, g_WindowsSizeX, g_WindowsSizeY);
}

void glfwKeyCallBack(GLFWwindow* ptr_window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(ptr_window, GL_TRUE);
    }
}

int main(void)
{

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit Error " << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* ptr_window = glfwCreateWindow(g_WindowsSizeX, g_WindowsSizeY, "First Game", nullptr, nullptr);
    if (!ptr_window)
    {
        std::cout << "glfwCreateWindow failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(ptr_window, glfwWindowSizeCallback);
    glfwSetKeyCallback(ptr_window, glfwKeyCallBack);

    /* Make the window's context current */
    glfwMakeContextCurrent(ptr_window);
	
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    std::cout << "Renderee: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	
	glClearColor(0, 1, 1, 1);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(ptr_window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 1, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(ptr_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
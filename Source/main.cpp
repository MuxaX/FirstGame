#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern "C" { // Чтобы выбиралась не "Intel(R) HD Graphics", а "NVidia GeForce" (или какая там у вас видеокарта)
    _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    // Для NVidia
       // __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; // Это для AMD, но не проверял.
}

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
	
	glClearColor(0, 1, 0, 1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(ptr_window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(ptr_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
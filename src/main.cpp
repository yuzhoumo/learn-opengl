#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

/* glfw callback function, resize window */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/* process inputs */
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {
  std::cout << "Hello world!" << std::endl;

  glfwInit(); // initialize windowing api

  // tell glfw to use opengl 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // use core file (drop backwards-compatible features)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
    // osx compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  // create empty window
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // initialize glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // set viewport
  glViewport(0, 0, 800, 600);

  // register resize window callback
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


  // SHADERS
  float vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f
  };

  unsigned int VBO;
  glGenBuffers(1, &VBO); // create 1 buffer with ID stored in VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // set buffer type
  /* copy data to buffer:
   * GL_STREAM_DRAW, GL_STATIC_DRAW,GL_DYNAMIC_DRAW specifies usage pattern
   * use GL_STATIC_DRAW since we are drawing a static triangle that does
   * not change between frames */
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER); // create shader obj


  // set shader source code
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

  // compile shader
  glCompileShader(vertexShader);

  // check if shader compiled successfully
  int  success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // main rendering loop
  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    // clear buffer between frames
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    /* double buffer: front and back buffer, prevent flickering by
     * swapping front buffer with back buffer as soon as finished
     * rendering to back buffer. */
    glfwSwapBuffers(window); // push new frame to screen
    glfwPollEvents(); // e.g. keyboard/mouse input
  }

  // clean glfw resources
  glfwTerminate();
  return 0;
}

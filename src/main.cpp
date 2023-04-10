#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

  glfwInit();

  // tell glfw to use opengl 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // use core file (drop backwards-compatible features)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // osx compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

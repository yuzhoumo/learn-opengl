#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";

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

  // compile fragment shader
  unsigned int fragmentShader, fragmentShader2;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader2);

  unsigned int shaderProgram, shaderProgram2;
  shaderProgram = glCreateProgram(); // initialize shader program
  shaderProgram2 = glCreateProgram();

  // link shaders
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShader2);
  glLinkProgram(shaderProgram2);

  glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // delete shader objects after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(fragmentShader2);

  // BUFFERS
  float t1[] = {
  -1.0f, -0.5f, 0.0f,
   0.0f, -0.5f, 0.0f,
  -0.5f,  0.5f, 0.0f,
  };

  float t2[] = {
   1.0f, -0.5f, 0.0f,
   0.0f, -0.5f, 0.0f,
   0.5f,  0.5f, 0.0f 
  };

  unsigned int VAO[2], VBO[2];

  glGenVertexArrays(2, VAO); 
  glGenBuffers(2, VBO); // create 2 buffer with ID stored in VBO

  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // set buffer type
  glBufferData(GL_ARRAY_BUFFER, sizeof(t1), t1, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // set buffer type

  /* copy data to buffer:
   * GL_STREAM_DRAW, GL_STATIC_DRAW,GL_DYNAMIC_DRAW specifies usage pattern
   * use GL_STATIC_DRAW since we are drawing a static triangle that does
   * not change between frames */
  glBufferData(GL_ARRAY_BUFFER, sizeof(t2), t2, GL_STATIC_DRAW);

  /* tell opengl how to interpret vertex data:
   * 0: configure vertex attribute for location 0
   * 3: specify size of vertex attribute (vec3, so size 3)
   * GL_FLOAT: data type
   * GL_FALSE: do not normalize (only relevant for ints, normalizes to between 0 and 1)
   * 3 * sizeof(float): stride, space between vertex attributes
   * (void*)0: offset of where pos data begins in the buffer
   */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // main rendering loop
  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    // clear buffer between frames
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram); // use the compiled shader program

    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shaderProgram2); // use the compiled shader program
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

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

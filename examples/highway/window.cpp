#include "window.hpp"

#include <unordered_map>

#include <glm/gtx/fast_trigonometry.hpp>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 1.0f;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }
}

void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  m_velocity = 1000.0f;

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "lookat.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "lookat.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_ground.create(m_program);

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");

  // Load wheels
  m_wheel.loadObj(assetsPath + "wheel.obj");
  m_wheel.setupVAO(m_program);

  // Load car
  m_car.loadObj(assetsPath + "car.obj");
  m_car.setupVAO(m_program);
}

void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  abcg::glBindVertexArray(m_VAO);

  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  m_angle = glm::wrapAngle(m_angle + glm::radians(m_velocity * deltaTime));

  // Draw 4 wheels
  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(-0.3f, 0.1f, 0.25f));
  model = glm::rotate(model, m_angle, glm::vec3(0, 0, -1));
  model = glm::scale(model, glm::vec3(0.1f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.5f, 0.5f, 0.5f, 1.5f);
  m_wheel.render();

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.3f, 0.1f, 0.25f));
  model = glm::rotate(model, m_angle, glm::vec3(0, 0, -1));
  model = glm::scale(model, glm::vec3(0.1f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.5f, 0.5f, 0.5f, 1.5f);
  m_wheel.render();

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-0.3f, 0.1f, -0.25f));
  model = glm::rotate(model, m_angle, glm::vec3(0, 0, -1));
  model = glm::scale(model, glm::vec3(0.1f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.5f, 0.5f, 0.5f, 1.5f);
  m_wheel.render();

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.3f, 0.1f, -0.25f));
  model = glm::rotate(model, m_angle, glm::vec3(0, 0, -1));
  model = glm::scale(model, glm::vec3(0.1f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.5f, 0.5f, 0.5f, 1.5f);
  m_wheel.render();

  // Draw red car
  model = glm::mat4(1.0);
  model = glm::scale(model, glm::vec3(0.5f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 0.25f, 0.25f, 1.0f);
  m_car.render();

  // Draw ground
  m_ground.paint();

  abcg::glUseProgram(0);
}

void Window::onPaintUI() { 
  abcg::OpenGLWindow::onPaintUI(); 

  {
    ImGui::SetNextWindowPos(ImVec2(5, m_viewportSize.y - 94));
    ImGui::SetNextWindowSize(ImVec2(m_viewportSize.x - 10, -1));
    ImGui::Begin("Slider window", nullptr, ImGuiWindowFlags_NoDecoration);

    // Create a slider to control the velocity of the wheels
    {
      // Slider will fill the space of the window
      ImGui::PushItemWidth(m_viewportSize.x - 25);

      static auto n{m_velocity};
      ImGui::SliderFloat(" ", &n, 0.0f, 1000.0f , "Velocidade");
      m_velocity = n ;

      ImGui::PopItemWidth();
    }

    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_ground.destroy();

  m_wheel.destroy();
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}
#include "rockets.hpp"

#include <glm/gtx/fast_trigonometry.hpp>

void Rockets::create(GLuint program, int quantity) {
  destroy();


  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create foguetes
  m_rockets.clear();
  m_rockets.resize(quantity);

  for (auto &rocket : m_rockets) {
    rocket = makeRocket();

    // Cria foguetes em posiçoes aleatorias na direita da tela
    
      std::random_device rd;
      std::default_random_engine eng(rd());
      std::uniform_real_distribution<> numeroAleatorioX(1.2f, 7.5f);
      std::uniform_real_distribution<> numeroAleatorioY(-1.0f, 1.0f);
      rocket.m_translation.x = numeroAleatorioX(eng),
      rocket.m_translation.y = numeroAleatorioY(eng);
    
  }
}

void Rockets::paint() {
  abcg::glUseProgram(m_program);

  for (auto const &rocket : m_rockets) {
    abcg::glBindVertexArray(rocket.m_VAO);

    abcg::glUniform4fv(m_colorLoc, 1, &rocket.m_color.r);
    abcg::glUniform1f(m_scaleLoc, rocket.m_scale);
    abcg::glUniform1f(m_rotationLoc, rocket.m_rotation);

    
        abcg::glUniform2f(m_translationLoc, rocket.m_translation.x,
                          rocket.m_translation.y);

        abcg::glDrawElements(GL_TRIANGLES, 10 * 3, GL_UNSIGNED_INT, nullptr);
      

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Rockets::destroy() {
  for (auto &rocket : m_rockets) {
    abcg::glDeleteBuffers(1, &rocket.m_VBO);
    abcg::glDeleteVertexArrays(1, &rocket.m_VAO);
    abcg::glDeleteBuffers(1, &rocket.m_EBO);
  }
}

void Rockets::update(const Ship &ship, float deltaTime) {
  for (auto &rocket : m_rockets) {
    rocket.m_translation -= ship.m_velocity * deltaTime;

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> numeroAleatorio(-1.0f, 1.0f);

    // Wrap-around
    if (rocket.m_translation.x < -1.3f){
         rocket.m_translation.x += 2.5f;
         rocket.m_translation.y = numeroAleatorio(eng);
    }

  }
}

Rockets::Rocket Rockets::makeRocket(glm::vec2 translation,
                                            float scale) {
  Rocket rocket;

  auto &re{m_randomEngine}; // Shortcut


  // Get a random color (actually, a grayscale)
  std::uniform_real_distribution randomIntensity(0.5f, 1.0f);
  rocket.m_color = glm::vec4(randomIntensity(re));

  rocket.m_color.a = 1.0f;
  rocket.m_rotation = 1.57f;;
  rocket.m_scale = scale;
  rocket.m_translation = translation;


  // Get a random direction
  glm::vec2 const direction{m_randomDist(re), m_randomDist(re)};
  rocket.m_velocity = glm::normalize(direction) / 7.0f;

  // Create geometry data
  std::array positions{
      // Rocket body
      glm::vec2{-02.5f, +12.5f}, glm::vec2{-4.5f,+02.5f},
      glm::vec2{-15.5f,-10.5f}, glm::vec2{-09.5f,-05.5f},
      glm::vec2{-02.5f,-12.5f}, glm::vec2{+02.5f,-12.5f},
      glm::vec2{+09.5f,-05.5f}, glm::vec2{+15.5f,-10.5f},
      glm::vec2{+4.5f,+02.5f}, glm::vec2{+02.5f,+12.5f},

      };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  std::array const indices{0, 1, 9,
                           1, 9, 8,
                           1, 3, 6,
                           1, 8, 6,
                           2, 3, 7,
                           2, 7, 6,
                           0, 4, 5,
                           1, 8, 4,
                           1, 8, 5,
                           3, 6, 7};
  // clang-format on

  // Generate VBO
  abcg::glGenBuffers(1, &rocket.m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, rocket.m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &rocket.m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rocket.m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &rocket.m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(rocket.m_VAO);
  
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, rocket.m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rocket.m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return rocket;
}
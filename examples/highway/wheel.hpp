#ifndef WHEEL_HPP_
#define WHEEL_HPP_

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Wheel {
public:
  void loadObj(std::string_view path);
  void render() const;
  void setupVAO(GLuint program);
  void destroy() const;

  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void createBuffers();
  void standardize();
};

#endif
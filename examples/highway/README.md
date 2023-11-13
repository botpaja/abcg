Nome: Claudio Yaitiro Sakamoto Mantovani RA:11116013

O projeto Highway consiste em uma aplicação que mostra o modelo 3D de um carro vermelho numa estrada. As rodas do carro se movimentam de acordo com um slider na tela, e é possivel movimentar a camera com as teclas AWSDQE.

Projeto
O projeto foi uma adaptação do projeto lookat e viewer. Foram modificadas as classes Window, Ground, Model.

Model
No metodo loadObj foi retirada a opção de normalizar e centralizar o objeto, além de também ser retirado o metodo que faz essa função.

Window
No metodo onCreate() foi adicionada a chamada do metodos metodos que carregam os arquivos dos objetos 3d e também são inicializados seus respectivos VAOs/VBOs

  // Load wheels
  m_wheel.loadObj(assetsPath + "wheel.obj");
  m_wheel.setupVAO(m_program);

  // Load car
  m_car.loadObj(assetsPath + "car.obj");
  m_car.setupVAO(m_program);

  No metodo onPaint() foi adicionada a variavel m_angle, que será usada para definir a rotação das 4 rodas.
  
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  m_angle = glm::wrapAngle(m_angle + glm::radians(m_velocity * deltaTime));

  O valor dessa variavel varia de acordo com a variavel m_velocity multiplicada pelo tempo.

  
  A seguir, foi adicionada a renderização das 4 rodas.

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

  Primeiro é criada a concatenação de transformações que forma a matriz de modelo de cada roda, para posiciona-las nos locais apropriados. Note que a variavel m_angle é utilizada aqui para definir a rotação das rodas no eixo Z. Depois a matriz de modelo é enviada à variável uniforme m_modelMatrix no vertex shader e a cor é definida. Por ultimo é chamado o metodo m_wheel.render() para realizar o comando de renderização.

  O mesmo processo também é feito para renderizar o carro.

    // Draw red car
  model = glm::mat4(1.0);
  model = glm::scale(model, glm::vec3(0.5f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 0.25f, 0.25f, 1.0f);
  m_car.render();

No metodo onPaintUI() é definido o slider que controlará a velocidade de rotação das rodas.

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
  

  

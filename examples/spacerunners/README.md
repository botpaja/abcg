
Nome: Claudio Yaitiro Sakamoto Mantovani RA:11116013 
Descrição: O projeto SpaceRunners é um jogo Side Scrolling, em que o objetivo é evitar que a nave bata nos foguetes o maximo de tempo possivel.
Para jogar use W, S ou as setas do teclado.

-Projeto-

  O projeto foi uma adaptação do projeto visto em aula "Asteroids". Foram modificadas as classes windows.cpp, ship.cpp, gamedata.hpp e criadas as classes rockets.hpp/rockest.cpp que são similares as classes asteroids.hpp/asteroids.cpp.

windows.cpp
  No metodo "onEvent" foram retiradas algumas teclas como opção de movimentação da nave, pois no novo projeto ela vai apenas se locomover para frente e se movimentar para os lados.
No metodo "restart" são criados 7 foguetes, no lugar de 3 asteroids.

rockets.cpp
  No metodo "create" os foguetes são criados de acordo com a quantidade especificada na classe windows.cpp, em posições aleatorias a direita do campo de visão do jogador.
  No metodo "update" a translação dos foguetes é modificada pelo vetor de velocidade da nave, e quando os foguetes chegam até o limite esquerdo do campo de visão do jogador, eles são movidos para o limite direito um pouco alem do campo de visão, em posições aleatorias no eixo Y.
  No metodo "makeRocket" são definidas as coordenadas dos vertices dos foguetes, o VBO, EBO e VAO que será usado na renderização.
  No metodo "paint" a renderização é feita, e no "destroy" o VBO, EBO e VAO são liberados.

ship.cpp
  No metodo "create" é configurada a posição inicial da nave, junto com as coordenadas dos vertices, o VBO, EBO e VAO que será usado na renderização.
  No metodo "update" é configurado o vetor de velocidade da nave, para que siga de forma constante para a direita. Também é configurado a movimentação da nave no eixo Y, ao pressionar os botões S/W ou as setas do teclado.

gamedata.hpp
  Foi feita modificações nos enums "Input" e "State", pois o jogo só terá o estado "Playing" e "GameOver" e os unicos inputs serão "Up" e "Down".


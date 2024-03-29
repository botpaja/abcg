#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Up, Down };
enum class State { Playing, GameOver };

struct GameData {
  State m_state{State::Playing};
  std::bitset<5> m_input;  // [up, down]
};

#endif
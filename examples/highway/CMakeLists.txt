project(highway)
add_executable(${PROJECT_NAME} camera.cpp ground.cpp main.cpp window.cpp wheel.cpp)
enable_abcg(${PROJECT_NAME})

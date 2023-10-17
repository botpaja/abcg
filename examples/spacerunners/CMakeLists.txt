project(spacerunners)
add_executable(${PROJECT_NAME} main.cpp window.cpp rockets.cpp ship.cpp starlayers.cpp)
enable_abcg(${PROJECT_NAME})

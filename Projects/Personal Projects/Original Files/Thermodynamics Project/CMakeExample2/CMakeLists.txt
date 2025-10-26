# Minimum required CMake version
cmake_minimum_required(VERSION 3.14)

# Set the project name
project(MyMatplotProject VERSION 1.0)

# Use C++17, which Matplot++ requires
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable the FetchContent module
include(FetchContent)

# Specify the Matplot++ repository and tag
FetchContent_Declare(
  matplotplusplus
  GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
  GIT_TAG        origin/master # Use a specific tag or branch if needed
)

# Populate the content and add it to the project
FetchContent_MakeAvailable(matplotplusplus)

# Add your executable
add_executable(my_executable main.cpp)

# Link your executable to the matplot library
target_link_libraries(my_executable PRIVATE matplot)
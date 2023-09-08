# Nodeflow

A C++ computational graph library with reverse-mode automatic differentiation support.

## Installation with CMake:

1. Clone the project:

```bash
git clone https://github.com/sokmontrey/nodeflow.git
```

2. Link the library:

```cmake
add_subdirectory(nodeflow)

target_link_libraries(${PROJECT_NAME} nodeflow)
```


# Nodeflow

A C++ computational graph library with reverse-mode automatic differentiation support.

### Simple function evaluation
```cpp
using namespace nodeflow;

Node x = Node::Scalar(3.1415);

Sin f(&x);

f.forward();
f.print();
```

### Combination of functions
```cpp
using namespace nodeflow;

Node a = Node::Scalar(1);
Node b = Node::Scalar(1);
Node c = Node::Scalar(0.5);

Add add (&a, &b);
Sin sin (&c);
Mul f (&sin, &add);

f.forward();
f.print();
```

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


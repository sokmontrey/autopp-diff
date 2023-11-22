# Nodeflow Cpp
A simple and highly-extensible computational graph library written in C++.

## A sneak peak

**Create a Simple Function:**

TODO: reset() before forward() on graph

```cpp
// f = sin(a * b)
Graph f ("sin(mul(a, b))");

f.setNode("a", Node::Scalar(3.14));
f.setNode("b", Node::Scalar(0.25));

//calculate sin(3.14 * 0.25)
f.forward().print();
```

**Automatic Differentiation:**

```cpp
f.backward();
//df/da
std::cout << f.getGrad("a") << std::endl;
//df/db
std::cout << f.getGrad("b") << std::endl;
```

## Installation 

1. Clone the project:

```bash
git clone https://github.com/sokmontrey/nodeflow.git
```

2. Link the library:

```cmake
add_subdirectory(nodeflow)
add_executable(main main.cpp)
target_link_libraries(main nodeflow)
```

## Getting Start

### Node, Operator, and Graph

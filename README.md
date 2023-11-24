# Nodeflow Cpp
A simple and highly-extensible computational graph library written in C++.

## Simplicity using `Graph` (High-level code)

**Basic Calculation:**
```cpp
#include <nodeflow/nodeflow.h>

//f = sin(PI/6);
Graph f ("sin(div(PI, 6))");
f.forward().print();
```

**Function with variables:**
```cpp
// f = sin(a + b)
Graph f ("sin(add(a, b))");

//make node a and b a scalar (1x1 matrix)
f.setNode("a", Node::Scalar(0.1));
f.setNode("b", Node::Scalar(0.25));

//calculate sin(0.1 + 0.25)
f.forward().print();

//change variable value
f.setNode("a", Node::Scalar(0.2));
f.setNode("b", Node::Scalar(0.1));

//calculate sin(0.2 + 0.1)
f.forward();
//result is stored on the graph
f.print();
```
or 
```cpp
// f = sin(a + b)
Graph f ("sin(mul(a, b))", {
	{"a", Node::Scalar(0.1)}, //initialize variables immediately
	{"b", Node::Scalar(0.25)}
});
f.forward().print();
```

**Automatic Differentiation:**
```cpp
Graph f ("tanh(add(mul(w, x), b))", {
	{"w", }
});

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

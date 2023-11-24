# Nodeflow Cpp
A simple and highly-extensible computational graph library written in C++.

## Simplicity using `Graph` class (High-level class)

### Creating a Function (Graph)
**Simple Function:**

$$\sin(\frac{\pi}{6})$$

```cpp
// create the function (graph)
Graph f ("sin(div(PI, 6))");

// evaluate sin(PI/6)
f.forward().print();
```

**Function with variables:**
```cpp
// f = sin(a + b)
Graph f ("sin(add(a, b))", {
	//make node "a" and "b" a scalar (a number, 1x1 matrix)
	//directly in the constructor
	{"a", Node::Scalar(0.1)},
	{"b", Node::Scalar(0.25)}
});

//calculate sin(0.1 + 0.25)
f.forward().print();

//change variable Node
f
	//update node value directly with eigen matrix
	.set("a", Eigen::MatrixXd::Constant(1,1,0.2))
	.setNode("b", Node::Random()) //random scalar value 0-1
	.forward() //calculate sin(0.2 + 0.1)
	.print()
;
```

**Vector and Matrix:**
Nodeflow cpp use Eigen::MatrixXd as node's value. Any 0D, 1D, or 2D tensor will be represent in a node by Eigen::MatrixXd.

All operators in Nodeflow support matrix right off the bat (mostly element-wise except matrix multiplication, "mul"). 

>[!Note]
>Element-wise operation: operation on corresponding element (same row and col index. i.e. `a[i][j] + b[i][j]` for `0<=i<rows`, `0<=j<columns` and `a.rows == b.rows` and `a.cols == b.cols`) 

```cpp
Graph f ("mul(a, b)");

f
	.setNode("a", Node::Matrix({
		 {1, 2, 3},
		 {3, 4, 5}
	}))
	.setNode("b", Node::Vector({
		1, 2, 3
	}))
	.forward() // calculate matrix multiplcation a * b
	.print()
;
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

2. Link the library using CMake:

```cmake
add_subdirectory(nodeflow)
add_executable(main main.cpp)
target_link_libraries(main nodeflow)
```

3. Start coding:
```cpp
#include <nodeflow/nodeflow.h>
using namespace nodeflow;
//your code
```

## Getting Start

### Node, Operator, and Graph

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

### Using Eigen3
```cpp
using namespace nodeflow;

Node a = Eigen::MatrixXd(3, 2);
a() << 1, 2, 
    3, 4,
    5, 6;
```

Using `node_name()` or `node_name.getValue()` return the reference to nodes' value which will alway be an `Eigen::MatrixXd`.

More examples of Node initialization:
```cpp
using namespace nodeflow;

// Eigen
Node a = Eigen::VectorXd(3);
Node b = Eigen::VectorXd::Random(3);
Node c = Eigen::VectorXd::Constant(3, 0);
//...

//
```

> [!NOTE]
> Further documentation will be here coming soon

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


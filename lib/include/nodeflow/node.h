#pragma once

namespace nodeflow{

template <typename DTYPE>
class Node{
    protected:
        bool is_differentiatable = true;
        DTYPE _data;

    public:
        Node() = default;
        Node(DTYPE initial_data);

        void print();
};

}

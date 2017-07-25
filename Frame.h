#ifndef QP42FRAMES_FRAME_H
#define QP42FRAMES_FRAME_H


#include <vector>

class Frame {
public:
    unsigned int duration;

    std::vector<unsigned int> pixels;

    explicit Frame(unsigned int duration) : duration{duration} {}
};


#endif //QP42FRAMES_FRAME_H

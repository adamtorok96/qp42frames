//
// Created by edems on 2017.07.25..
//

#ifndef QP42FRAMES_FRAME_H
#define QP42FRAMES_FRAME_H


#include <vector>

class Frame {
public:
    std::vector<unsigned int> pixels;

    unsigned int duration;

    explicit Frame(unsigned int duration) : duration{duration} {}
};


#endif //QP42FRAMES_FRAME_H

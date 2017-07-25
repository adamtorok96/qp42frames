#ifndef QP42FRAMES_CLIP_H
#define QP42FRAMES_CLIP_H

#include <string>
#include <algorithm>

#include "Frame.h"
#include "Embed.h"

class Clip {
public:
    std::string name;
    unsigned int width, height;

    std::vector<Frame> frames;
    std::vector<Embed> embeds;

    unsigned int duration, realDuration;

    Clip(std::string name, unsigned int width, unsigned int height) : name{std::move(name)}, width{width}, height{height}, duration{0}, realDuration{0} {};

    void calculateDurationFromFrames();
    void sortEmbeds();
};


#endif //QP42FRAMES_CLIP_H

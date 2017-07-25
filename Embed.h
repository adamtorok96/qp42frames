#ifndef QP42FRAMES_EMBED_H
#define QP42FRAMES_EMBED_H

#include <string>

class Embed {
public:
    std::string clip;

    unsigned int x, y;
    int z;

    unsigned int t;

    Embed(std::string clip, unsigned int x, unsigned int y, int z, unsigned int t) : clip{std::move(clip)}, x{x}, y{y}, z{z}, t{t} {};
};


#endif //QP42FRAMES_EMBED_H

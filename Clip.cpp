#include "Clip.h"

void Clip::calculateDurationFromFrames() {
    for(auto const & frame : frames) {
        duration += frame.duration;
    }
}

void Clip::sortEmbeds() {
    std::sort(embeds.begin(), embeds.end(), [](const Embed & a, const Embed & b) -> bool {
        return a.t < b.t;
    });
}
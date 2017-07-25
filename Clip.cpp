#include "Clip.h"

void Clip::sortEmbeds() {
    std::sort(embeds.begin(), embeds.end(), [](const Embed & a, const Embed & b) -> bool {
        return a.t < b.t;
    });
}

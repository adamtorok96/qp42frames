#include "Matrix.h"

Clip &Matrix::getClipByName(std::string name) {
    for(auto & clip : clips) {
        if( strcmp(name.c_str(), clip.name.c_str()) == 0 )
            return clip;
    }
}

Clip &Matrix::getRootClip() {
    return getClipByName(rootClip);
}

void Matrix::calculateMaxClipDuration() {
    calculateClipDuration(getRootClip());
}

void Matrix::calculateClipDuration(Clip &clip) {
    clip.calculateDurationFromFrames();

    unsigned int max = clip.duration;

    for(auto const &embed : clip.embeds) {
        auto & c = getClipByName(embed.clip);

        calculateClipDuration(c);

        if( c.realDuration + embed.t > max )
            max = c.realDuration + embed.t;
    }

    clip.realDuration = max;
}

void Matrix::generateEmptyFrames() {
    for(auto t = 0; t < (getRootClip().realDuration * FPS) / 1000; t++) {
        frames.emplace_back(26, 32, CV_8UC4);
    }
}

void Matrix::generateFrames() {
    generateFrame(getRootClip(), 0);
}

void Matrix::generateFrame(Clip & clip, unsigned int offset) {
    unsigned int time = 0;

    for(auto const & frame : clip.frames) {

        //std::cout << frame.duration << " --> " << ((frame.duration * FPS) / 1000) << std::endl;

        Mat realFrame = Mat(26, 32, CV_8UC4);

        auto i = 0;

        for(auto pixel : frame.pixels) {
            realFrame.at<Vec4b>(i / 32, i % 32) = Vec4b(
                    (unsigned char)(((pixel & 0x00FF0000) >> 4) & 0xFF),
                    (unsigned char)(((pixel & 0x0000FF00) >> 2) & 0xFF),
                    (unsigned char)(((pixel & 0x000000FF) >> 0) & 0xFF),
                    (unsigned char)(((pixel & 0xFF000000) >> 6) & 0xFF)
            );

            i++;
        }

        for(auto t = 0; t < (frame.duration * FPS) / 1000; t++) {
            frames[time + offset + t] = realFrame;
        }

//        for(auto t = 0; t < (frame.duration * FPS) / 1000; t++) {
//            auto i = 0;
//
//            for (auto pixel : frame.pixels) {
//                frames[time + offset + t].at<Vec4b>(i / 32, i % 32) = Vec4b(
//                        (unsigned char) (((pixel & 0x00FF0000) >> 4) & 0xFF),
//                        (unsigned char) (((pixel & 0x0000FF00) >> 2) & 0xFF),
//                        (unsigned char) (((pixel & 0x000000FF) >> 0) & 0xFF),
//                        (unsigned char) (((pixel & 0xFF000000) >> 6) & 0xFF)
//                );
//
//                i++;
//            }
//        }

        time += (frame.duration * FPS) / 1000;
    }

    clip.sortEmbeds();

    for(auto const & embed : clip.embeds) {
        generateFrame(getClipByName(embed.clip), (embed.t * FPS) / 1000);
    }
}

void Matrix::reScale(unsigned int scale) {
    for(auto & frame : frames) {
        frame = reScaleMat(frame, scale);
    }
}

Mat Matrix::reScaleMat(Mat const & src, unsigned int scale) {
    Mat dst = Mat(src.rows * scale, src.cols * scale, src.type());

    for(auto y = 0; y < dst.rows; y++) {
        for(auto x = 0; x < dst.cols; x++) {
            dst.at<Vec4b>(y, x) = src.at<Vec4b>(y / scale, x / scale);
        }
    }

    return dst;
}

void Matrix::saveAs(std::string prefix) {
    auto c = 0;

    for(auto const & frame : frames) {
        imwrite((prefix + std::to_string(c++) + ".png").c_str(), frame);
    }
}

void Matrix::reScaleAndSave(unsigned int scale, std::string prefix) {
    auto c = 0;

    for(auto i = 0; i < frames.size(); i += (FPS / 10)) {
        Mat dst = reScaleMat(frames[i], scale);

        imwrite((prefix + std::to_string(c++) + ".png").c_str(), dst);
    }
}

void Matrix::setRootClip(const std::string &rootClip) {
    this->rootClip = rootClip;
}

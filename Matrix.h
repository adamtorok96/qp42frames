#ifndef QP42FRAMES_MATRIX_H
#define QP42FRAMES_MATRIX_H

#include <string>
#include <cstring>
#include <iostream>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include "Clip.h"

#define FPS 500

using namespace cv;

class Matrix {
    void calculateClipDuration(Clip & clip);
    void generateFrame(Clip const & clip, unsigned int offset);
    Mat reScaleMat(Mat const & src, unsigned int scale);

public:
    std::string rootclip;

    std::vector<Clip> clips;
    std::vector<Mat> frames;

    explicit Matrix(std::string rootclip) : rootclip{std::move(rootclip)} {};

    Clip & getClipByName(std::string name);
    Clip & getRootClip();

    void calculateMaxClipDuration();
    void generateEmptyFrames();
    void generateFrames();
    void reScale(unsigned int scale);
    void saveAs(std::string prefix);

    void reScaleAndSave(unsigned int scale, std::string prefix);
};


#endif //QP42FRAMES_MATRIX_H

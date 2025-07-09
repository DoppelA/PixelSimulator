#ifndef ADVCPPSIMPIXEL_BOARDPRESENTER_H
#define ADVCPPSIMPIXEL_BOARDPRESENTER_H

#include "PixelBoard.h"
#include <opencv2/opencv.hpp>
#include <thread>

class BoardPresenter {
public:
    BoardPresenter(uint16_t width, uint16_t height);
    BoardPresenter(uint16_t width, uint16_t height, PixelBoard::pixel mat);
    //~BoardPresenter() = default;

    void updateVisualBoard();
    void updateMathBoard();
    void BoardPresenter::showBoard();
    void setAt(const uint16_t & x,const uint16_t & y, const PixelBoard::pixel & toSet);
    void drawCube(uint16_t x, uint16_t y,uint8_t size, PixelBoard::pixel material);
    void drawSquare(uint16_t startY, uint16_t startX, uint16_t endY, uint16_t endX, PixelBoard::pixel material);
    void RepeatMathBoard(const bool & pause);
    void RepeatVisualBoard(const bool & pause);

    const PixelBoard::pixel getAt (const uint16_t & x,const uint16_t & y);

    PixelBoard::pixel paintMaterial = PixelBoard::pixel::AIR;

    bool mouseUp = true, firstCornerSet = false;
    uint16_t squareX, squareY;

private:
    uint16_t width, height;
    uint16_t delay = 1;

    cv::Mat cvBoard;
    std::string winName = "PixelSimulator";

    PixelBoard livePixelBoard;

    static void CallBackF(int event, int x, int y, int flags, void* img);
};

#endif //ADVCPPSIMPIXEL_BOARDPRESENTER_H

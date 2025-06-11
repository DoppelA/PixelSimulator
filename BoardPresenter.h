#ifndef ADVCPPSIMPIXEL_BOARDPRESENTER_H
#define ADVCPPSIMPIXEL_BOARDPRESENTER_H

#include "PixelBoard.h"
#include <opencv2/opencv.hpp>

class BoardPresenter {
public:
    BoardPresenter(uint16_t width, uint16_t height);
    BoardPresenter(uint16_t width, uint16_t height, PixelBoard::pixel mat);
    //~BoardPresenter() = default;

    void updateVisualBoard();
    void BoardPresenter::showBoard();
    void setAt(const uint16_t & x,const uint16_t & y, const PixelBoard::pixel & toSet);
    void drawCube(uint16_t x, uint16_t y,uint8_t size, PixelBoard::pixel material);

    const PixelBoard::pixel getAt (const uint16_t & x,const uint16_t & y);

private:
    uint16_t width, height;
    uint8_t delay = 0;

    cv::Mat cvBoard;
    PixelBoard livePixelBoard;
};


#endif //ADVCPPSIMPIXEL_BOARDPRESENTER_H

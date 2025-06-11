#ifndef ADVCPPSIMPIXEL_BOARDPRESENTER_H
#define ADVCPPSIMPIXEL_BOARDPRESENTER_H

#include "PixelBoard.h"
#include <opencv2/opencv.hpp>

class BoardPresenter {
public:
    BoardPresenter(uint16_t width, uint16_t height);
    ~BoardPresenter();
    void updateVisualBoard();
    void BoardPresenter::showBoard();
    uint8_t delay;


private:
    uint16_t width, height;
    cv::Mat cvBoard;
    PixelBoard livePixelBoard;
};


#endif //ADVCPPSIMPIXEL_BOARDPRESENTER_H

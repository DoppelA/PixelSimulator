#include "BoardPresenter.h"

BoardPresenter::BoardPresenter(uint16_t width, uint16_t height) : width(width) , height(height), livePixelBoard{height, width} {
    cv::Mat temp(width, height, CV_8UC3);
    cvBoard = temp;
    delay = 5;
}

BoardPresenter::BoardPresenter(uint16_t width, uint16_t height, uint8_t delay) : width(width) , height(height),delay(delay) , livePixelBoard{width, height} {
    cv::Mat temp(width, height, CV_8UC3);
    cvBoard = temp;
    delay = 5;
}

void BoardPresenter::showBoard() {
    bool pause = false;
    while (true){
        if (!pause)
            updateVisualBoard();

        cv::imshow("test", cvBoard);

        int key = cv::waitKey(delay);
        //switch
        if (key == 'q' || key == 'Q')
            return;
        else if (key == '-' && delay > 5) {
            delay -= 5;
        } else if (key == '+' && delay < 200) {
            delay += 5;
        } else if (key == 'p' || key == 'P') {
            pause = !pause;
        }
    }
}

void BoardPresenter::updateVisualBoard() {
    livePixelBoard.updateBoard();

    for (int i = 0; i < cvBoard.rows; ++i)
        for (int j = 0; j < cvBoard.cols; ++j) {
            int randfac = (i + j) % 10;
            randfac *= (i + j % 2) ? -1 : 1;
            switch (livePixelBoard.getAt(i, j)) {
                case PixelBoard::pixel::AIR:
                    cvBoard.at<cv::Vec3b>(i, j)[0] = 254;
                    cvBoard.at<cv::Vec3b>(i, j)[1] = 254;
                    cvBoard.at<cv::Vec3b>(i, j)[2] = 254;
                    break;

                case PixelBoard::pixel::WOOD:
                    cvBoard.at<cv::Vec3b>(i, j)[0] = 0;
                    cvBoard.at<cv::Vec3b>(i, j)[1] = 75 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[2] = 150 + randfac;
                    break;

                case PixelBoard::pixel::STONE:
                    cvBoard.at<cv::Vec3b>(i, j)[0] = 100 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[1] = 100 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[2] = 100 + randfac;
                    break;

                case PixelBoard::pixel::FIRE:
                    cvBoard.at<cv::Vec3b>(i, j)[0] = 10 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[1] = 10 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[2] = 250 + randfac;
                    //temp++;
                    break;

                case PixelBoard::pixel::WATER:
                    cvBoard.at<cv::Vec3b>(i, j)[0] = 250 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[1] = 10 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[2] = 10 + randfac;
                    break;

                case PixelBoard::pixel::SMOKE:
                    cvBoard.at<cv::Vec3b>(i, j)[0] = 175 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[1] = 175 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[2] = 175 + randfac;
                    break;

                case PixelBoard::pixel::SAND:
                    cvBoard.at<cv::Vec3b>(i, j)[0] = 128 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[1] = 178 + randfac;
                    cvBoard.at<cv::Vec3b>(i, j)[2] = 194 + randfac;
                    break;

                default:
                    cvBoard.at<cv::Vec3b>(i, j)[0] = ((unsigned char) livePixelBoard.getAt(i, j) * 10);
                    cvBoard.at<cv::Vec3b>(i, j)[1] = ((unsigned char) livePixelBoard.getAt(i, j) * 10);
                    cvBoard.at<cv::Vec3b>(i, j)[2] = ((unsigned char) livePixelBoard.getAt(i, j) * 10);
            }
        }
}

const PixelBoard::pixel BoardPresenter::getAt (const uint16_t & x,const uint16_t & y) {
    return livePixelBoard.getAt(x,y);
}

void BoardPresenter::setAt(const uint16_t & x,const uint16_t & y, const PixelBoard::pixel & toSet){
    livePixelBoard.setAt(x,y, toSet);
}

void BoardPresenter::drawCube(uint16_t x, uint16_t y,uint8_t size, PixelBoard::pixel material) {
    livePixelBoard.drawCube(x,y,size,material);
}
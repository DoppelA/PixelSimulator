#include "BoardPresenter.h"

//BoardPresenter::BoardPresenter(uint16_t width, uint16_t height) : width(width) , height(height), livePixelBoard{height, width}, cvBoard{width, height, CV_8UC3} {}

BoardPresenter::BoardPresenter(uint16_t width, uint16_t height, PixelBoard::pixel mat) : width(width) , height(height), livePixelBoard{width, height, mat}, cvBoard{width, height, CV_8UC3}{}

void BoardPresenter::showBoard() {
    bool pause = false;

    cv::namedWindow(winName, cv::WND_PROP_FULLSCREEN);
    cv::setWindowProperty(winName, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    cv::setMouseCallback(winName, CallBackF, this);
    while (true){
        if (!pause) {
            std::thread th1(&BoardPresenter::updateVisualBoard, this);
            std::thread th2(&BoardPresenter::updateMathBoard, this);
            th1.join();
            th2.join();
            //updateVisualBoard();
            //updateMathBoard();
            }

        imshow(winName,cvBoard);

        int key = cv::waitKey(delay);
        switch(key) {
            case 'q':
            case 'Q':
                return;

            case 'P':
            case 'p':
                pause = !pause;
                break;

            case '-':
                if (delay - 5 > 0)
                    delay -= 5;
                break;

            case '+':
                delay += 5;
                break;

            default:
                if (key >= '0' && key <= (static_cast<uchar>(PixelBoard::pixel::NUM_TYPES) + ('0' - 1)))
                    paintMaterial = static_cast<PixelBoard::pixel>(key - '0');
                break;
        }
    }
}

void BoardPresenter::updateVisualBoard() {
    for (int y = 0; y < cvBoard.rows; ++y)
        for (int x = 0; x < cvBoard.cols; ++x) {
            int randfac = (y + x) % 10;
            randfac *= (y + x % 2) ? -1 : 1;
            switch (livePixelBoard.getAt(y, x)) {
                case PixelBoard::pixel::AIR:
                    cvBoard.at<cv::Vec3b>(y, x)[0] = 254;
                    cvBoard.at<cv::Vec3b>(y, x)[1] = 254;
                    cvBoard.at<cv::Vec3b>(y, x)[2] = 254;
                    break;

                case PixelBoard::pixel::WOOD:
                    cvBoard.at<cv::Vec3b>(y, x)[0] = 0;
                    cvBoard.at<cv::Vec3b>(y, x)[1] = 75 + randfac;
                    cvBoard.at<cv::Vec3b>(y, x)[2] = 150 + randfac;
                    break;

                case PixelBoard::pixel::STONE:
                    cvBoard.at<cv::Vec3b>(y, x)[0] = (100 + randfac);
                    cvBoard.at<cv::Vec3b>(y, x)[1] = 100 + randfac;
                    cvBoard.at<cv::Vec3b>(y, x)[2] = 100 + randfac;
                    break;

                case PixelBoard::pixel::FIRE:
                    cvBoard.at<cv::Vec3b>(y, x)[0] = 10 + randfac;
                    cvBoard.at<cv::Vec3b>(y, x)[1] = 10 + randfac;
                    cvBoard.at<cv::Vec3b>(y, x)[2] = 250 + randfac;
                    //temp++;
                    break;

                case PixelBoard::pixel::WATER:
                    cvBoard.at<cv::Vec3b>(y, x)[0] = 250 + randfac;
                    cvBoard.at<cv::Vec3b>(y, x)[1] = 10 + randfac;
                    cvBoard.at<cv::Vec3b>(y, x)[2] = 10 + randfac;
                    break;

                case PixelBoard::pixel::SMOKE:
                    cvBoard.at<cv::Vec3b>(y, x)[0] = 175 + randfac;
                    cvBoard.at<cv::Vec3b>(y, x)[1] = 175 + randfac;
                    cvBoard.at<cv::Vec3b>(y, x)[2] = 175 + randfac;
                    break;

                case PixelBoard::pixel::SAND:
                    cvBoard.at<cv::Vec3b>(y, x)[0] = 128 + randfac;
                    cvBoard.at<cv::Vec3b>(y, x)[1] = 178 + randfac;
                    cvBoard.at<cv::Vec3b>(y, x)[2] = 194 + randfac;
                    break;

                default:
                    cvBoard.at<cv::Vec3b>(y, x)[0] = (static_cast<uchar>(livePixelBoard.getAt(y, x)) * 10);
                    cvBoard.at<cv::Vec3b>(y, x)[1] = (static_cast<uchar>(livePixelBoard.getAt(y, x)) * 10);
                    cvBoard.at<cv::Vec3b>(y, x)[2] = (static_cast<uchar>(livePixelBoard.getAt(y, x)) * 10);
            }
        }
}

void BoardPresenter::RepeatVisualBoard(const bool & pause) {
    while(!pause)
        updateVisualBoard();
}

void BoardPresenter::updateMathBoard() {
    livePixelBoard.updateBoard();
}

void BoardPresenter::RepeatMathBoard(const bool & pause) {
    while(!pause)
        livePixelBoard.updateBoard();
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

void BoardPresenter::drawSquare(uint16_t startY, uint16_t startX, uint16_t endY, uint16_t endX, PixelBoard::pixel material) {
    livePixelBoard.drawSquare(startY, startX, endY, endX, material);
}

void BoardPresenter::CallBackF(int event, int x, int y, int flags, void* Board) {
    //std::cout<< "test 1" << std::endl;
    BoardPresenter *curBoard = (BoardPresenter*)Board;

    if(event == cv::EVENT_LBUTTONDOWN) {
       curBoard->mouseUp = false;
    }
    else if (event == cv::EVENT_LBUTTONUP){
        curBoard->mouseUp = true;
    }
    else if (event == cv::EVENT_RBUTTONDOWN) {
        if (!curBoard->firstCornerSet) {
            curBoard->squareX = x;
            curBoard->squareY = y;
            curBoard->firstCornerSet = true;
        } else {
            curBoard->drawSquare(curBoard->squareY,curBoard->squareX,y,x,curBoard->paintMaterial);
            curBoard->firstCornerSet = false;
        }
    }
    if (!curBoard->mouseUp) {
        curBoard->setAt(y,x,curBoard->paintMaterial);
    }
}
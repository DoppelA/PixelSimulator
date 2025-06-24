#include "BoardPresenter.h"

BoardPresenter::BoardPresenter(uint16_t width, uint16_t height) : width(width) , height(height), livePixelBoard{height, width} {
    cv::Mat temp(width, height, CV_8UC3);
    cvBoard = temp;
}

BoardPresenter::BoardPresenter(uint16_t width, uint16_t height, PixelBoard::pixel mat) : width(width) , height(height), livePixelBoard{width, height, mat} {
    cv::Mat temp(width, height, CV_8UC3);
    cvBoard = temp;
}

void BoardPresenter::showBoard() {
    bool pause = false;

    cv::namedWindow("Pixel Simulator", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Pixel Simulator", CallBackF, this);
    while (true){
        if (!pause) {
            std::thread th1(&BoardPresenter::updateVisualBoard, this);
            std::thread th2(&BoardPresenter::updateMathBoard, this);
            th1.join();
            th2.join();
        }

        cv::imshow("Pixel Simulator", cvBoard);

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
                if (delay - 5 >= 0)
                    delay -= 5;
                break;

            case '+':
                delay += 5;
                break;

            default:
                if (key >= '0' && key <= ((int)PixelBoard::pixel::NUM_TYPES + 48))
                    paintMaterial = (PixelBoard::pixel)(key - 48);
                break;
        }
    }
}

void BoardPresenter::updateVisualBoard() {
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

void BoardPresenter::CallBackF(int event, int x, int y, int flags, void* Board) {
    //std::cout<< "test 1" << std::endl;
    BoardPresenter curBoard = *(BoardPresenter*)Board;
    if(event == cv::EVENT_LBUTTONDOWN) {
       curBoard.mouseUp = false;
    }
    if (event == cv::EVENT_LBUTTONUP){
        curBoard.mouseUp = true;
    }
    if (!curBoard.mouseUp) {
        std::cout << "drawing: " << (int)curBoard.paintMaterial << " at: " << std::endl << "x: " << x << " y: " << y << std::endl;
        curBoard.setAt(x,y,curBoard.paintMaterial);
    }
}
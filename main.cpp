#include <iostream>
#include <benchmark/benchmark.h>
#include <opencv2/opencv.hpp>
#include "PixelBoard.h"

void writePixel(cv::Mat  &img, PixelBoard &dst);

static void BM_PixelBoardIni(benchmark::State& state) {
    int size = state.range(0);

    for (auto _ : state) {
        //benchmark::DoNotOptimize(toSort);
        PixelBoard brd(size, size/*, PixelBoard::pixel::WOOD*/);
    }
}

static void BM_PixelBoardUpdate(benchmark::State& state) {
    int size = state.range(0);
    PixelBoard brd(size, size/*,PixelBoard::pixel::WOOD*/);

    for (auto _ : state) {
        //benchmark::DoNotOptimize(toSort);
        brd.updateBoard();
    }
}


BENCHMARK(BM_PixelBoardIni)->Unit(benchmark::kMillisecond)-> Range(1080, 3840);
BENCHMARK(BM_PixelBoardUpdate)->Unit(benchmark::kMillisecond)-> Range(1080, 3840);

int main(int argc, char **argv) {
   //::benchmark::Initialize(&argc, argv);
   //::benchmark::RunSpecifiedBenchmarks();
   //::benchmark::Shutdown();

    PixelBoard testBoard(300,720, PixelBoard::pixel::WOOD);
    testBoard.setAt(100,100,PixelBoard::pixel::FIRE);
    testBoard.setAt(100,120,PixelBoard::pixel::SAND);
    testBoard.setAt(100,119,PixelBoard::pixel::SAND);
    testBoard.setAt(101,120,PixelBoard::pixel::SAND);
    testBoard.setAt(101,119,PixelBoard::pixel::SAND);
    testBoard.setAt(200,120,PixelBoard::pixel::WATER);
    testBoard.setAt(200,119,PixelBoard::pixel::WATER);
    testBoard.setAt(201,120,PixelBoard::pixel::WATER);
    testBoard.setAt(201,119,PixelBoard::pixel::WATER);
    testBoard.setAt(202,120,PixelBoard::pixel::WATER);
    testBoard.setAt(202,119,PixelBoard::pixel::WATER);
    testBoard.setAt(203,120,PixelBoard::pixel::WATER);
    testBoard.setAt(203,119,PixelBoard::pixel::WATER);
    //testBoard.retBoard(std::cout);

    cv::Mat img(testBoard.width, testBoard.height, CV_8UC3);
    uint8_t delay = 6;
    bool pause = false;
    while(true) {
        if (!pause) {
            testBoard.updateBoard();
            writePixel(img, testBoard);
        }

        cv::imshow("test",img);

        int key = cv::waitKey(delay);
        if(key == 'q' || key == 'Q') {
            return 0;
        }
        else if(key == '-' && delay > 5){
            delay -= 5;
        }
        else if(key == '+' && delay < 200){
            delay += 5;
        }
        else if(key == 'p' || key == 'P'){
            pause = !pause;
        }
    }

    return 0;
}

void writePixel(cv::Mat  &img, PixelBoard &dst) {
    int temp = 0;
    for (int i = 0; i < img.rows; ++i)
        for (int j = 0; j < img.cols; ++j) {
            int randfac = (i + j) % 10;
            randfac *= (i + j % 2) ? -1 : 1;
            switch (dst.getAt(i,j)) {
                case PixelBoard::pixel::AIR:
                    img.at<cv::Vec3b>(i, j)[0] = 254;
                    img.at<cv::Vec3b>(i, j)[1] = 254;
                    img.at<cv::Vec3b>(i, j)[2] = 254;
                    break;

                case PixelBoard::pixel::WOOD:
                    img.at<cv::Vec3b>(i, j)[0] = 0;
                    img.at<cv::Vec3b>(i, j)[1] = 75 + randfac;
                    img.at<cv::Vec3b>(i, j)[2] = 150 + randfac;
                    break;

                case PixelBoard::pixel::STONE:
                    img.at<cv::Vec3b>(i, j)[0] = 100 + randfac;
                    img.at<cv::Vec3b>(i, j)[1] = 100 + randfac;
                    img.at<cv::Vec3b>(i, j)[2] = 100 + randfac;
                    break;

                case PixelBoard::pixel::FIRE:
                    img.at<cv::Vec3b>(i, j)[0] = 10 + randfac;
                    img.at<cv::Vec3b>(i, j)[1] = 10 + randfac;
                    img.at<cv::Vec3b>(i, j)[2] = 250 + randfac;
                    //temp++;
                    break;

                case PixelBoard::pixel::WATER:
                    img.at<cv::Vec3b>(i, j)[0] = 250 + randfac;
                    img.at<cv::Vec3b>(i, j)[1] = 10 + randfac;
                    img.at<cv::Vec3b>(i, j)[2] = 10 + randfac;
                    temp++;
                    break;

                case PixelBoard::pixel::SMOKE:
                    img.at<cv::Vec3b>(i, j)[0] = 175 + randfac;
                    img.at<cv::Vec3b>(i, j)[1] = 175 + randfac;
                    img.at<cv::Vec3b>(i, j)[2] = 175 + randfac;
                    break;

                case PixelBoard::pixel::SAND:
                    img.at<cv::Vec3b>(i, j)[0] = 128 + randfac;
                    img.at<cv::Vec3b>(i, j)[1] = 178 + randfac;
                    img.at<cv::Vec3b>(i, j)[2] = 194 + randfac;
                    break;

                default:
                    img.at<cv::Vec3b>(i, j)[0] = ((unsigned char) dst.getAt(i,j) * 10);
                    img.at<cv::Vec3b>(i, j)[1] = ((unsigned char) dst.getAt(i,j) * 10);
                    img.at<cv::Vec3b>(i, j)[2] = ((unsigned char) dst.getAt(i,j) * 10);
            }
        }
    //std::cout << temp << std::endl;
}
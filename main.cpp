#include <iostream>
#include <benchmark/benchmark.h>
#include <opencv2/opencv.hpp>
#include "PixelBoard.h"
#include "BoardPresenter.h"

void writePixel(cv::Mat  &img, PixelBoard &dst);

static void BM_PixelBoardIni(benchmark::State& state) {
    int size = state.range(0);

    for (auto _ : state) {
        //benchmark::DoNotOptimize(toSort);
        BoardPresenter brd(size, size, PixelBoard::pixel::WOOD);
    }
}

static void BM_PixelBoardUpdate(benchmark::State& state) {
    int size = state.range(0);
    BoardPresenter brd(750,750,PixelBoard::pixel::WOOD);

    brd.setAt(220,420,PixelBoard::pixel::FIRE);
    brd.setAt(220,20,PixelBoard::pixel::FIRE);
    brd.drawCube(40,40,100,PixelBoard::pixel::SAND);
    brd.drawCube(100,100,100,PixelBoard::pixel::WATER);


    for (auto _ : state) {
        //benchmark::DoNotOptimize(brd);
        for (int i = 0; i < 1; i++) {
            brd.updateMathBoard();
            brd.updateVisualBoard();
        }
    }
}


BENCHMARK(BM_PixelBoardIni)->Unit(benchmark::kMillisecond)-> Range(1080, 3840);
BENCHMARK(BM_PixelBoardUpdate)->Unit(benchmark::kMillisecond)-> Range(1080, 3840);

int main(int argc, char **argv) {
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();

    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

    BoardPresenter pixelsim(1080 / 4,1920 / 4,PixelBoard::pixel::WOOD);
    pixelsim.setAt(220,420,PixelBoard::pixel::FIRE);
    pixelsim.setAt(220,20,PixelBoard::pixel::FIRE);
    pixelsim.drawCube(40,40,100,PixelBoard::pixel::SAND);
    pixelsim.drawCube(240,240,100,PixelBoard::pixel::SMOKE);
    pixelsim.drawCube(540,40,100,PixelBoard::pixel::WATER);

    pixelsim.showBoard();

    return 0;
}
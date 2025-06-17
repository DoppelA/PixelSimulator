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
    BoardPresenter brd(size, size,PixelBoard::pixel::WOOD);

    for (auto _ : state) {
        //benchmark::DoNotOptimize(toSort);
        brd.updateVisualBoard();
    }
}


BENCHMARK(BM_PixelBoardIni)->Unit(benchmark::kMillisecond)-> Range(1080, 3840);
BENCHMARK(BM_PixelBoardUpdate)->Unit(benchmark::kMillisecond)-> Range(1080, 3840);

int main(int argc, char **argv) {
    //::benchmark::Initialize(&argc, argv);
    //::benchmark::RunSpecifiedBenchmarks();
    //::benchmark::Shutdown();
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

    BoardPresenter pixelsim(750,750/*,PixelBoard::pixel::WOOD*/);
    pixelsim.setAt(20,20,PixelBoard::pixel::FIRE);
    pixelsim.drawCube(40,40,100,PixelBoard::pixel::SAND);
    pixelsim.drawCube(240,240,100,PixelBoard::pixel::SMOKE);

    pixelsim.showBoard();


    return 0;
}
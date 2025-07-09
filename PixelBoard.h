#ifndef ADVCPPSIMPIXEL_PIXELBOARD_H
#define ADVCPPSIMPIXEL_PIXELBOARD_H
#include<cstdint>
#include<vector>
#include <array>
#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include <algorithm>

class PixelBoard {

//private:
public:
    enum class pixel {AIR = 0, WOOD, WATER, SAND, STONE, FIRE, SMOKE, NUM_TYPES};
    enum class actions {NONE = 0, BURN, SOLIDIFY, GO_UP, FALL_DOWN, FIRETICK, EXTINGUISH, FLOW, SINK, ATOP}; //Makros?

    //PixelBoard(uint16_t width, uint16_t height);
    PixelBoard(uint16_t width, uint16_t height, pixel basePixel = pixel::AIR);
    ~PixelBoard() = default;

    void setAt(const uint16_t y,const uint16_t x, const pixel & toSet);
    void updateBoard();
    void printBoard (std::ostream &os) const;
    void drawCube(const uint16_t x, const uint16_t y, const uint8_t size, const pixel & material);
    void drawSquare(const uint16_t startY, const uint16_t startX, const uint16_t endY, const uint16_t endX, const pixel & material);
    const pixel getAt(const uint16_t y,const uint16_t x) const;

    const uint16_t width;
    const uint16_t height;

private:
    static std::vector<std::vector<actions>> reactionTable;
    std::vector<std::vector<pixel>> board;
    std::vector<std::vector<pixel>> flipboard;
    std::minstd_rand lce;

    void initReactTable();
    bool flipped = false;
};

#endif //ADVCPPSIMPIXEL_PIXELBOARD_H
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
    enum class actions {NONE = 0, BURN, TURNSMOKE, SOLIDIFY, GO_UP, FALL_DOWN, FIRETICK, EXTINGUISH, FLOW, SINK, ATOP}; //Makros?

    PixelBoard(uint16_t width, uint16_t height);
    PixelBoard(uint16_t width, uint16_t height, pixel basePixel);
    ~PixelBoard() = default;

    void setAt(const uint16_t & y,const uint16_t & x, const pixel & toSet);
    void updateBoard();
    void retBoard (std::ostream &os);
    void drawCube(uint16_t x, uint16_t y, uint8_t size, pixel material);
    void drawSquare(uint16_t startY, uint16_t startX, uint16_t endY, uint16_t endX, pixel material);
    const pixel getAt(const uint16_t & y,const uint16_t & x);

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
#ifndef ADVCPPSIMPIXEL_PIXELBOARD_H
#define ADVCPPSIMPIXEL_PIXELBOARD_H
#include<cstdint>
#include<vector>
#include <array>
#include <iostream>
#include <sstream>
#include <ctime>

class PixelBoard {

//private:
public:
    enum class pixel {AIR = 0, WOOD, WATER, SAND, STONE, FIRE, SMOKE, NUM_TYPES};
    enum class actions {NONE = 0, BURN, TURNSMOKE, SOLIDIFY, GO_UP, FALL_DOWN, FIRETICK, EXTINGUISH, FLOW};

    PixelBoard(uint16_t height, uint16_t width);
    PixelBoard(uint16_t height, uint16_t width, pixel basePixel);
    ~PixelBoard() = default;

    void setAt(const uint16_t & x,const uint16_t & y, const pixel & toSet);
    void updateBoard();
    void retBoard (std::ostream &os);

    const pixel getAt(const uint16_t & x,const uint16_t & y);

    const uint16_t width;
    const uint16_t height;
private:
    std::vector<std::vector<actions>> reactionTable;
    std::vector<std::vector<pixel>> board;
    std::vector<std::vector<pixel>> flipboard;

    //std::vector<pixel> board;
    //std::vector<pixel> flipboard;

    void initReactTable();
    bool flipped = false;
};

#endif //ADVCPPSIMPIXEL_PIXELBOARD_H
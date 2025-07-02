#include <cassert>
#include "PixelBoard.h"
std::vector<std::vector<PixelBoard::actions>> PixelBoard::reactionTable;

PixelBoard::PixelBoard(uint16_t height, uint16_t width) : width (width), height(height), board{height, std::vector<pixel>(width, pixel::AIR)} {
    initReactTable();
    srand(std::time(nullptr));

    for (uint16_t x = 0; x < width; x++) {
        board[0][x] = pixel::STONE;
        board[height - 1][x] = pixel::STONE;
    }
    for (uint16_t y = 0; y < height - 1; y++) {
        board[y][0] = pixel::STONE;
        board[y][width - 1] = pixel::STONE;
    }
    flipboard = board;
}

PixelBoard::PixelBoard(uint16_t height, uint16_t width, pixel basePixel) : width (width), height(height), board{height, std::vector<pixel>(width, basePixel)} {
    initReactTable();
    //std::cout << "y: " << board.size() << " x: " << board[0].size() << std::endl;
    srand(std::time(nullptr));

    for (uint16_t x = 0; x < width; x++) {
        board[0][x] = pixel::STONE;
        board[height - 1][x] = pixel::STONE;
    }
    for (uint16_t y = 0; y < height - 1; y++) {
        board[y][0] = pixel::STONE;
        board[y][width - 1] = pixel::STONE;
    }
    flipboard = board;
}



/*PixelBoard::~PixelBoard() {

}*/

void PixelBoard::initReactTable() {
    std::vector<std::vector<actions>> tempActions((uint8_t)pixel::NUM_TYPES, std::vector<actions>((uint8_t)pixel::NUM_TYPES, actions::NONE));
    reactionTable = tempActions;
    reactionTable[(uint8_t)pixel::WOOD][(uint8_t)pixel::FIRE] = actions::BURN;

    reactionTable[(uint8_t)pixel::FIRE][(uint8_t)pixel::WATER] = actions::TURNSMOKE;

    //reactionTable[(uint8_t)pixel::SAND][(uint8_t)pixel::FIRE] = actions::SOLIDIFY;

    reactionTable[(uint8_t)pixel::FIRE][(uint8_t)pixel::FIRE] = actions::FIRETICK;
    reactionTable[(uint8_t)pixel::FIRE][(uint8_t)pixel::AIR] = actions::FIRETICK;
    reactionTable[(uint8_t)pixel::FIRE][(uint8_t)pixel::SMOKE] = actions::FIRETICK;
    reactionTable[(uint8_t)pixel::FIRE][(uint8_t)pixel::STONE] = actions::FIRETICK;
    reactionTable[(uint8_t)pixel::FIRE][(uint8_t)pixel::SAND] = actions::FIRETICK;

    reactionTable[(uint8_t)pixel::FIRE][(uint8_t)pixel::WATER] = actions::EXTINGUISH;

    reactionTable[(uint8_t)pixel::SAND][(uint8_t)pixel::SMOKE] = actions::FALL_DOWN;
    reactionTable[(uint8_t)pixel::SAND][(uint8_t)pixel::AIR] = actions::FALL_DOWN;
    reactionTable[(uint8_t)pixel::SAND][(uint8_t)pixel::FIRE] = actions::FALL_DOWN;

    reactionTable[(uint8_t)pixel::WATER][(uint8_t)pixel::AIR] = actions::FLOW;
    reactionTable[(uint8_t)pixel::WATER][(uint8_t)pixel::SMOKE] = actions::FLOW;

    reactionTable[(uint8_t)pixel::SMOKE][(uint8_t)pixel::AIR] = actions::GO_UP;
    reactionTable[(uint8_t)pixel::SMOKE][(uint8_t)pixel::FIRE] = actions::GO_UP;

    reactionTable[(uint8_t)pixel::SAND][(uint8_t)pixel::WATER] = actions::SINK;

    reactionTable[(uint8_t)pixel::WATER][(uint8_t)pixel::STONE] = actions::ATOP;
    reactionTable[(uint8_t)pixel::WATER][(uint8_t)pixel::SAND] = actions::ATOP;
    reactionTable[(uint8_t)pixel::WATER][(uint8_t)pixel::WATER] = actions::ATOP;
}

void PixelBoard::retBoard (std::ostream &os) {
    const auto& src = flipped ? flipboard : board;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            os << (uint8_t)src[y][x];
        }
        os << '\n';
    }
}

const PixelBoard::pixel PixelBoard::getAt (const uint16_t & y,const uint16_t & x) {
    return flipped ? flipboard[y][x] : board[y][x];
}

void PixelBoard::setAt(const uint16_t & y,const uint16_t & x, const PixelBoard::pixel & toSet){
    auto & dst = flipped ? flipboard : board;
    if (y < height - 1 && x < width - 1 && y > 1 && x > 1)
        dst[y][x] = toSet;
    //std::cout << "drawing at:\nx:" << x << " y: " << y << std::endl;
}

void PixelBoard::drawCube(uint16_t y, uint16_t x, uint8_t size, pixel material){
    if ((y + size) < height && (x + size) < width) {
        for(uint8_t i = 0; i <= size; i++) {
            for (uint8_t j = 0; j <= size; j++) {
                setAt(y + i, x + j, material);
            }
        }
    }
}

void PixelBoard::updateBoard() {
    //std::vector<std::vector<uint8_t>> hasMoved(width, std::vector<uint8_t>(height, false));
    std::vector<std::vector<bool>> hasMoved(height, std::vector<bool>(width, false));
    std::array<int8_t,3> arr = {0,-1, 1};
    std::array<int8_t,3> arrdiag = {0, -1, 1};
    const auto& src = flipped ? flipboard : board;
    auto& dst = flipped ? board : flipboard;

    uint16_t x_begin = flipped ? 0 : width - 1;
    uint16_t x_end   = flipped ? width - 1 : 0;
    int8_t x_step  = flipped ? 1 : -1;

    for (uint16_t y = 1; y < height - 1; y++) {
        for (int16_t x = x_begin; x != x_end; x += x_step) {

            const pixel & curPixel = src[y][x];
            pixel & destinationPixel = dst[y][x];

            if ((!hasMoved[y][x] && curPixel == pixel::AIR) || (!hasMoved[y][x] && curPixel == pixel::STONE)) {
                destinationPixel = curPixel;
                continue;
            }

            //std::shuffle(arrdiag.begin(), arrdiag.end(), lce);
            if (rand() % 2) {
                for (int8_t & i : arrdiag) {
                    i *= -1;
                }
            }

            actions curAction;
            actions lastActiveAction = actions::NONE;
            uint8_t firetick = 0;

            for (int8_t arrX: arr) {
                for (int8_t arrY : arrdiag) {
                    curAction = reactionTable[(uint8_t) curPixel][(uint8_t) src[y + arrX][x + arrY]];

                    if (curAction != actions::NONE && curAction != actions::FALL_DOWN && curAction != actions::GO_UP && curAction != actions::FLOW)
                        lastActiveAction = curAction;

                    if (curAction == actions::FIRETICK) {
                        firetick++;
                        continue;
                    }

                    if (curAction == actions::SINK && rand() % 2)
                        curAction = actions::FALL_DOWN;
                    else if (curAction == actions::SINK)
                        curAction = actions::NONE;

                    if (!hasMoved[y][x] && !hasMoved[y + arrX][x + arrY]) {
                        if ((curAction == actions::FALL_DOWN || curAction == actions::FLOW) && arrX > 0) {
                            destinationPixel = hasMoved[y + arrX][x + arrY] ? dst[y + arrX][x + arrY] : src[y + arrX][x + arrY];
                            dst[y + arrX][x + arrY] = curPixel;
                            lastActiveAction = curAction;
                            hasMoved[y][x] = true;
                            hasMoved[y + arrX][x + arrY] = true;
                            break;
                        } else if (curAction == actions::GO_UP && arrX < 0) {
                            destinationPixel = hasMoved[y + arrX][x + arrY] ? dst[y + arrX][x + arrY] : src[y + arrX][x + arrY];
                            dst[y + arrX][x + arrY] = curPixel;
                            lastActiveAction = curAction;
                            hasMoved[y][x] = true;
                            hasMoved[y + arrX][x + arrY] = true;
                            break;
                        } else if (curAction == actions::FLOW && arrX == 0) {
                            actions atopCheck = reactionTable[(uint8_t)curPixel][(uint8_t)src[y + arrX + 1][x + arrY]];
                            if (atopCheck != actions::ATOP)
                                break;
                            //std::cout << "flowin it" << std::endl;
                            destinationPixel = hasMoved[y + arrX][x + arrY] ? dst[y + arrX][x + arrY] : src[y + arrX][x + arrY];
                            dst[y + arrX][x + arrY] = curPixel;
                            hasMoved[y][x] = true;
                            hasMoved[y + arrX][x + arrY] = true;
                            break;
                        }
                    }
                }
            }

            if (hasMoved[y][x])
                continue;

            switch(lastActiveAction) {
                case actions::BURN:
                    if (rand() % 10 >= 9)
                        destinationPixel = pixel::SMOKE;
                    else
                        destinationPixel = pixel::FIRE;
                    hasMoved[y][x] = true;
                    //continue;
                    break;

                case actions::SOLIDIFY:
                    destinationPixel = pixel::STONE;
                    hasMoved[y][x] = true;
                    //continue;
                    break;

                case actions::EXTINGUISH:
                    dst[y][x] = pixel::AIR;
                    hasMoved[y][x] = true;
                    //continue;
                    break;

                case actions::NONE:
                break;

                default:
                    if (firetick >= 4 && rand() % 10 >= 3 && !hasMoved[y][x] ) {
                        destinationPixel = pixel::AIR;
                        //destinationPixel = pixel::FIRE;
                        hasMoved[y][x] = true;
                        continue;
                    }
            }
            if (!hasMoved[y][x]) {
                destinationPixel = curPixel;
                hasMoved[y][x] = true;
                continue;
           }
        }
    }
    flipped = !flipped;
   }
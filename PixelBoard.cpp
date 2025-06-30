#include <cassert>
#include "PixelBoard.h"
std::vector<std::vector<PixelBoard::actions>> PixelBoard::reactionTable;

PixelBoard::PixelBoard(uint16_t width, uint16_t height) : width (width), height(height), board{width, std::vector<pixel>(height, pixel::AIR)} {
    initReactTable();
    srand(std::time(nullptr));

    for (uint16_t i = 0; i < width; i++) {
        board[i][0] = pixel::STONE;
        board[i][height - 1] = pixel::STONE;
    }
    for (uint16_t j = 0; j < height - 1; j++) {
        board[0][j] = pixel::STONE;
        board[width - 1][j] = pixel::STONE;

    }
    flipboard = board;
}

PixelBoard::PixelBoard(uint16_t width, uint16_t height, pixel basePixel) : width (width), height(height), board{width, std::vector<pixel>(height, basePixel)} {
    initReactTable();
    srand(std::time(nullptr));

    for (uint16_t i = 0; i < width; i++) {
        board[i][0] = pixel::STONE;
        board[i][height - 1] = pixel::STONE;
    }
    for (uint16_t j = 0; j < height - 1; j++) {
        board[0][j] = pixel::STONE;
        board[width - 1][j] = pixel::STONE;

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
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            os << (uint8_t)src[i][j];
        }
        os << '\n';
    }
}

const PixelBoard::pixel PixelBoard::getAt (const uint16_t & x,const uint16_t & y) {
    return flipped ? flipboard[x][y] : board[x][y];
}

void PixelBoard::setAt(const uint16_t & x,const uint16_t & y, const PixelBoard::pixel & toSet){
    auto & dst = flipped ? flipboard : board;
    if (x < height && y < width)
        dst[x][y] = toSet;
    //std::cout << "drawing at:\nx:" << x << " y: " << y << std::endl;
}

void PixelBoard::drawCube(uint16_t x, uint16_t y,uint8_t size, pixel material){
    if ((x + size) < width && (y + size) < height) {
        for(uint8_t i = 0; i <= size; i++) {
            //std::cout << i << std::endl;
            for (uint8_t j = 0; j <= size; j++) {
                setAt(x + i, y + j, material);
                //std::cout << j << std::endl;
            }
        }
    }
    else
        throw("problem");
}

void PixelBoard::updateBoard() {
    //std::vector<std::vector<uint8_t>> hasMoved(width, std::vector<uint8_t>(height, false));
    std::vector<std::vector<bool>> hasMoved(width, std::vector<bool>(height, false));
    std::array<int8_t,3> arr = {0,-1, 1};
    std::array<int8_t,3> arrdiag = {0, -1, 1};
    const auto& src = flipped ? flipboard : board;
    auto& dst = flipped ? board : flipboard;

    uint16_t y_begin = flipped ? 1 : height - 2;
    uint16_t y_end   = flipped ? height - 1 : 0;
    int8_t y_step  = flipped ? 1 : -1;

    //std::shuffle(arr.begin(), arr.end(), lce);

    for (uint16_t x = 1; x < width - 1; x++) {
        //for (uint16_t y = 1; y < height - 1; y++) {
        for (int16_t y = y_begin; y != y_end; y += y_step) {
            const pixel & curPixel = src[x][y];
            pixel & destinationPixel = dst[x][y];

            if ((!hasMoved[x][y] && curPixel == pixel::AIR) || (!hasMoved[x][y] && curPixel == pixel::STONE)) {
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
                    curAction = reactionTable[(uint8_t) curPixel][(uint8_t) src[x + arrX][y + arrY]];

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

                    if (!hasMoved[x][y] &&!hasMoved[x + arrX][y + arrY]) {
                        if ((curAction == actions::FALL_DOWN || curAction == actions::FLOW) && arrX > 0) {
                            destinationPixel = hasMoved[x + arrX][y + arrY] ? dst[x + arrX][y + arrY] : src[x + arrX][y + arrY];
                            dst[x + arrX][y + arrY] = curPixel;
                            lastActiveAction = curAction;
                            hasMoved[x][y] = true;
                            hasMoved[x + arrX][y + arrY] = true;
                            break;
                        } else if (curAction == actions::GO_UP && arrX < 0) {
                            destinationPixel = hasMoved[x + arrX][y + arrY] ? dst[x + arrX][y + arrY] : src[x + arrX][y + arrY];
                            dst[x + arrX][y + arrY] = curPixel;
                            lastActiveAction = curAction;
                            hasMoved[x][y] = true;
                            hasMoved[x + arrX][y + arrY] = true;
                            break;
                        }

                        else if (curAction == actions::FLOW && arrX == 0) {
                            actions atopCheck = reactionTable[(uint8_t)curPixel][(uint8_t)src[x + arrX + 1][y + arrY]];
                            if (atopCheck != actions::ATOP)
                                break;
                            //std::cout << "flowin it" << std::endl;
                            destinationPixel = hasMoved[x + arrX][y + arrY] ? dst[x + arrX][y + arrY] : src[x + arrX][y + arrY];
                            dst[x + arrX][y + arrY] = curPixel;
                            hasMoved[x][y] = true;
                            hasMoved[x + arrX][y + arrY] = true;
                            break;
                        }
                    }
                }
            }

            if (hasMoved[x][y])
                continue;

            switch(lastActiveAction) {
                case actions::BURN:
                    if (rand() % 10 >= 9)
                        destinationPixel = pixel::SMOKE;
                    else
                        destinationPixel = pixel::FIRE;
                    hasMoved[x][y] = true;
                    //continue;
                    break;

                case actions::SOLIDIFY:
                    destinationPixel = pixel::STONE;
                    hasMoved[x][y] = true;
                    //continue;
                    break;

                case actions::EXTINGUISH:
                    dst[x][y] = pixel::AIR;
                    hasMoved[x][y] = true;
                    //continue;
                    break;

                case actions::NONE:
                break;

                default:
                    if (firetick >= 4 && rand() % 10 >= 3 && !hasMoved[x][y] ) {
                        destinationPixel = pixel::AIR;
                        //destinationPixel = pixel::FIRE;
                        hasMoved[x][y] = true;
                        continue;
                    }
            }
            if (!hasMoved[x][y]) {
                destinationPixel = curPixel;
                hasMoved[x][y] = true;
                continue;
           }
        }
    }
    flipped = !flipped;
}
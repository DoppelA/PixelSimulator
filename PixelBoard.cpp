#include "PixelBoard.h"

PixelBoard::PixelBoard(uint16_t height, uint16_t width) : height(height), width (width) {
    std::vector<std::vector<pixel>> tempBoard(width, std::vector<pixel>(height, pixel::AIR));
    board = tempBoard;
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

PixelBoard::PixelBoard(uint16_t width, uint16_t height, pixel basePixel) : width (width), height(height) {
    std::vector<std::vector<pixel>> tempBoard(width, std::vector<pixel>(height, basePixel));
    board = tempBoard;
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
    std::vector<std::vector<actions>> tempActions((int)pixel::NUM_TYPES, std::vector<actions>((int)pixel::NUM_TYPES, actions::NONE));
    reactionTable = tempActions;
    reactionTable[(int)pixel::WOOD][(int)pixel::FIRE] = actions::BURN;
    reactionTable[(int)pixel::WATER][(int)pixel::SMOKE] = actions::FLOW;
    reactionTable[(int)pixel::FIRE][(int)pixel::WATER] = actions::TURNSMOKE;
    //reactionTable[(int)pixel::SAND][(int)pixel::FIRE] = actions::SOLIDIFY;
    reactionTable[(int)pixel::FIRE][(int)pixel::FIRE] = actions::FIRETICK;
    reactionTable[(int)pixel::FIRE][(int)pixel::AIR] = actions::FIRETICK;
    reactionTable[(int)pixel::FIRE][(int)pixel::SMOKE] = actions::FIRETICK;
    reactionTable[(int)pixel::FIRE][(int)pixel::STONE] = actions::FIRETICK;
    reactionTable[(int)pixel::FIRE][(int)pixel::SAND] = actions::FIRETICK;
    reactionTable[(int)pixel::FIRE][(int)pixel::WATER] = actions::EXTINGUISH;
    reactionTable[(int)pixel::SAND][(int)pixel::SMOKE] = actions::FALL_DOWN;
    reactionTable[(int)pixel::SAND][(int)pixel::AIR] = actions::FALL_DOWN;
    reactionTable[(int)pixel::SAND][(int)pixel::FIRE] = actions::FALL_DOWN;
    reactionTable[(int)pixel::WATER][(int)pixel::AIR] = actions::FALL_DOWN;
    reactionTable[(int)pixel::SMOKE][(int)pixel::AIR] = actions::GO_UP;
    reactionTable[(int)pixel::SMOKE][(int)pixel::FIRE] = actions::GO_UP;
}

void PixelBoard::retBoard (std::ostream &os) {
    const auto& src = flipped ? flipboard : board;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            os << (int)src[i][j];
        }
        os << '\n';
    }
}

const PixelBoard::pixel PixelBoard::getAt (const uint16_t & x,const uint16_t & y) {
    return flipped ? flipboard[x][y] : board[x][y];
}

void PixelBoard::setAt(const uint16_t & x,const uint16_t & y, const PixelBoard::pixel & toSet){
    auto & dst = flipped ? flipboard : board;
    dst[x][y] = toSet;
}

void PixelBoard::updateBoard() {
    std::vector<std::vector<bool>> hasMoved(width, std::vector<bool>(height, false));
    const auto& src = flipped ? flipboard : board;
    auto& dst = flipped ? board : flipboard;
    std::array<int8_t,2> arr = {1,-1};

    for (uint16_t i = 1; i < width - 1; i++) {
        for (uint16_t j = 1; j < height - 1; j++) {
            const pixel & curPixel = src[i][j];
            if (curPixel != PixelBoard::pixel::AIR) {
                actions curAction;
                actions lastActiveAction = actions::NONE;
                uint8_t firetick = 0;
                for (int8_t ddy: arr) {
                    curAction = reactionTable[(int) curPixel][(int) src[i + ddy][j]];

                    if (curAction != actions::NONE && curAction != actions::FALL_DOWN && curAction != actions::GO_UP)
                        lastActiveAction = curAction;

                    if (curAction == actions::FIRETICK)
                        firetick++;

                    if (curAction == actions::FALL_DOWN && ddy > 0 && !hasMoved[i][j]) {
                        dst[i][j] = hasMoved[i + ddy][j] ? dst[i + ddy][j] : src[i + ddy][j];
                        dst[i + ddy][j] =  src[i][j];
                        lastActiveAction = actions::FALL_DOWN;
                        hasMoved[i][j] = true;
                        hasMoved[i + ddy][j] = true;
                        break;
                    }

                    if (curAction == actions::GO_UP && ddy < 0 && !hasMoved[i][j]) {
                        dst[i][j] = hasMoved[i + ddy][j] ? dst[i + ddy][j] : src[i + ddy][j];
                        dst[i + ddy][j] = src[i][j];
                        lastActiveAction = actions::GO_UP;
                        hasMoved[i][j] = true;
                        hasMoved[i + ddy][j] = true;
                        break;
                    }
                }

                for (int8_t ddx: arr) {
                    curAction = reactionTable[(int) curPixel][(int) src[i][j + ddx]];

                    if (curAction != actions::NONE && curAction != actions::FALL_DOWN && curAction != actions::GO_UP)
                        lastActiveAction = curAction;

                    if (curAction == actions::FIRETICK) firetick++;
                }
                if (hasMoved[i][j])
                    continue;

                switch(lastActiveAction) {
                    case actions::BURN:
                        if (rand() % 10 >= 8)
                            dst[i][j] = pixel::SMOKE;
                        else
                            dst[i][j] = pixel::FIRE;
                        hasMoved[i][j] = true;
                        //continue;
                        break;

                    case actions::SOLIDIFY:
                        dst[i][j] = pixel::STONE;
                        hasMoved[i][j] = true;
                        //continue;
                        break;

                    case actions::EXTINGUISH:
                        dst[i][j] = pixel::AIR;
                        hasMoved[i][j] = true;
                        //continue;
                        break;

                    case actions::NONE:
                        for (int8_t dxy: arr) {
                            curAction = reactionTable[(int) curPixel][(int) src[i + dxy][j + dxy]];
                            if (curAction == actions::FALL_DOWN && dxy > 0 && !hasMoved[i][j]) {
                                dst[i][j] = hasMoved[i + dxy][j + dxy] ? dst[i + dxy][j + dxy] : src[i + dxy][j + dxy];
                                dst[i + dxy][j + dxy] = src[i][j];
                                hasMoved[i][j] = true;
                                hasMoved[i + dxy][j + dxy] = true;
                                break;
                            }
                            if (curAction == actions::GO_UP && dxy < 0 && !hasMoved[i][j]) {
                                dst[i][j] = hasMoved[i + dxy][j + dxy] ? dst[i + dxy][j + dxy] : src[i + dxy][j + dxy];
                                dst[i + dxy][j + dxy] = src[i][j];
                                hasMoved[i][j] = true;
                                hasMoved[i + dxy][j + dxy] = true;
                                break;
                            }
                            curAction = reactionTable[(int) curPixel][(int) src[i + (dxy * -1)][j + dxy]];
                            if (curAction == actions::FALL_DOWN && dxy < 0 && !hasMoved[i][j]) {
                                dst[i][j] = hasMoved[i + (dxy * -1)][j + dxy] ? dst[i + (dxy * -1)][j + dxy] : src[i + (dxy * -1)][j + dxy];
                                dst[i + (dxy * -1)][j + dxy] = src[i][j];
                                hasMoved[i][j] = true;
                                hasMoved[i + (dxy * -1)][j + dxy] = true;
                                break;
                            }
                            if (curAction == actions::GO_UP && dxy > 0 && !hasMoved[i][j]) {
                                dst[i][j] = hasMoved[i + (dxy * -1)][j + dxy] ? dst[i + (dxy * -1)][j + dxy] : src[i + (dxy * -1)][j + dxy];
                                dst[i + (dxy * -1)][j + dxy] = src[i][j];
                                hasMoved[i][j] = true;
                                hasMoved[i + (dxy * -1)][j + dxy] = true;
                                break;
                            }
                        }

                    default:
                        if (firetick >= 4 && rand() % 10 >= 3 && !hasMoved[i][j]) {
                            dst[i][j] = pixel::AIR;
                            //dst[i][j] = pixel::FIRE;
                            hasMoved[i][j] = true;
                            continue;
                        }
                }
                if (!hasMoved[i][j]) {
                    dst[i][j] = src[i][j];
                    hasMoved[i][j] = true;
                    continue;
                }
            }
            else
                dst[i][j] = src[i][j];
        }
    }
    flipped = !flipped;
}
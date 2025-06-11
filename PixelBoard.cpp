#include "PixelBoard.h"

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
    if (reactionTable.size() > 0)
        return;

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
    reactionTable[(int)pixel::WATER][(int)pixel::SMOKE] = actions::FALL_DOWN;
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
    std::vector<std::vector<bool>> hasMoved(width, std::vector<bool>(height, false));
    const auto& src = flipped ? flipboard : board;
    auto& dst = flipped ? board : flipboard;
    std::array<int8_t,2> arr = {1,-1};

    for (uint16_t x = 1; x < width - 1; x++) {
        for (uint16_t y = 1; y < height - 1; y++) {
            const pixel & curPixel = src[x][y];
            pixel & targetPixel = dst[x][y];
            if (curPixel != pixel::AIR && curPixel != pixel::STONE) {
                actions curAction;
                actions lastActiveAction = actions::NONE;
                uint8_t firetick = 0;
                for (int8_t arrX: arr) {
                    curAction = reactionTable[(int) curPixel][(int) src[x + arrX][y]];

                    if (curAction != actions::NONE && curAction != actions::FALL_DOWN && curAction != actions::GO_UP)
                        lastActiveAction = curAction;

                    if (curAction == actions::FIRETICK)
                        firetick++;

                    if (curAction == actions::FALL_DOWN && arrX > 0 && !hasMoved[x][y]) {
                        targetPixel = hasMoved[x + arrX][y] ? dst[x + arrX][y] : src[x + arrX][y];
                        dst[x + arrX][y] = curPixel;
                        lastActiveAction = actions::FALL_DOWN;
                        hasMoved[x][y] = true;
                        hasMoved[x + arrX][y] = true;
                        break;
                    }

                    if (curAction == actions::GO_UP && arrX < 0 && !hasMoved[x][y]) {
                        targetPixel = hasMoved[x + arrX][y] ? dst[x + arrX][y] : src[x + arrX][y];
                        dst[x + arrX][y] = curPixel;
                        lastActiveAction = actions::GO_UP;
                        hasMoved[x][y] = true;
                        hasMoved[x + arrX][y] = true;
                        break;
                    }
                }

                for (int8_t arrY: arr) {
                    curAction = reactionTable[(int) curPixel][(int) src[x][y + arrY]];

                    if (curAction != actions::NONE && curAction != actions::FALL_DOWN && curAction != actions::GO_UP)
                        lastActiveAction = curAction;

                    if (curAction == actions::FIRETICK) firetick++;
                }

                if (hasMoved[x][y])
                    continue;

                switch(lastActiveAction) {
                    case actions::BURN:
                        if (rand() % 10 >= 8)
                            //targetPixel = (rand() % 10 >= 8) ? pixel::SMOKE : pixel::AIR;
                            targetPixel = pixel::SMOKE;
                        else
                            targetPixel = pixel::FIRE;
                        hasMoved[x][y] = true;
                        //continue;
                        break;

                    case actions::SOLIDIFY:
                        targetPixel = pixel::STONE;
                        hasMoved[x][y] = true;
                        //continue;
                        break;

                    case actions::EXTINGUISH:
                        dst[x][y] = pixel::AIR;
                        hasMoved[x][y] = true;
                        //continue;
                        break;

                    case actions::NONE:
                        if (curPixel == pixel::WATER || curPixel == pixel::SMOKE || curPixel == pixel::SAND)
                            for (int8_t dxy: arr) {
                                curAction = reactionTable[(int) curPixel][(int) src[x + dxy][y + dxy]];
                                pixel & tlbrDiagonalTarget = dst[x + dxy][y + dxy];
                                pixel & tlbrDiagonalSource = const_cast<pixel &>(src[x + dxy][y + dxy]);

                                if (curAction == actions::FALL_DOWN && dxy > 0 && !hasMoved[x][y]) {
                                    //targetPixel = hasMoved[i + dxy][j + dxy] ? tlbrDiagonalTarget : tlbrDiagonalSource;
                                    targetPixel = tlbrDiagonalSource;
                                    tlbrDiagonalTarget = curPixel;
                                    hasMoved[x][y] = true;
                                    hasMoved[x + dxy][y + dxy] = true;
                                    break;
                                }
                                if (curAction == actions::GO_UP && dxy < 0 && !hasMoved[x][y]) {
                                    //targetPixel = hasMoved[i + dxy][j + dxy] ? tlbrDiagonalTarget : tlbrDiagonalSource;
                                    targetPixel = tlbrDiagonalSource;
                                    tlbrDiagonalTarget = curPixel;
                                    hasMoved[x][y] = true;
                                    hasMoved[x + dxy][y + dxy] = true;
                                    break;
                                }

                                curAction = reactionTable[(int) curPixel][(int) src[x + (dxy * -1)][y + dxy]];
                                pixel & trblDiagonalTarget = dst[x + (dxy * -1)][y + dxy];
                                pixel & trblDiagonalSource = const_cast<pixel &>(src[x + (dxy * -1)][y + dxy]);

                                if (curAction == actions::FALL_DOWN && dxy < 0 && !hasMoved[x][y]) {
                                    //targetPixel = hasMoved[i + (dxy * -1)][j + dxy] ? trblDiagonalTarget : trblDiagonalSource;
                                    targetPixel = trblDiagonalSource;
                                    trblDiagonalTarget = curPixel;
                                    hasMoved[x][y] = true;
                                    hasMoved[x + (dxy * -1)][y + dxy] = true;
                                    break;
                                }
                                if (curAction == actions::GO_UP && dxy > 0 && !hasMoved[x][y]) {
                                    //targetPixel = hasMoved[i + (dxy * -1)][j + dxy] ? trblDiagonalTarget : trblDiagonalSource;
                                    targetPixel = trblDiagonalSource;
                                    trblDiagonalTarget = curPixel;
                                    hasMoved[x][y] = true;
                                    hasMoved[x + (dxy * -1)][y + dxy] = true;
                                    break;
                                }
                            }
                        break;

                    default:
                        if (firetick >= 4 && rand() % 10 >= 3 && !hasMoved[x][y]) {
                            targetPixel = pixel::AIR;
                            //targetPixel = pixel::FIRE;
                            hasMoved[x][y] = true;
                            continue;
                        }
                }
                if (!hasMoved[x][y]) {
                    targetPixel = curPixel;
                    hasMoved[x][y] = true;
                    continue;
                }
            }
            else {
                targetPixel = curPixel;
            }
        }
    }
    flipped = !flipped;
}
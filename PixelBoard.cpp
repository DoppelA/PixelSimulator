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
    reactionTable[(int)pixel::SAND][(int)pixel::WATER] = actions::SINK;
    //reactionTable[(int)pixel::SAND][(int)pixel::WATER] = actions::FALL_DOWN;
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
    //std::vector<std::vector<uint8_t>> hasMoved(width, std::vector<uint8_t>(height, false));
    std::vector<std::vector<bool>> hasMoved(width, std::vector<bool>(height, false));
    std::array<int8_t,3> arr = {-1, 0, 1};
    std::array<int8_t,3> arrdiag = {-1, 0, 1};
    const auto& src = flipped ? flipboard : board;
    auto& dst = flipped ? board : flipboard;

    uint16_t y_begin = flipped ? 1 : height - 2;
    uint16_t y_end   = flipped ? height - 1 : 0;
    int8_t y_step  = flipped ? 1 : -1;

    //std::shuffle(arrdiag.begin(), arrdiag.end(), lce);
    std::shuffle(arr.begin(), arr.end(), lce);

    for (uint16_t x = 1; x < width - 1; x++) {
        //for (uint16_t y = 1; y < height - 1; y++) {
        for (int16_t y = y_begin; y != y_end; y += y_step) {
            const pixel & curPixel = src[x][y];
            pixel & destinationPixel = dst[x][y];

            if ((!hasMoved[x][y] && curPixel == pixel::AIR) || (!hasMoved[x][y] && curPixel == pixel::STONE)) {
                destinationPixel = curPixel;
                continue;
            }

            std::shuffle(arrdiag.begin(), arrdiag.end(), lce);
            //std::shuffle(arr.begin(), arr.end(), lce);
            actions curAction;
            actions lastActiveAction = actions::NONE;
            uint8_t firetick = 0;
           // std::vector<bool> pos(8,false);

            for (int8_t arrX: arr) {
                for (int8_t arrY : arrdiag) {
                    curAction = reactionTable[(int) curPixel][(int) src[x + arrX][y + arrY]];

                    if (curAction != actions::NONE && curAction != actions::FALL_DOWN && curAction != actions::GO_UP)
                        lastActiveAction = curAction;

                    if (curAction == actions::FIRETICK)
                        firetick++;

                    if (curAction == actions::SINK && rand() % 2)
                        curAction = actions::FALL_DOWN;
                    else if (curAction == actions::SINK)
                        curAction = actions::NONE;

                    if (curAction == actions::FALL_DOWN && arrX > 0 && !hasMoved[x][y] && !hasMoved[x + arrX][y + arrY]) {
                        destinationPixel = hasMoved[x + arrX][y + arrY] ? dst[x + arrX][y + arrY] : src[x + arrX][y + arrY];
                        dst[x + arrX][y + arrY] = curPixel;
                        lastActiveAction = curAction;
                        hasMoved[x][y] = true;
                        hasMoved[x + arrX][y + arrY] = true;
                        break;
                    }
                    else if (curAction == actions::GO_UP && arrX < 0 && !hasMoved[x][y] && !hasMoved[x + arrX][y + arrY]) {
                        destinationPixel = hasMoved[x + arrX][y + arrY] ? dst[x + arrX][y + arrY] : src[x + arrX][y + arrY];
                        dst[x + arrX][y + arrY] = curPixel;
                        lastActiveAction = curAction;
                        hasMoved[x][y] = true;
                        hasMoved[x + arrX][y + arrY] = true;
                        break;
                    }
                }
            }



            if (hasMoved[x][y])
                continue;

            switch(lastActiveAction) {
                case actions::BURN:
                    if (rand() % 10 >= 9)
                        //destinationPixel = (rand() % 10 >= 8) ? pixel::SMOKE : pixel::AIR;
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
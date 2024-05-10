/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
*/

#pragma once

#include <stdbool.h>

#include "consts.h"

// Structure used for storing moves
// Here is how a white move Rbxe6 would look
/*
    {
        0,
        ROOK_INDEX,
        BITFILE_B,
        BITFILE_E & BITRANK_6,
        true,
        0,
    }
*/ 
struct BitMove {
    // 0 -> white, 1 -> black
    bool movingSide;
    int movedPiece;
    unsigned long long org;
    unsigned long long dest;
    bool capture;
    // 0 if false, otherwise index of piece promoted to
    int promotion;
};
typedef struct BitMove BitMove;

static const BitMove W_SHORT_CASTLE = (BitMove){0, SHORT_CASTLE_INDEX, 0, 0, 0, 0};
static const BitMove W_LONG_CASTLE = (BitMove){0, LONG_CASTLE_INDEX, 0, 0, 0, 0};
static const BitMove B_SHORT_CASTLE = (BitMove){1, SHORT_CASTLE_INDEX, 0, 0, 0, 0};
static const BitMove B_LONG_CASTLE = (BitMove){1, LONG_CASTLE_INDEX, 0, 0, 0, 0};

BitMove translateAlgebraic(char* algebraicMove, bool movingSide);
unsigned long long filterCandidates(unsigned long long cand, unsigned long long king, BitMove *bitMove, Board *board);
unsigned long long extractOrigin(BitMove bitMove, Board board);
unsigned long long expandBetweenMask(unsigned long long piece, unsigned long long king, bool *pinRayType);
Board applyBitMove(BitMove bitMove, Board board);
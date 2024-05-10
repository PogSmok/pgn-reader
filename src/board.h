/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
*/

#pragma once

#include <stdbool.h>

#include "consts.h"

//This is the datatype in which piece locations of a position are stored
struct Board {
    //pieces[0] -> pawns, pieces[1] -> knights, pieces[2] -> bishops, pieces[3] -> rooks, pieces[4] -> queens, pieces[5] -> kings
    unsigned long long white[PIECE_TYPE_COUNT];
    unsigned long long black[PIECE_TYPE_COUNT];
    //false -> white to move, true -> black to move
    bool turn;
};
typedef struct Board Board;

static const Board EMPTY_BOARD = {
    {
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000,
    },
    {
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000000000000,
    },
    false,
};

//Default starting position of a chess game (rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1)
static const Board STARTING_BOARD = {
    {
        0x000000000000FF00,
        0x0000000000000042,
        0x0000000000000024,
        0x0000000000000081,
        0x0000000000000008,
        0x0000000000000010,
    },
    {
        0x00FF000000000000,
        0x4200000000000000,
        0x2400000000000000,
        0x8100000000000000,
        0x0800000000000000,
        0x1000000000000000,
    },
    false,
};

bool isBoardNull(Board board);
unsigned long long allPieces(Board board);
int bitSquareToIndex(unsigned long long bitSquare);
Board fenToBoard(char* fen);
/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "consts.h"
#include "bitconsts.h"
#include "board.h"

bool isBoardNull(Board board) {
    return !(board.white[KING_INDEX] | board.black[KING_INDEX]);
}

// Returns all occupied squares
unsigned long long allPieces(Board board) {
    return board.white[PAWN_INDEX] | board.white[KNIGHT_INDEX] | board.white[BISHOP_INDEX] | board.white[ROOK_INDEX] | board.white[QUEEN_INDEX] | board.white[KING_INDEX] | board.black[PAWN_INDEX] | board.black[KNIGHT_INDEX] | board.black[BISHOP_INDEX] | board.black[ROOK_INDEX] | board.black[QUEEN_INDEX] | board.black[KING_INDEX];
}

//Returns index of '1' (there always is one and only one bit set to '1') in a bit representation of a square
int bitSquareToIndex(unsigned long long bitSquare) {
    unsigned long long i = 0, it = 1;
    while(!(bitSquare & it)) it<<=1, i++;
    return i;
}

//Translates FEN into Board struct
Board fenToBoard(char* fen) {
    //FEN begins from A8 square
    int currentRankIndex = 7;
    unsigned long long currentBitSquare = BITFILE_A & ALL_BITRANKS[currentRankIndex];
    Board board = EMPTY_BOARD;
    char* i;
    for(i = fen; *i != '\0'; i++) {
        if(*i == ' ') break;
        if(*i == '/') currentBitSquare = BITFILE_A & ALL_BITRANKS[--currentRankIndex];
        else if(*i <= '8' && *i >= '1') currentBitSquare<<=(*i - '0');
        else {
            if(PIECE_INDEX(*i) == -1) {
                printf(" FEN provided as a starting position of the game (\"%s\") is not a valid FEN.\n", fen);
                exit(-1);
            }
            *i < 'Z' ? (board.white[PIECE_INDEX(*i)] ^= currentBitSquare) : (board.black[PIECE_INDEX(*i)] ^= currentBitSquare);
            currentBitSquare<<=1;
        }
    }

    if(*(i+1) == 'b') board.turn = true;
    return board;
}
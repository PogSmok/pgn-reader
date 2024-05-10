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
#include "bitmove.h"

// Converts algebraic notation move to BitMove
BitMove translateAlgebraic(char* c, bool movingSide) {
    // Castling moves are handles separately and have their special represenations 
    if(*c == 'O') {
        if(*(c+3) == '-') return movingSide ? B_LONG_CASTLE : W_LONG_CASTLE;
        return movingSide ? B_SHORT_CASTLE : W_SHORT_CASTLE;
    }

    //Those arrays store indexes to ALL_BITFILES and ALL_BITRANKS, -1 means move contains no data about origin or destination
    int orgIndex[2] = {-1, -1}, destIndex[2] = {-1, -1};
    bool capture = false;
    int movedPiece = 0, promotedPiece = 0;

    // If condition is met it is a piece move, otherwise a pawn is moved
    if(*c > 'A' && *c < 'Z') movedPiece = PIECE_INDEX(*c);
    else {
        movedPiece = 0;
        orgIndex[0] = *(c--)-'a';
    }

    for(char* i = c+1; *i != '\0'; i++) {
        if(*i >= 'a' && *i <= 'h') {
            if(destIndex[0] != -1) orgIndex[0] = destIndex[0];
            destIndex[0] = *i-'a';
        } else if(*i >= '1' && *i <= '8') {
            if(destIndex[1] != -1) orgIndex[1] = destIndex[1];
            destIndex[1] = *i-'1';
        } else if(*i == 'x') {
            capture = true;
            if(destIndex[0] != -1) orgIndex[0] = destIndex[0];
            if(destIndex[1] != -1) orgIndex[1] = destIndex[1];
            destIndex[0] = *(++i)-'a';
            destIndex[1] = *(++i)-'1';
        } else if(*i == '=') promotedPiece = PIECE_INDEX(*(++i));
        else continue;	
    }

    //Both values for destination are always known
    if(destIndex[0] == -1 || destIndex[1] == -1) {
        printf(" There is an invalid, uninterpretable move (%s) in the PGN file.", c);
        exit(-1);
    }

    unsigned long long org = WHOLE_BOARD, dest = WHOLE_BOARD & ALL_BITFILES[destIndex[0]] & ALL_BITRANKS[destIndex[1]];
    if(orgIndex[0] != -1) org &= ALL_BITFILES[orgIndex[0]];
    if(orgIndex[1] != -1) org &= ALL_BITRANKS[orgIndex[1]];

    return (BitMove){movingSide, movedPiece, org, dest, capture, promotedPiece};
}

unsigned long long filterCandidates(unsigned long long cand, unsigned long long king, BitMove *bitMove, Board *board) {
    unsigned long long end = MSB(cand), it = LSB(cand), pieceToCheck, directedPinRay, pinRay;
    unsigned long long (*enemy)[PIECE_TYPE_COUNT] = (*bitMove).movingSide ? &(*board).white : &(*board).black;
    while(it != end) {
        int kingIndex = bitSquareToIndex(king), itIndex = bitSquareToIndex(it);
        bool pinRayType;
        //The path is not blocked by a piece
        if((BETWEEN_MASK[itIndex][bitSquareToIndex((*bitMove).dest)] & allPieces(*board)) == 0) {
            pinRayType = false;
            pinRay = expandBetweenMask(it, king, &pinRayType);
            //The origin is legal, return it
            if(pinRay == 0) return it;
            
            //Direct the pinray
            if(kingIndex>>3 == itIndex>>3) 
                directedPinRay = king > it ? pinRay & BITFILES_TO[(kingIndex&7)-1] : pinRay & (WHOLE_BOARD ^ BITFILES_TO[kingIndex&7]);
            else 
                directedPinRay = king > it ? pinRay & BITRANKS_TO[(kingIndex>>3)-1] : pinRay & (WHOLE_BOARD ^ BITRANKS_TO[kingIndex>>3]);
            
            //Moves along pinray, thus is a legal move
            if(pinRay & (*bitMove).dest) return it;

            //Check if the piece is actually pinned, that is there are no pieces between the pinning piece and pinned piece
            if(LSB(directedPinRay) > LSB(pinRay)) pieceToCheck = LSB(directedPinRay);
            else pieceToCheck = MSB(directedPinRay);
            
            if(pinRayType)
            if((((*enemy)[BISHOP_INDEX] | (*enemy)[QUEEN_INDEX]) & pieceToCheck) == 0) return it;
            else
            if((((*enemy)[ROOK_INDEX] | (*enemy)[QUEEN_INDEX]) & pieceToCheck) == 0) return it;
        }
        cand ^= it;
        while(!(it&cand)) it<<=1;
    }
    return end;
}

// Deducts origin of a move, basing on the position in which it was played
unsigned long long extractOrigin(BitMove bitMove, Board board) {
    //cand holds all possible origins, which are then filtered
    unsigned long long (*our)[PIECE_TYPE_COUNT] = bitMove.movingSide ? &board.black : &board.white, cand;
    
    switch(bitMove.movedPiece) {
        case PAWN_INDEX: 
            cand = bitMove.org & (*our)[PAWN_INDEX];
            unsigned long long moveMask = (bitMove.dest >> 8);
            if(!bitMove.movingSide && bitMove.dest & BITRANK_4) moveMask |= bitMove.dest >> 16;

            if(bitMove.movingSide) moveMask = (bitMove.dest << 8);
            if(bitMove.movingSide && bitMove.dest & BITRANK_5) moveMask |= bitMove.dest << 16;

            if(bitMove.capture) moveMask = bitMove.movingSide ? WHITE_PAWN_ATTACK_MASK[bitSquareToIndex(bitMove.dest)] : BLACK_PAWN_ATTACK_MASK[bitSquareToIndex(bitMove.dest)];
            cand = moveMask&cand;
            if(LSB(cand) == cand) return cand;
            return filterCandidates(cand, (*our)[KING_INDEX], &bitMove, &board);

        case KNIGHT_INDEX:
            cand = bitMove.org & (*our)[KNIGHT_INDEX] & KNIGHT_ATTACK_MASK[bitSquareToIndex(bitMove.dest)];
            if(LSB(cand) == cand) return cand;
            return filterCandidates(cand, (*our)[KING_INDEX], &bitMove, &board);

        case BISHOP_INDEX:
            cand = bitMove.org & (*our)[BISHOP_INDEX] & BISHOP_ATTACK_MASK[bitSquareToIndex(bitMove.dest)];
            if(LSB(cand) == cand) return cand;
            return filterCandidates(cand, (*our)[KING_INDEX], &bitMove, &board);

        case ROOK_INDEX:
            cand = bitMove.org & (*our)[ROOK_INDEX] & ROOK_ATTACK_MASK[bitSquareToIndex(bitMove.dest)];
            if(LSB(cand) == cand) return cand;
            return filterCandidates(cand, (*our)[KING_INDEX], &bitMove, &board);

        case QUEEN_INDEX: 
            cand = bitMove.org & (*our)[QUEEN_INDEX] & (BISHOP_ATTACK_MASK[bitSquareToIndex(bitMove.dest)] | ROOK_ATTACK_MASK[bitSquareToIndex(bitMove.dest)]);
            if(LSB(cand) == cand) return cand;
            return filterCandidates(cand, (*our)[KING_INDEX], &bitMove, &board);

        default:
            return bitMove.org & (*our)[KING_INDEX] & KING_ATTACK_MASK[bitSquareToIndex(bitMove.dest)];
    }
}

// Expands between mask so that it reaches both ends of the board
// Returns 0 if no such ray exists
// pinRayType = false -> horizontal or vertical, pinRayType = true -> diagonal
unsigned long long expandBetweenMask(unsigned long long piece, unsigned long long king, bool *pinRayType) {
    int pieceIndex = bitSquareToIndex(piece), kingIndex = bitSquareToIndex(king);
    if(!BETWEEN_MASK[pieceIndex][kingIndex]) return 0;

    //Vertical rook pin (For example A1-H1)
    if(pieceIndex&7 == kingIndex&7) return ALL_BITFILES[pieceIndex&7];
    //Horizontal rook pin (For example A1-A8)
    else if(pieceIndex>>3 == kingIndex>>3) return ALL_BITRANKS[pieceIndex>>3];

    *pinRayType = true;
    if(ALL_L_DIAG[(pieceIndex&7) + (pieceIndex>>3)] & ALL_L_DIAG[(kingIndex&7) + (kingIndex>>3)]) return ALL_L_DIAG[(pieceIndex&7) + (pieceIndex>>3)];
    return ALL_R_DIAG[((pieceIndex&7)-(pieceIndex>>3)+DIAGONAL_COUNT)%DIAGONAL_COUNT];
}

// Applies a move to a board, returns a new board with position after the move is made
Board applyBitMove(BitMove bitMove, Board board) {
    unsigned long long (*our)[PIECE_TYPE_COUNT] = bitMove.movingSide ? &board.black : &board.white;
    unsigned long long (*their)[PIECE_TYPE_COUNT] = bitMove.movingSide ? &board.white : &board.black;
    board.turn = !bitMove.movingSide;

    if(bitMove.movedPiece == SHORT_CASTLE_INDEX) {
        if(bitMove.movingSide) {
            (*our)[KING_INDEX] = BITFILE_G & BITRANK_8;
            (*our)[ROOK_INDEX] ^= MSB((*our)[ROOK_INDEX]) ^ (BITFILE_F & BITRANK_8);
        } else {
            (*our)[KING_INDEX] = BITFILE_G & BITRANK_1;
            (*our)[ROOK_INDEX] ^= MSB((*our)[ROOK_INDEX]) ^ (BITFILE_F & BITRANK_1);
        }
        return board;
    }
    if(bitMove.movedPiece == LONG_CASTLE_INDEX) {
        if(bitMove.movingSide) {
            (*our)[KING_INDEX] = BITFILE_C & BITRANK_8;
            (*our)[ROOK_INDEX] = (*our)[ROOK_INDEX] ^ LSB((*our)[ROOK_INDEX]) ^ (BITFILE_D & BITRANK_8);
        } else {
            (*our)[KING_INDEX] = BITFILE_C & BITRANK_1;
            (*our)[ROOK_INDEX] = (*our)[ROOK_INDEX] ^ LSB((*our)[ROOK_INDEX]) ^ (BITFILE_D & BITRANK_1);
        }
        return board;
    }

    //Remove moved piece from original square
    (*our)[bitMove.movedPiece] ^= extractOrigin(bitMove, board);

    //Remove captured piece from the board
    if(bitMove.capture) {
        int i;
        for(i = 0; i < PIECE_TYPE_COUNT; i++){
            if((*their)[i] & bitMove.dest){
                (*their)[i] ^= bitMove.dest;
                break;
            }
        }

        //en-passant!
        if(i == PIECE_TYPE_COUNT) {
            if(bitMove.movingSide) (*their)[PAWN_INDEX] ^= bitMove.dest << 8;
            else (*their)[PAWN_INDEX] ^= bitMove.dest >> 8;
        }
    }

    if(bitMove.promotion) {
        (*our)[bitMove.promotion] ^= bitMove.dest;
        return board;
    }

    //Place back the piece on its destination
    (*our)[bitMove.movedPiece] ^= bitMove.dest;
    return board;
}
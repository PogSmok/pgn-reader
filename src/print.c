/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
*/

#include <stdio.h>
#include <string.h>

#include "consts.h"
#include "bitconsts.h"
#include "board.h"
#include "print.h"

void printAbout() {
    printf("%s VERSION: %s\n\n\n", PGN_READER_ASCII_ART, VERSION);
}

void clearScreen() {
    for(int i = 0; i < 128; i++) printf("\n");
}

void tabulate(char** c, int tabAmount) {
    if(tabAmount == 0) {
        **c = ' ', ++*c;
        return;
    }
    for(int spaces = 0; spaces < AMOUNT_OF_SPACES_IN_TAB*tabAmount; spaces++, ++*c) **c = ' ';
}

void printTags(char (*tagContent)[MAX_TAG_VALUE_LENGTH]) {
    if(tagContent[11][0]) printf(" %s\n", tagContent[11]);
    if(tagContent[15][0]) {
        printf(" %s", tagContent[15]);
        tagContent[16][0] ? printf(" Round: %s\n", tagContent[16]) : printf("\n");
    }

    if(tagContent[0][0]) {
        if(tagContent[2][0]) printf(" %s ", tagContent[2]);
        printf(" %s", tagContent[0]);
        if(tagContent[1][0]) printf(" (%s)", tagContent[1]);
    } else printf(" Unknown");
    printf(" vs ");
    if(tagContent[5][0]) {
        if(tagContent[7][0]) printf("%s ", tagContent[7]);
        printf("%s", tagContent[5]);
        if(tagContent[6][0]) printf(" (%s)", tagContent[6]);
    } else printf("Unknown");
    printf("\n");

    if(tagContent[12][0]) {
        printf(" %s -> ", tagContent[12]);
        if(!strcmp("1-0", tagContent[12])) printf("White won the game\n");
        else if(!strcmp("0-1", tagContent[12])) printf("Black won the game\n");
        else if(!strcmp("1/2-1/2", tagContent[12])) printf("The game ended in a draw\n");
        else printf("Result of the game is unknown\n");
    }
}

void printBoard(Board board) {
    unsigned long long i = (BITFILE_A & BITRANK_8)>>1;
    int rank = 8, j, k;
    printf("     a   b   c   d   e   f   g   h\n    -------------------------------\n");

    // Iterate rank by rank, starting from rank 8
    while(rank != 0) {
        printf(" %i |", rank);
        // For each square in rank
        for(j = 0; j < 8; j++) {
            i <<= 1;
            if(i == 0) i = 1;

            for(k = 0; k < PIECE_TYPE_COUNT; k++){
            if(board.white[k] & i) {
                printf(" %c ", PIECE_CHARS[k]);
                break;
            } else if(board.black[k] & i) {
                printf(" %c ", PIECE_CHARS[k+6]);
                break;
            }
            }
            //Piece is not there, the square is empty
            if(k == 6) printf("   ");
            printf("|");
        }
        printf(" %i\n", rank--);
        printf("    -------------------------------\n");
        // Go back to the beginning of the rank i>>=8 and then to the beginning of the rank below i>>=8, so i>>=16 in total
        i >>= 16;
    }
    printf("     a   b   c   d   e   f   g   h\n");
}

void fullMoveString(int* newLinesBeforeMove, int* allNewLines, char* moveString, char (*rawMoves)[MAX_ALGEBRAIC_LENGTH], char (*comments)[MAX_COMMENT_LENGTH], Board* positions) {
    char* i = moveString;
    int commentIndex = 0, newLineCount = 0, realMoveIndex = 1, depth = 0, moveIndexForDepth[MAX_VARIATION_DEPTH] = {1};
    for(int moveIndex = 0; rawMoves[moveIndex][0] != '\0'; moveIndex++) {
        if(rawMoves[moveIndex][0] == '-') {
            depth--;
            continue;
        }
        if(rawMoves[moveIndex][0] == '+') {
            depth++, moveIndexForDepth[depth] = moveIndexForDepth[depth-1];
            continue;
        }

        //We have reached a commment
        if(rawMoves[moveIndex][0] == '0') {
            *i++ = '\n', newLineCount++;
            tabulate(&i, depth);
            *i++ = '#', *i++ = ' ';
            for(char* j = comments[commentIndex]; *j != '\0'; j++) {
            *i++ = *j;
            if(*j == '\n') {
                tabulate(&i, depth);
                *i++ = '#', *i++ = ' ', newLineCount++;
            }
            }
            commentIndex++;
            continue;
        }

        //If we get here, it means we deal with an actual move
        char charMoveIndex[5];
        if(moveIndex == 0 || rawMoves[moveIndex-1][0] == '+' || rawMoves[moveIndex-1][0] == '-' || rawMoves[moveIndex-1][0] == '0') {
            *i++ = '\n', newLineCount++, newLinesBeforeMove[realMoveIndex] = newLineCount;
            tabulate(&i, depth);
            sprintf(charMoveIndex, "%i", moveIndexForDepth[depth]);
            for(char* j = charMoveIndex; *j != '\0'; j++) *i++ = *j;

            if(!positions[realMoveIndex++].turn) *i++ = '.', *i++ = '.', moveIndexForDepth[depth]++;
            *i++ = '.', *i++ = ' ', *i++ = SPECIAL_MOVE_HIGHLIGHT_CHAR;
            for(char *j  = rawMoves[moveIndex]; *j != '\0'; j++) *i++ = *j;
        } else if(!positions[realMoveIndex].turn) {
            *i++ = ' ', *i++ = SPECIAL_MOVE_HIGHLIGHT_CHAR, newLinesBeforeMove[realMoveIndex++] = newLineCount;
            for(char *j  = rawMoves[moveIndex]; *j != '\0'; j++) *i++ = *j;
        } else {
            moveIndexForDepth[depth]++, *i++ = '\n', newLineCount++, newLinesBeforeMove[realMoveIndex++] = newLineCount;
            tabulate(&i, depth);
            sprintf(charMoveIndex, "%i", moveIndexForDepth[depth]);
            for(char* j = charMoveIndex; *j != '\0'; j++) *i++ = *j;
            *i++ = '.', *i++ = ' ', *i++ = SPECIAL_MOVE_HIGHLIGHT_CHAR;
            for(char *j = rawMoves[moveIndex]; *j != '\0'; j++) *i++ = *j;
        }
    }
    *allNewLines = newLineCount;
}

//Print the final string that the user will see in console
void printMoves(int curMove, int skipNewLines, int allNewLines, char* moveString, Board* positions, char (*tagContent)[MAX_TAG_VALUE_LENGTH]) {
    clearScreen();
    printAbout();
    
    int count = 0, newLines = 0, realIndex = 0;
    for(char* c = moveString; *c != '\0'; c++) {
        if(*c == '\n') newLines++;
        if(*c == SPECIAL_MOVE_HIGHLIGHT_CHAR) {
        if(++count == curMove) printf(">>>");
        continue;
        }

        int missedBefore = skipNewLines < AMOUNT_OF_PRINTED_LINES/2 ? AMOUNT_OF_PRINTED_LINES/2 - skipNewLines : 0;
        int missedAfter = skipNewLines + AMOUNT_OF_PRINTED_LINES/2 > allNewLines ? skipNewLines + AMOUNT_OF_PRINTED_LINES/2 - allNewLines : 0;
        if(!(skipNewLines-newLines-missedAfter > AMOUNT_OF_PRINTED_LINES/2 || newLines-skipNewLines-missedBefore > AMOUNT_OF_PRINTED_LINES/2)) printf("%c", *c);
    }
    printf("\n\n\n");
    printBoard(positions[curMove]);
    printTags(tagContent);
    printf(" Input \"a\" to go back a move, \"d\" to go forward and \"0\" to terminate the program.\n");
}
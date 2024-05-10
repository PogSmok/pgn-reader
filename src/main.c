/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "board.h"
#include "consts.h"
#include "extract.h"
#include "print.h"

int main(int argc, char* argv[]) {
    printAbout();
    if(argc < 2) {
        printf(" To read a .pgn file, use the program as follows <executable> <path to .pgn>.\n");
        return -1;
    }

    int allNewLines, newLinesBeforeMove[MAX_CONTENT_COUNT] = {0};
    char pgnContent[MAX_PGN_LENGTH] = {0}, tagContent[TAG_COUNT][MAX_TAG_VALUE_LENGTH] = {0}, rawMoves[MAX_CONTENT_COUNT][MAX_ALGEBRAIC_LENGTH] = {0}, comments[MAX_COMMENT_COUNT][MAX_COMMENT_LENGTH] = {0}, moveString[MAX_PGN_LENGTH];
    Board positions[MAX_CONTENT_COUNT] = {0};

    extractAll(argv[1], pgnContent, tagContent, rawMoves, comments, positions);
    fullMoveString(newLinesBeforeMove, &allNewLines, moveString, rawMoves, comments, positions);

    int curMove = 0;
    printMoves(0, 0, allNewLines, moveString, positions, tagContent);
    while(1) {
        char c;
        do c = getc(stdin);
        while(c != 'd' && c != 'a' && c != '0');

        if(c == '0') break;
        c == 'a' ? curMove-- : curMove++;
        if(curMove < 0) curMove = 0;
        else if(isBoardNull(positions[curMove])) curMove--;
        printMoves(curMove, newLinesBeforeMove[curMove], allNewLines, moveString, positions, tagContent);
    }
    return 0;
}

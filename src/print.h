/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
*/

#pragma once

#include "consts.h"
#include "board.h"

void printAbout();
void clearScreen();
void tabluate(char** c, int tabAmount);
void printTags(char (*tagContent)[MAX_TAG_VALUE_LENGTH]);
void printBoard(Board board);
void fullMoveString(
                    int* newLinesBeforeMove, int* allNewLines, char* moveString,
                    char (*rawMoves)[MAX_ALGEBRAIC_LENGTH], char (*comments)[MAX_COMMENT_LENGTH],
                    Board* positions
                );
void printMoves(int curMove, int skipNewLines, int allNewLines, char* moveString, Board* positions, char (*tagContent)[MAX_TAG_VALUE_LENGTH]);
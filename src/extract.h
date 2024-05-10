/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
*/

#pragma once

#include "consts.h"
#include "board.h"

void readFile(char* path, char* pgnContent);
char* extractTags(char* path, char* pgnContent, char  (*tagContent)[MAX_TAG_VALUE_LENGTH]);
void extractMoves(char* moves, char (*rawMoves)[MAX_ALGEBRAIC_LENGTH], char (*comments)[MAX_COMMENT_LENGTH]);
void fillPositions(char (*rawMoves)[MAX_ALGEBRAIC_LENGTH], Board* positions, Board startingPosition);
void extractAll(char* path, char* pgnContent, char (*tagContent)[MAX_TAG_VALUE_LENGTH], char (*rawMoves)[MAX_ALGEBRAIC_LENGTH], char (*comments)[MAX_COMMENT_LENGTH], Board* positions);
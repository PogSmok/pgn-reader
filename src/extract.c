/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "consts.h"
#include "board.h"
#include "bitmove.h"

// Extracts unedited content of the file to pgnContent
void readFile(char* path, char* pgnContent) {
    FILE *fptr;
    int c, i = 0;
    fptr = fopen(path, "r");

    if(fptr != NULL) 
        while((c = fgetc(fptr)) != EOF && i < MAX_PGN_LENGTH) pgnContent[i++] = c;
    else exit(printf(" The file \"%s\" could not be opened, verify the path.\n", path));
    fclose(fptr); 
}

// Returns pointer to the beginning of the moves in the file 
char* extractTags(char* path, char* pgnContent, char (*tagContent)[MAX_TAG_VALUE_LENGTH]) {
    char *head, *i = pgnContent;

    while(*i != '\0') {
        // We have reached the first move
        if(*i == '1') return i;
        if(*i == '[') {
            head = i+1;

            while(*(++i) != ' ');
            char* tag = calloc(MAX_TAG_VALUE_LENGTH, sizeof(char));
            // First use tag, to search for which tag type it is
            strncpy(tag, head, i-head);

            int tagIndex;
            for(tagIndex = 0; tagIndex < TAG_COUNT; tagIndex++) 
                if(!strcmp(PGN_TAGS[tagIndex], tag)) break;

            head = ++i+1;
            while(*(++i) != '"')
                // Allow escaping " (\")
                if(*i == '\\' && *(i+1) == '"') *i++;

            // Now that tag type is saved in tagIndex, use tag to store value of the tag
            tag = memset(tag, 0, MAX_TAG_VALUE_LENGTH);
            tag = strncpy(tag, head, i-head);

            if(tagIndex < TAG_COUNT) {
                int j = 0;
                head = tag;
                while(*head != '\0' && j < MAX_TAG_VALUE_LENGTH) tagContent[tagIndex][j++] = *(head++);
            }

            while(*(++i) != '\n');

            free(tag);
        }
        printf("%c", *i);
        *i++;
    }
    exit(printf(" The file \"%s\" does not have any moves.\n", path));
}

void extractMoves(char* moves, char (*rawMoves)[MAX_ALGEBRAIC_LENGTH], char (*comments)[MAX_COMMENT_LENGTH]) {
    int rawMovesIndex = 0, commentIndex = 0;
    for(char* i = moves; *i != '\0'; i++) {
        while(*i == ' ' || *i == '\n') i++; 
        // Beginning of a comment
        if(*i == '{') {
            i++;
            // rawMoves[i] == "0" -> comment at that index
            rawMoves[rawMovesIndex++][0] = '0';

            int j = 0;
            while(*i != '}') comments[commentIndex][j++] = *i++;
            commentIndex++;
        } else if(*i == '(') {
            // rawMoves[i] == "+" -> depth increase at that index
            rawMoves[rawMovesIndex++][0] = '+';
        // End of a variation
        } else if(*i == ')') {
            // rawMoves[x] == "-" -> depth decrease at that index
            rawMoves[rawMovesIndex++][0] = '-';
        // Remove any move encoding from various websites
        } else if(*i == '$') while(*i != ' ' && *i != '\n' && *i != '\0') *i++;
        // We have reached a move
        else {
        // Skip number of the move, it is calculated and doesn't need to be saved
            if(*i >= '0' && *i <= '9') {
            while(*i != '.' || *(i+1) == '.') if(*i++ == '\0') return;
            i++;
            }
            while((*i == ' ' || *i == '\n' || *i == '*') && *i != '\0') i++;

            int j = 0;
            // Extract algebraic move
            while(*i != ' ' && *i != '\n' && *i != '\0') {
            if(*i == ')') {
                i--;
                break;
            }
            rawMoves[rawMovesIndex][j++] = *i++;
            }
            rawMovesIndex++;
        }
    }
}

void fillPositions(char (*rawMoves)[MAX_ALGEBRAIC_LENGTH], Board* positions, Board startingPosition) {
    positions[0] = startingPosition;
    int depth = 0, realIndex = 1;
    Board positionsInDepth[MAX_VARIATION_DEPTH][2], previousPosition = startingPosition;
    positionsInDepth[0][0] = positions[0];  

    for(int i = 0; rawMoves[i][0] != '\0'; i++) {
        if(rawMoves[i][0] == '+') depth++, previousPosition = positionsInDepth[depth-1][1], positionsInDepth[depth][0] = previousPosition;
        else if(rawMoves[i][0] == '-') depth--, previousPosition = positionsInDepth[depth][0];
        else if(rawMoves[i][0] == '0') continue;
        else {
            positions[realIndex] = applyBitMove(translateAlgebraic(rawMoves[i], previousPosition.turn), previousPosition), previousPosition = positions[realIndex];
            positionsInDepth[depth][1] = positionsInDepth[depth][0], positionsInDepth[depth][0] = positions[realIndex++];
        }
    }
}

void extractAll(char* path, char* pgnContent, char (*tagContent)[MAX_TAG_VALUE_LENGTH], char (*rawMoves)[MAX_ALGEBRAIC_LENGTH], char (*comments)[MAX_COMMENT_LENGTH], Board* positions) {
   readFile(path, pgnContent);
   char* moves = extractTags(path, pgnContent, tagContent);
   Board startingPosition = STARTING_BOARD;
   if(tagContent[TAG_COUNT-1][0] != '\0') startingPosition = fenToBoard(tagContent[TAG_COUNT-1]);
   extractMoves(moves, rawMoves, comments);
   fillPositions(rawMoves, positions, startingPosition);
}
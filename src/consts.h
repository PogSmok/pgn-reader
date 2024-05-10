/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
*/

#pragma once

/*
    ____   ____ _   _    ____  _____    _    ____  _____ ____ 
    |  _ \ / ___| \ | |  |  _ \| ____|  / \  |  _ \| ____|  _ \
    | |_) | |  _|  \| |  | |_) |  _|   / _ \ | | | |  _| | |_) |
    |  __/| |_| | |\  |  |  _ <| |___ / ___ \| |_| | |___|  _ <
    |_|    \____|_| \_|  |_| \_\_____/_/   \_\____/|_____|_| \_\
*/
#define PGN_READER_ASCII_ART "  ____   ____ _   _    ____  _____    _    ____  _____ ____  \n |  _ \\ / ___| \\ | |  |  _ \\| ____|  / \\  |  _ \\| ____|  _ \\\n | |_) | |  _|  \\| |  | |_) |  _|   / _ \\ | | | |  _| | |_) |\n |  __/| |_| | |\\  |  |  _ <| |___ / ___ \\| |_| | |___|  _ <\n |_|    \\____|_| \\_|  |_| \\_\\_____/_/   \\_\\____/|_____|_| \\_\\"
#define VERSION "1.00"

#define PLAYER_COUNT 2
#define SQUARE_COUNT 64
#define RANK_COUNT 8
#define FILE_COUNT 8
// Only one diagonal type is being counted (for example right oriented diagonals A1-H8, A2-G8 ... B1-H7, C1-H6 ...)
#define DIAGONAL_COUNT 15

#define PIECE_TYPE_COUNT 6
#define PAWN_INDEX 0
#define KNIGHT_INDEX 1
#define BISHOP_INDEX 2
#define ROOK_INDEX 3
#define QUEEN_INDEX 4
#define KING_INDEX 5
#define SHORT_CASTLE_INDEX 6
#define LONG_CASTLE_INDEX 7
static const char PIECE_CHARS[PIECE_TYPE_COUNT*PLAYER_COUNT] = {
    'P', 'N', 'B', 'R', 'Q', 'K',
    'p', 'n', 'b', 'r', 'q', 'k',
};
static int PIECE_INDEX(char pieceChar) {
    switch(pieceChar) {
        case('p'):
        case('P'):
            return PAWN_INDEX;
        case('n'):            
        case('N'):
            return KNIGHT_INDEX;
        case('b'):
        case('B'):
            return BISHOP_INDEX;
        case('r'):
        case('R'):
            return ROOK_INDEX;
        case('q'):
        case('Q'):
            return QUEEN_INDEX;
        case('k'):
        case('K'):
            return KING_INDEX;
        default: 
            return -1;
    }
}

#define MAX_PGN_LENGTH 262144
#define MAX_CONTENT_COUNT 2048
#define MAX_COMMENT_COUNT 1024
#define MAX_COMMENT_LENGTH 1024
#define MAX_VARIATION_DEPTH 10
#define MAX_ALGEBRAIC_LENGTH 16

#define MAX_TAG_NAME_LENGTH 11
#define TAG_COUNT 20
#define MAX_TAG_VALUE_LENGTH 255
static const char* PGN_TAGS[TAG_COUNT] = {
    "White",
    "WhiteElo",
    "WhiteTitle",
    "WhiteTeam",
    "WhiteFideId",

    "Black",
    "BlackElo",
    "BlackTitle",
    "BLackTeam",
    "BlackFideId",

    "TimeControl",
    "Date",
    "Result",
    "Termination",
    "Site",
    "Event",
    "Round",
    "Board",
    "Annotator",
    "FEN",
};

#define AMOUNT_OF_SPACES_IN_TAB 7
#define AMOUNT_OF_PRINTED_LINES 30
// Moves are prefixed with this char, to extract them easily
#define SPECIAL_MOVE_HIGHLIGHT_CHAR 127
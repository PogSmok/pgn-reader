# PGN Reader
Lightweight, blazingly fast (overly optimized) CLI for reading .pgn files

![Screenshot of The Game of the Century displayed in PGN Reader](https://github.com/PogSmok/pgn-reader/blob/main/EXAMPLE.png?raw=true)

## Usage
1. Download the project and compile the program, for example, with gcc. Note: You have to compile all .c files, not only main.c
```bash
gcc -o pgn-reader bitconsts.c bitmove.c board.c extract.c main.c print.c
```
&emsp; or alternatively
```bash
gcc -o pgn-reader *.c
```

2. Run the executable with the attached example game to see if everything works.
```bash
pgn-reader ./example.pgn
```

3. Run the executable with the path to your `.pgn` file as an argument.
```bash
pgn-reader path-to-my-pgn-file
```

4. View your game; it's this simple!

## FAQ
#### Q: What is a `.pgn` file?

A: It's a file extension specifically used for saving chess games. More information on [wikipedia](https://en.wikipedia.org/wiki/Portable_Game_Notation)

___

#### Q: How do I save my game as a `.pgn` file?

A: The quickest way is to use [lichess](https://lichess.org) and import a game to their site (you can get a computer analysis for free too). Then, in an analysis tab, you can export your game as a `.pgn`.

___

#### Q: When will I ever need this program?

A: If ever an alien civilization destroys all network infrastructure and asks you to present them [The Game of the Century](https://en.chessbase.com/post/game-of-the-century-a-full-summary) within a few milliseconds, or otherwise they will destroy everything you know and love, only then will your gratitude be immeasurable. 
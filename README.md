![Screenshot][screenshot]

<div align="center">
	<h1>PGN Reader</h1>
	<h3>Lightweight, blazingly fast (overly optimized) CLI for reading .pgn files</h3>

  <br>
  
  [![Report a bug][issue-badge]][issue-link]
  [![Suggest a feature][feature-badge]][feature-link]
  
  <br>
  
  [![License][license-badge]][license-link]
  [![Version][version-badge]][version-link]
  [![Latest Release][latest-badge]][latest-link]

</div>

## Overview
PGN Reader is a user-friendly and efficient application that allows you to read .pgn files smoothly through a terminal. The application is incredibly fast and lightweight, which means it can work on almost any device or system. It supports Chess 960 and can handle variations, comments, and annotations with clean and readable formatting. PGN Reader provides a simple way to access all the necessary information from your .pgn files.

## Files
  PGN reader consists of:
  * [README.md][readme-link] > File explaning everything about the application
  * [LICENSE][license-link] > License under which the program must be used
  * [EXAMPLE.png][screenshot] > Screenshot of a terminal while viewing a game in PGN Reader
  * [example.pgn][example-link] > Annotated PGN file containing The Game Of The Century
  * [src][src-link] > Directory with source code of the application

## Usage
  Using and compiling PGN Reader is as easy as it gets. Download the entire project and then compile the source code.
  <br>
  NOTE: you have to compile all .c files, not only main.c
  ```
    gcc -o pgn-reader ./src/*.c
  ```
  After the program is compiled, view .pgn file as follows
  ```
    <executable> <path to .pgn file>
    ./pgn-reader ./example.pgn
  ```
## FAQ
  #### Q: What is a `.pgn` file?
  
  A: It's a file extension specifically used for saving chess games. More information on [wikipedia][wikipedia-link]
  
  ___
  
  #### Q: How do I save my game as a `.pgn` file?
  
  A: The quickest way is to use [lichess][lichess-link] and import a game to their site (you can get a computer analysis for free too). Then, in an analysis tab, you can export your game as a `.pgn`.
  
  ___
  
  #### Q: When will I ever need this program?
  
  A: If ever an alien civilization destroys all network infrastructure and asks you to present them [The Game of the Century][game-of-the-century-link] within a few milliseconds, or otherwise they will destroy everything you know and love, only then will your gratitude be immeasurable. 

[screenshot]: https://github.com/PogSmok/pgn-reader/blob/master/EXAMPLE.png?raw=true
[issue-link]: https://github.com/PogSmok/pgn-reader/issues
[feature-link]: https://github.com/PogSmok/pgn-reader/discussions/categories/ideas
[license-link]: https://github.com/PogSmok/pgn-reader/blob/master/LICENSE
[version-link]: https://github.com/PogSmok/pgn-reader/releases/tag/1.0
[latest-link]:  https://github.com/PogSmok/pgn-reader/releases/latest
[readme-link]: https://github.com/PogSmok/pgn-reader/blob/master/README.md
[example-link]: https://github.com/PogSmok/pgn-reader/blob/master/example.pgn
[src-link]: https://github.com/PogSmok/pgn-reader/tree/master/src
[wikipedia-link]: https://en.wikipedia.org/wiki/Portable_Game_Notation
[lichess-link]: https://lichess.org
[game-of-the-century-link]: https://en.chessbase.com/post/game-of-the-century-a-full-summary

[issue-badge]: https://img.shields.io/badge/%F0%9F%91%BE-Report%20a%20bug-%23a8161b?style=for-the-badge&labelColor=%23ab5053
[feature-badge]: https://img.shields.io/badge/%F0%9F%92%A1-Suggest%20a%20feature-%2300d1ca?style=for-the-badge&labelColor=%23c8f7f6
[license-badge]: https://img.shields.io/badge/Apache%202.0-red?style=for-the-badge&label=License
[version-badge]: https://img.shields.io/badge/1.0-%2332a852?style=for-the-badge&label=Version
[latest-badge]: https://img.shields.io/badge/Latest%20Release-blue?style=for-the-badge

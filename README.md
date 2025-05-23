# Chess Engine in C++

Simple chess engine written in C++ based on minmax algorithm with alpha-beta pruning. It supports basic gameplay through simple terminal interface. Built for learning purpouses, this is not a high-performance engine in any way, shape or form.

## Features

- Board representation using an 8×8 character array
- Legal move generation including:
  - Castling
  - En passant
  - Pawn promotion
- Move making and unmaking (with state tracking)
- Search engine (minimax with depth control)
- Console interface to play as white or black
- Basic error handling for invalid moves

## Getting Started

### Prerequisites

- C++17 compatible compiler (tested on g++)
- GNU Make (tested with GNU Make 4.3 on Ubuntu 22.04)

### Build

To compile the project:

```bash
make
make test
./build/main
```

## Tests

- `unmakeTest.cpp` – Verifies the `makeMove()` and `unmakeMove()` functions to check board state consistency.
- `randomEngineTest.cpp` – Runs automated games between the engine and a random-move player to evaluate basic functionality and track win rate.
- `depthEngineTest.cpp` – Runs two engines with different search depths against each other, tracking win rates of each engine.

## Limitations

This is a work-in-progress. Currently, it does not support:

- Move input in SAN

- Detection of stalemates or threefold repetition

- Time controls or UCI protocol

- Advanced evaluation heuristics (material-only evaluation for now)

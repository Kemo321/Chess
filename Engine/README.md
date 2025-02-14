
---

# PerchFish Chess Engine

**PerchFish** is a C++ chess engine that calculates the best moves using an efficient alpha-beta pruning algorithm combined with custom heuristics. It features an HTTP API for remote access and leverages SQLite for caching previously computed positions.

---

## Architecture

- **Chess State & Moves**  
  - **ChessState**: Represents the board, handles move execution/undo, legal move generation, and game state updates.
  - **ChessMove**: Encapsulates move details (from/to coordinates, promotion, etc.).

- **Engine**  
  - Core search logic uses **alpha-beta pruning** to efficiently search the game tree.
  - Evaluates board positions using one or more heuristics (e.g., material balance, positional advantage).
  - Caches evaluated positions in an SQLite database using the **PositionORM** layer.

- **PositionORM**  
  - A lightweight ORM that interacts with an SQLite database.
  - Automatically creates the `POSITION` table (if it doesn’t exist) using:  
    ```sql
    CREATE TABLE IF NOT EXISTS POSITION (
      NAME CHAR(71) NOT NULL PRIMARY KEY,
      BEST_MOVE CHAR(5) NOT NULL,
      SCORE INT NOT NULL
    );
    ```
  - Supports insert, update, delete, and fetch operations for board positions.

- **HTTP Server**  
  - Built with [cpp-httplib](https://github.com/yhirose/cpp-httplib), it exposes a simple endpoint (`/getBestMove`) that accepts a chess state string and returns the best move.
  - Ideal for integration with other applications or remote clients.

---

## Algorithms

- **Alpha-Beta Pruning**  
  PerchFish employs alpha-beta pruning to reduce the number of nodes evaluated during the minimax search, thus optimizing performance without sacrificing decision quality.

- **Heuristic Evaluation**  
  The engine uses customizable heuristics to evaluate board positions. These heuristics assign numerical scores based on factors such as material balance, piece activity, and positional strength.

---

## Build & Deployment

- **CMake Build System**  
  The project uses CMake to manage builds. External dependencies such as GoogleTest and cpp-httplib are fetched automatically using CMake’s FetchContent module.

- **Dockerized Deployment with SQLite**  
  A Dockerfile and docker-compose setup are provided for containerized deployment. The Docker Compose file creates a persistent volume for the SQLite database, ensuring cached positions persist across container restarts.

  **docker-compose.yml:**
  ```yaml
  version: "3.8"

  services:
    perchfish:
      build: .
      environment:
        DB_PATH: /app/database/chess.db
      ports:
        - "8080:8080"
      volumes:
        - sqlite-data:/app/database

  volumes:
    sqlite-data:
  ```

  **Dockerfile:**
  ```dockerfile
  FROM ubuntu:latest

  RUN apt update && apt install -y \
      cmake \
      g++ \
      make \
      git \
      curl \
      wget \
      libssl-dev \
      libsqlite3-dev \
      unzip

  WORKDIR /app

  COPY . .

  RUN rm -rf build CMakeCache.txt CMakeFiles
  RUN mkdir -p build
  WORKDIR /app/build

  RUN cmake -DCMAKE_BUILD_TYPE=Release .. && make -j$(nproc)

  EXPOSE 8080

  CMD ["./PerchFishMain"]
  ```

---

## Usage & Testing

- **HTTP API**:  
  Send a POST request to `http://localhost:8080/getBestMove` with a valid 71-character chess state string in the body. The engine will return the computed best move.

- **Command-Line**:  
  Run the standalone executable (built as `PerchFishMain`) to start the HTTP server or perform command-line operations.

- **Unit Tests**:  
  Tests are implemented using GoogleTest. Run the `PerchFishTest` target to execute the unit tests.

---

## Conclusion

PerchFish is a high-performance, modular chess engine that integrates advanced search algorithms with a simple caching layer via SQLite. Its HTTP API makes it easy to integrate into larger systems or expose chess analysis as a web service. Enjoy exploring and extending its capabilities!

---
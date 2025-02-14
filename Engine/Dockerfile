# Use an official Ubuntu image
FROM ubuntu:latest

# Install dependencies
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

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Ensure no old CMake cache exists
RUN rm -rf build CMakeCache.txt CMakeFiles

# Create build directory
RUN mkdir -p build
WORKDIR /app/build

# Configure & build
RUN cmake -DCMAKE_BUILD_TYPE=Release .. && make -j$(nproc)

# Expose the HTTP server port
EXPOSE 8080

# Run the chess server
CMD ["./PerchFishMain"]

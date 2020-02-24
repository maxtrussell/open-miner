#include <array>
#include <cstdint>
#include <iostream>
#include <cmath>
#include <vector>

const int CHUNK_SIZE = 16;
const int CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
const int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

using u8 = uint8_t;
using ChunkData = std::array<u8, CHUNK_VOLUME>;
using CompressedChunk = std::vector<std::pair<u8, unsigned short>>;

CompressedChunk compress(const ChunkData &data);
ChunkData decompress(const CompressedChunk &compressed);

int main() {
    ChunkData chunkData;
    CompressedChunk compressed = compress(chunkData);
    ChunkData restored = decompress(compressed);
    std::cout << sizeof(chunkData) << std::endl;
    std::cout << sizeof(compressed) << std::endl;
    std::cout << sizeof(restored) << std::endl;
}

// Compresses chunk using rle compression
CompressedChunk compress(const ChunkData &data) {
    CompressedChunk compressed;

    u8 curr = data[0];
    unsigned short runLength = 1;
    for (u8 id : data) {
        if (id == curr) {
            runLength++;
        } else {
            compressed.emplace_back(curr, runLength);
            curr = id;
            runLength = 1;
        }
    }
    compressed.emplace_back(curr, runLength);
    return compressed;
}

// Reverses rle compression
ChunkData decompress(const CompressedChunk &compressed) {
    ChunkData data;
    int i = 0;
    for (auto pair : compressed) {
        u8 id = pair.first;
        unsigned short runLength = pair.second;

        for (int j = 0; j < runLength; j++) {
            data[i++] = id;
        }
    }
    return data;
}

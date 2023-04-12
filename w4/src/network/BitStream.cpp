#include "BitStream.hpp"

BitStream::BitStream(byte *buffer, size_t capacity)
    : buffer_(buffer), capacity_(capacity) {}
#pragma once
#include "mathUtils.h"
#include "raylib.h"
#include "enet/enet.h"
#include <limits>


template<typename T>
T pack_float(float v, float lo, float hi, int n_bits)
{
  T range = (1 << n_bits) - 1;//std::numeric_limits<T>::max();
  return range * ((clamp(v, lo, hi) - lo) / (hi - lo));
}

template<typename T>
float unpack_float(T c, float lo, float hi, int n_bits)
{
  T range = (1 << n_bits) - 1;//std::numeric_limits<T>::max();
  return float(c) / range * (hi - lo) + lo;
}

template<typename T, int n_bits>
struct PackedFloat
{
  T packed_val;

  PackedFloat(float v, float lo, float hi) {
    pack(v, lo, hi);
  }

  PackedFloat(T packed_val) : packed_val(packed_val) {
  }

  void pack(float v, float lo, float hi) {
    packed_val = pack_float<T>(v, lo, hi, n_bits);
  }

  float unpack(float lo, float hi) {
    return unpack_float<T>(packed_val, lo, hi, n_bits);
  }
};

typedef PackedFloat<uint8_t, 4> float4bitsQuantized;

template<typename T, int n_bits_1, int n_bits_2>
struct PackedVec2 {
  PackedVec2(float x, float y, float lo, float hi) {
    pack(x, y, lo, hi);
  }

  PackedVec2(T packed_val) : packed_val(packed_val) {
  }

  void pack(float x, float y, float lo, float hi) {
    T packed_x = pack_float<T>(x, lo, hi, n_bits_1);
    T packed_y = pack_float<T>(y, lo, hi, n_bits_2);

    packed_val = packed_x << n_bits_2 | packed_y;
  }

  Vector2 unpack(float lo, float hi) {
    Vector2 vec;
    vec.x = unpack_float<T>(packed_val >> n_bits_2, lo, hi, n_bits_1);
    vec.y = unpack_float<T>(packed_val & ((1 << n_bits_2) - 1), lo, hi,
                            n_bits_2);
    return vec;
  }

  T packed_val;
};

template<typename T, int n_bits_1, int n_bits_2, int n_bits_3>
struct PackedVec3 {
  PackedVec3(float x, float y, float z, float lo, float hi) {
    pack(x, y, z, lo, hi);
  }

  PackedVec3(T packet_val) : packed_val(packet_val) {
  }

  void pack(float x, float y, float z, float lo, float hi) {
    T packed_x = pack_float<T>(x, lo, hi, n_bits_1);
    T packed_y = pack_float<T>(y, lo, hi, n_bits_2);
    T packed_z = pack_float<T>(z, lo, hi, n_bits_3);
    packed_val = (packed_x << (n_bits_2 + n_bits_3)) |
                 (packed_y << n_bits_3)
                 | packed_z;
  }

  Vector3 unpack(float lo, float hi) {
    Vector3 vec;
    vec.x = unpack_float<T>(packed_val >> (n_bits_2 + n_bits_3), lo, hi,
                            n_bits_1);
    vec.y = unpack_float<T>((packed_val >> n_bits_3) & ((1 << n_bits_2) - 1),
                            lo, hi, n_bits_2);
    vec.z = unpack_float<T>(packed_val & ((1 << n_bits_3) - 1), lo, hi,
                            n_bits_3);
    return vec;
  }

  T packed_val;
};


using PositionXQuantized = PackedFloat<uint16_t, 11>;
using PositionYQuantized = PackedFloat<uint16_t, 10>;

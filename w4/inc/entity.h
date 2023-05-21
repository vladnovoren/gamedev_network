#pragma once

#include <cstdint>
#include "Vector2.hpp"

using eid_t = uint16_t;

constexpr eid_t invalid_entity = -1;

struct Entity {
  uint32_t color = 0xff00ffff;
  Vec2f position;
  float radius;
  eid_t eid = invalid_entity;
};


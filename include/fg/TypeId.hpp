#pragma once
#include <cstdint>

using TypeId = uint32_t;

inline TypeId GetNextTypeId() {
  static uint32_t nextTypeId;

  return nextTypeId++;
}

template<typename T>
static uint32_t GetTypeId() {
  static uint32_t typeId = GetNextTypeId();

  return typeId;
}
#pragma once

#include <typeindex>
#include <any>
#include <unordered_map>

class FrameGraphBlackboard {
public:
  FrameGraphBlackboard() = default;
  FrameGraphBlackboard(const FrameGraphBlackboard &) = delete;
  FrameGraphBlackboard(FrameGraphBlackboard &&) noexcept = default;
  ~FrameGraphBlackboard() = default;

  FrameGraphBlackboard &operator=(const FrameGraphBlackboard &) = delete;
  FrameGraphBlackboard &operator=(FrameGraphBlackboard &&) noexcept = default;

  template <typename T, typename... Args> T &add(Args &&...args);

  template <typename T> [[nodiscard]] T &get();
  template <typename T> [[nodiscard]] T *try_get();

  template <typename T> [[nodiscard]] bool has() const;

private:
  std::unordered_map<std::type_index, std::any> m_storage;
};

#include "fg/Blackboard.inl"
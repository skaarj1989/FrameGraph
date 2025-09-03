#include <cassert>

template <typename T, typename... Args>
inline T &FrameGraphBlackboard::add(Args &&...args) {
  assert(!has<T>());
  return m_storage[GetTypeId<T>()].emplace<T>(T{std::forward<Args>(args)...});
}

template <typename T> const T &FrameGraphBlackboard::get() const {
  assert(has<T>());
  return std::any_cast<const T &>(m_storage.at(GetTypeId<T>()));
}
template <typename T> const T *FrameGraphBlackboard::try_get() const {
  auto it = m_storage.find(GetTypeId<T>());
  return it != m_storage.cend() ? std::any_cast<const T>(&it->second) : nullptr;
}

template <typename T> inline T &FrameGraphBlackboard::get() {
  return const_cast<T &>(
    const_cast<const FrameGraphBlackboard *>(this)->get<T>());
}
template <typename T> inline T *FrameGraphBlackboard::try_get() {
  return const_cast<T *>(
    const_cast<const FrameGraphBlackboard *>(this)->try_get<T>());
}

template <typename T> inline bool FrameGraphBlackboard::has() const {
  if constexpr (__cplusplus >= 202002L)
    return m_storage.contains(GetTypeId<T>());
  else
    return m_storage.find(GetTypeId<T>()) != m_storage.cend();
}

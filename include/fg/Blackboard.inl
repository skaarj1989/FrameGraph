#include <cassert>

template <typename T, typename... Args>
inline T &FrameGraphBlackboard::add(Args &&...args) {
  assert(!has<T>());
  return m_storage[typeid(T)].emplace<T>(T{std::forward<Args>(args)...});
}

template <typename T> inline T &FrameGraphBlackboard::get() {
  assert(has<T>());
  return std::any_cast<T &>(m_storage[typeid(T)]);
}
template <typename T> inline T *FrameGraphBlackboard::try_get() {
  auto it = m_storage.find(typeid(T));
  return it != m_storage.cend() ? std::any_cast<T>(&it->second) : nullptr;
}

template <typename T> inline bool FrameGraphBlackboard::has() const {
  if constexpr (_HAS_CXX20)
    return m_storage.contains(typeid(T));
  else
    return m_storage.find(typeid(T)) != m_storage.cend();
}

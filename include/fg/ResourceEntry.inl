#include <cassert>

//
// ResourceEntry class:
//

inline void ResourceEntry::create(void *allocator) {
  assert(isTransient());
  m_concept->create(allocator);
}
inline void ResourceEntry::destroy(void *allocator) {
  assert(isTransient());
  m_concept->destroy(allocator);
}

template <typename T> inline T &ResourceEntry::get() {
  return _getModel<T>()->resource;
}
template <typename T>
inline const typename T::Desc &ResourceEntry::getDescriptor() const {
  return _getModel<T>()->descriptor;
}

//
// (private):
//

template <typename T>
inline ResourceEntry::ResourceEntry(const Type type, uint32_t id,
                                    const typename T::Desc &desc, T &&obj)
    : m_type{type}, m_id{id}, m_version{kInitialVersion},
      m_concept{std::make_unique<Model<T>>(desc, std::forward<T>(obj))} {}

template <typename T> inline auto *ResourceEntry::_getModel() const {
  auto *model = dynamic_cast<Model<T> *>(m_concept.get());
  assert(model && "Invalid type");
  return model;
}

//
// ResourceEntry::Model class:
//

template <typename T>
inline ResourceEntry::Model<T>::Model(const typename T::Desc &desc, T &&obj)
    : descriptor{desc}, resource{std::move(obj)} {}

template <typename T>
inline void ResourceEntry::Model<T>::create(void *allocator) {
  resource.create(descriptor, allocator);
}
template <typename T>
inline void ResourceEntry::Model<T>::destroy(void *allocator) {
  resource.destroy(descriptor, allocator);
}

template <typename T>
inline std::string ResourceEntry::Model<T>::toString() const {
#if __cplusplus >= 202002L
  if constexpr (has_toString<T>)
#else
  if constexpr (has_toString<T>::value)
#endif
    return T::toString(descriptor);
  else
    return "";
}

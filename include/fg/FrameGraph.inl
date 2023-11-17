#include <functional>

//
// FrameGraph class:
//

template <typename Data, typename Setup, typename Execute>
inline const Data &FrameGraph::addCallbackPass(const std::string_view name,
                                               Setup &&setup, Execute &&exec) {
  static_assert(std::is_invocable_v<Setup, Builder &, Data &>,
                "Invalid setup callback");
  static_assert(std::is_invocable_v<Execute, const Data &,
                                    FrameGraphPassResources &, void *>,
                "Invalid exec callback");
  static_assert(sizeof(Execute) < 1024, "Execute captures too much");

  auto *pass = new FrameGraphPass<Data, Execute>(std::forward<Execute>(exec));
  auto &passNode =
    _createPassNode(name, std::unique_ptr<FrameGraphPass<Data, Execute>>(pass));
  Builder builder{*this, passNode};
  std::invoke(setup, builder, pass->data);
  return pass->data;
}

template <_VIRTUALIZABLE_CONCEPT_IMPL(T)>
inline const typename T::Desc &
FrameGraph::getDescriptor(FrameGraphResource id) const {
  return _getResourceEntry(id).getDescriptor<T>();
}

template <_VIRTUALIZABLE_CONCEPT_IMPL(T)>
inline FrameGraphResource FrameGraph::import(const std::string_view name,
                                             const typename T::Desc &desc,
                                             T &&resource) {
  return _create<T>(ResourceEntry::Type::Imported, name, desc,
                    std::forward<T>(resource));
}

template <typename Writer>
inline std::ostream &FrameGraph::debugOutput(std::ostream &os,
                                             Writer &&writer) const {
  for (const auto &node : m_passNodes) {
    writer(node, m_resourceNodes);
  }
  for (const auto &node : m_resourceNodes) {
    writer(node, m_resourceRegistry[node.m_resourceId], m_passNodes);
  }
  writer.flush(os);
  return os;
}

//
// (private):
//

template <_VIRTUALIZABLE_CONCEPT_IMPL(T)>
inline FrameGraphResource
FrameGraph::_create(const ResourceEntry::Type type, const std::string_view name,
                    const typename T::Desc &desc, T &&resource) {
  const auto resourceId = static_cast<uint32_t>(m_resourceRegistry.size());
  m_resourceRegistry.emplace_back(
    ResourceEntry{type, resourceId, desc, std::forward<T>(resource)});
  return _createResourceNode(name, resourceId).getId();
}

//
// FrameGraph::Builder class:
//

template <_VIRTUALIZABLE_CONCEPT_IMPL(T)>
inline FrameGraphResource
FrameGraph::Builder::create(const std::string_view name,
                            const typename T::Desc &desc) {
  const auto id =
    m_frameGraph._create<T>(ResourceEntry::Type::Transient, name, desc, T{});
  return m_passNode.m_creates.emplace_back(id);
}

//
// FrameGraphPassResources class:
//

template <_VIRTUALIZABLE_CONCEPT_IMPL(T)>
inline T &FrameGraphPassResources::get(FrameGraphResource id) {
  assert(m_passNode.reads(id) || m_passNode.creates(id) ||
         m_passNode.writes(id));
  return m_frameGraph._getResourceEntry(id).get<T>();
}
template <_VIRTUALIZABLE_CONCEPT_IMPL(T)>
inline const typename T::Desc &
FrameGraphPassResources::getDescriptor(FrameGraphResource id) const {
  assert(m_passNode.reads(id) || m_passNode.creates(id) ||
         m_passNode.writes(id));
  return m_frameGraph.getDescriptor<T>(id);
}

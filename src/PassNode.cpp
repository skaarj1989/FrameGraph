#include "fg/PassNode.hpp"
#include <algorithm>
#include <cassert>

namespace {

[[nodiscard]] bool hasId(const std::vector<FrameGraphResource> &v,
                         FrameGraphResource id) {
#if __cplusplus >= 202002L
  return std::ranges::find(v, id) != v.cend();
#else
  return std::find(v.cbegin(), v.cend(), id) != v.cend();
#endif
}
[[nodiscard]] bool hasId(const std::vector<PassNode::AccessDeclaration> &v,
                         FrameGraphResource id) {
  const auto match = [id](const auto &e) { return e.id == id; };
#if __cplusplus >= 202002L
  return std::ranges::find_if(v, match) != v.cend();
#else
  return std::find_if(v.cbegin(), v.cend(), match) != v.cend();
#endif
}

[[nodiscard]] bool contains(const std::vector<PassNode::AccessDeclaration> &v,
                            PassNode::AccessDeclaration n) {
#if __cplusplus >= 202002L
  return std::ranges::find(v, n) != v.cend();
#else
  return std::find(v.cbegin(), v.cend(), n) != v.cend();
#endif
}

} // namespace

bool PassNode::creates(FrameGraphResource id) const {
  return hasId(m_creates, id);
}
bool PassNode::reads(FrameGraphResource id) const { return hasId(m_reads, id); }
bool PassNode::writes(FrameGraphResource id) const {
  return hasId(m_writes, id);
}

//
// (private):
//

PassNode::PassNode(const std::string_view name, uint32_t nodeId,
                   std::unique_ptr<FrameGraphPassConcept> &&exec)
    : GraphNode{name, nodeId}, m_exec{std::move(exec)} {
  m_creates.reserve(10);
  m_reads.reserve(10);
  m_writes.reserve(10);
}

FrameGraphResource PassNode::_read(FrameGraphResource id, uint32_t flags) {
  assert(!creates(id) && !writes(id));
  return contains(m_reads, {id, flags})
           ? id
           : m_reads.emplace_back(AccessDeclaration{id, flags}).id;
}
FrameGraphResource PassNode::_write(FrameGraphResource id, uint32_t flags) {
  return contains(m_writes, {id, flags})
           ? id
           : m_writes.emplace_back(AccessDeclaration{id, flags}).id;
}

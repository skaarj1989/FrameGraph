#include "fg/PassNode.hpp"
#include <cassert>

namespace {

[[nodiscard]] bool hasId(const std::vector<FrameGraphResource> &v,
                         FrameGraphResource id) {
  return std::find(v.cbegin(), v.cend(), id) != v.cend();
}

} // namespace

bool PassNode::creates(FrameGraphResource id) { return hasId(m_creates, id); }
bool PassNode::reads(FrameGraphResource id) { return hasId(m_reads, id); }
bool PassNode::writes(FrameGraphResource id) { return hasId(m_writes, id); }

bool PassNode::hasSideEffect() const { return m_hasSideEffect; }

bool PassNode::canExecute() const { return m_refCount > 0 || hasSideEffect(); }

PassNode::PassNode(const std::string_view name, uint32_t id,
                   std::unique_ptr<FrameGraphPassConcept> &&exec)
    : GraphNode{name, id}, m_exec{std::move(exec)} {}

FrameGraphResource PassNode::_read(FrameGraphResource id) {
  assert(!creates(id) && !writes(id));
  return reads(id) ? id : m_reads.emplace_back(id);
}
FrameGraphResource PassNode::_write(FrameGraphResource id) {
  return writes(id) ? id : m_writes.emplace_back(id);
}

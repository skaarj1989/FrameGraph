#pragma once

#include "fg/GraphNode.hpp"
#include "fg/PassEntry.hpp"
#include "fg/FrameGraphResource.hpp"
#include <memory>
#include <vector>

class PassNode final : public GraphNode {
  friend class FrameGraph;

public:
  [[nodiscard]] bool creates(FrameGraphResource id) const;
  [[nodiscard]] bool reads(FrameGraphResource id) const;
  [[nodiscard]] bool writes(FrameGraphResource id) const;

  [[nodiscard]] bool hasSideEffect() const;
  [[nodiscard]] bool canExecute() const;

private:
  PassNode(const std::string_view name, uint32_t id,
           std::unique_ptr<FrameGraphPassConcept> &&);

  FrameGraphResource _read(FrameGraphResource id);
  [[nodiscard]] FrameGraphResource _write(FrameGraphResource id);

private:
  std::unique_ptr<FrameGraphPassConcept> m_exec;

  std::vector<FrameGraphResource> m_creates;
  std::vector<FrameGraphResource> m_reads;
  std::vector<FrameGraphResource> m_writes;

  bool m_hasSideEffect{false};
};

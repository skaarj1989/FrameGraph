#pragma once

#include "fg/PassNode.hpp"

constexpr uint32_t kResourceInitialVersion{1u};

class ResourceNode final : public GraphNode {
  friend class FrameGraph;

public:
  [[nodiscard]] auto getResourceId() const { return m_resourceId; }
  [[nodiscard]] auto getVersion() const { return m_version; }

private:
  ResourceNode(const std::string_view name, uint32_t id, uint32_t resourceId,
               uint32_t version);

private:
  // Index to virtual resource (m_resourceRegistry in FrameGraph)
  const uint32_t m_resourceId;
  const uint32_t m_version;

  PassNode *m_producer{nullptr};
  PassNode *m_last{nullptr};
};

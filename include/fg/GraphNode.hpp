#pragma once

#include <string>
#include <cstdint>

class FrameGraph;

class GraphNode {
  friend class FrameGraph;

public:
  GraphNode() = delete;
  GraphNode(const GraphNode &) = delete;
  GraphNode(GraphNode &&) noexcept = default;
  virtual ~GraphNode() = default;

  GraphNode &operator=(const GraphNode &) = delete;
  GraphNode &operator=(GraphNode &&) noexcept = delete;

  [[nodiscard]] auto getId() const { return m_id; }
  [[nodiscard]] std::string_view getName() const { return m_name; }
  [[nodiscard]] auto getRefCount() const { return m_refCount; }

protected:
  GraphNode(const std::string_view name, uint32_t id);

private:
  const std::string m_name;
  const uint32_t m_id; // Unique id, matches an array index in FrameGraph
  int32_t m_refCount{0};
};

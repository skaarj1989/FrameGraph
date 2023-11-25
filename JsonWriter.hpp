#pragma once

#include "fg/ResourceNode.hpp"
#include "fg/ResourceEntry.hpp"
#include "nlohmann/json.hpp"

struct JsonWriter {
  nlohmann::json j;

  void operator()(const PassNode &node,
                  const std::vector<ResourceNode> &resourceNodes) {
    auto &obj = j["passes"].emplace_back(nlohmann::json{
      {"id", node.getId()},
      {"name", node.getName()},
      {"culled", !node.canExecute()},
    });

    const auto getResourceIds = [&](const auto type) {
      std::vector<uint32_t> ids;
      ids.reserve(resourceNodes.size());
      for (const auto [id, _] : node.each(type)) {
        ids.emplace_back(resourceNodes[id].getResourceId());
      }
      return ids;
    };
    obj["reads"] = getResourceIds(PassNode::Read{});
    obj["writes"] = getResourceIds(PassNode::Write{});
  }

  void operator()(const ResourceNode &node, const ResourceEntry &entry,
                  const std::vector<PassNode> &passNodes) {
    auto &obj = j["resources"][entry.getId()];
    if (obj.empty()) {
      obj = nlohmann::json{
        {"id", entry.getId()},
        {"name", node.getName()},
        {"description", entry.toString()},
        {"transient", entry.isTransient()},
      };
    }
    for (const auto &p : passNodes) {
      if (p.creates(node.getId())) {
        obj["createdBy"] = p.getId();
      }
      if (p.reads(node.getId())) {
        obj["readers"].emplace_back(p.getId());
      }
      if (p.writes(node.getId())) {
        obj["writers"].emplace_back(p.getId());
      }
    }
  }

  void flush(std::ostream &os) const { os << std::setw(2) << j << "\n"; }
};

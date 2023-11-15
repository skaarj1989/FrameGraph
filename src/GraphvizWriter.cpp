#include "fg/GraphvizWriter.hpp"
#include <cassert>
#include <sstream>
#if __has_include(<format>)
#  include <format>
#endif

// https://www.graphviz.org/pdf/dotguide.pdf

namespace graphviz {

namespace {

[[nodiscard]] auto toString(const Color color) {
#define CASE(Value)                                                            \
  case Value:                                                                  \
    return #Value

  switch (color) {
    using enum Color;

    CASE(aliceblue);
    CASE(antiquewhite);
    CASE(aqua);
    CASE(aquamarine);
    CASE(azure);
    CASE(beige);
    CASE(bisque);
    CASE(black);
    CASE(blanchedalmond);
    CASE(blue);
    CASE(blueviolet);
    CASE(brown);
    CASE(burlywood);
    CASE(cadetblue);
    CASE(chartreuse);
    CASE(chocolate);
    CASE(coral);
    CASE(cornflowerblue);
    CASE(cornsilk);
    CASE(crimson);
    CASE(cyan);
    CASE(darkblue);
    CASE(darkcyan);
    CASE(darkgoldenrod);
    CASE(darkgray);
    CASE(darkgreen);
    CASE(darkgrey);
    CASE(darkkhaki);
    CASE(darkmagenta);
    CASE(darkolivegreen);
    CASE(darkorange);
    CASE(darkorchid);
    CASE(darkred);
    CASE(darksalmon);
    CASE(darkseagreen);
    CASE(darkslateblue);
    CASE(darkslategray);
    CASE(darkslategrey);
    CASE(darkturquoise);
    CASE(darkviolet);
    CASE(deeppink);
    CASE(deepskyblue);
    CASE(dimgray);
    CASE(dimgrey);
    CASE(dodgerblue);
    CASE(firebrick);
    CASE(floralwhite);
    CASE(forestgreen);
    CASE(fuchsia);
    CASE(gainsboro);
    CASE(ghostwhite);
    CASE(gold);
    CASE(goldenrod);
    CASE(gray);
    CASE(grey);
    CASE(green);
    CASE(greenyellow);
    CASE(honeydew);
    CASE(hotpink);
    CASE(indianred);
    CASE(indigo);
    CASE(ivory);
    CASE(khaki);
    CASE(lavender);
    CASE(lavenderblush);
    CASE(lawngreen);
    CASE(lemonchiffon);
    CASE(lightblue);
    CASE(lightcoral);
    CASE(lightcyan);
    CASE(lightgoldenrodyellow);
    CASE(lightgray);
    CASE(lightgreen);
    CASE(lightgrey);
    CASE(lightpink);
    CASE(lightsalmon);
    CASE(lightseagreen);
    CASE(lightskyblue);
    CASE(lightslategray);
    CASE(lightslategrey);
    CASE(lightsteelblue);
    CASE(lightyellow);
    CASE(lime);
    CASE(limegreen);
    CASE(linen);
    CASE(magenta);
    CASE(maroon);
    CASE(mediumaquamarine);
    CASE(mediumblue);
    CASE(mediumorchid);
    CASE(mediumpurple);
    CASE(mediumseagreen);
    CASE(mediumslateblue);
    CASE(mediumspringgreen);
    CASE(mediumturquoise);
    CASE(mediumvioletred);
    CASE(midnightblue);
    CASE(mintcream);
    CASE(mistyrose);
    CASE(moccasin);
    CASE(navajowhite);
    CASE(navy);
    CASE(oldlace);
    CASE(olive);
    CASE(olivedrab);
    CASE(orange);
    CASE(orangered);
    CASE(orchid);
    CASE(palegoldenrod);
    CASE(palegreen);
    CASE(paleturquoise);
    CASE(palevioletred);
    CASE(papayawhip);
    CASE(peachpuff);
    CASE(peru);
    CASE(pink);
    CASE(plum);
    CASE(powderblue);
    CASE(purple);
    CASE(red);
    CASE(rosybrown);
    CASE(royalblue);
    CASE(saddlebrown);
    CASE(salmon);
    CASE(sandybrown);
    CASE(seagreen);
    CASE(seashell);
    CASE(sienna);
    CASE(silver);
    CASE(skyblue);
    CASE(slateblue);
    CASE(slategray);
    CASE(slategrey);
    CASE(snow);
    CASE(springgreen);
    CASE(steelblue);
    CASE(tan);
    CASE(teal);
    CASE(thistle);
    CASE(tomato);
    CASE(turquoise);
    CASE(violet);
    CASE(wheat);
    CASE(white);
    CASE(whitesmoke);
    CASE(yellow);
    CASE(yellowgreen);
  }
  assert(false);
  return "";

#undef CASE
}
[[nodiscard]] auto toString(const RankDir rankDir) {
  switch (rankDir) {
    using enum RankDir;

  case TB:
    return "TB";
  case BT:
    return "BT";
  case LR:
    return "LR";
  case RL:
    return "RL";
  }
  assert(false);
  return "";
}

std::ostream &operator<<(std::ostream &os, const Graph::Style &style) {
  os << "graph [style=invis, rankdir=" << toString(style.rankDir)
     << " ordering=out, splines=spline]"
        "\n"
        "node [shape=record, fontname="
     << style.font.name << ", fontsize=" << style.font.size
     << R"(, margin="0.2,0.03"])"
        "\n";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Graph::Vertex &vertex) {
  os << vertex.key << "[label=<" << vertex.label << ">"
     << R"( style="rounded,filled", fillcolor=)" << toString(vertex.fillcolor)
     << "]\n";
  if (!vertex.cluster.empty()) {
    os << "subgraph cluster_" << vertex.key << " { " << vertex.key << " ";
    for (const auto &str : vertex.cluster) {
      os << str << " ";
    }
    os << "}\n";
  }
  return os;
}
std::ostream &operator<<(std::ostream &os, const Graph::Edge &edge) {
  if (!edge.vertices.empty()) {
    os << edge.key << "->{ ";
    for (const auto &vertex : edge.vertices) {
      os << vertex << " ";
    }
    os << "} [color=" << toString(edge.color) << "]\n";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const Graph &graph) {
  os << "digraph FrameGraph {\n" << graph.style << "\n";
  for (const auto &vertex : graph.vertices) {
    os << vertex;
  }
  for (const auto &edge : graph.edges) {
    os << edge;
  }
  if (!graph.imported.empty()) {
    os << "\nsubgraph cluster_imported_resources {\n";
    os << "graph [style=dotted, fontname=" << graph.style.font.name
       << ", label=< <B>Imported</B> >]"
          "\n";
    for (const auto id : graph.imported) {
      os << "R" << id << "_" << kResourceInitialVersion << " ";
    }
    os << "\n}\n";
  }
  os << "}\n";
  return os;
}

[[nodiscard]] auto makeKey(const PassNode &node) {
#if __has_include(<format>)
  return std::format("P{}", node.getId());
#else
  return "P" + std::to_string(node.getId());
#endif
}
[[nodiscard]] auto makeKey(const ResourceNode &node) {
#if __has_include(<format>)
  return std::format("R{}_{}", node.getResourceId(), node.getVersion());
#else
  return "R" + std::to_string(node.getResourceId()) + "_" +
         std::to_string(node.getVersion());
#endif
}

[[nodiscard]] auto stringify(const PassNode &node) {
  std::ostringstream oss;
  oss << "{ {<B>" << node.getName() << "</B>} | {"
      << (node.hasSideEffect() ? "&#x2605; " : "")
      << "Refs: " << node.getRefCount() << "<BR/> Index: " << node.getId()
      << "} }";
  return oss.str();
}
[[nodiscard]] auto stringify(const ResourceNode &node,
                             const ResourceEntry &entry) {
  std::ostringstream oss;
  oss << "{ {<B>" << node.getName() << "</B>";
  if (const auto version = node.getVersion();
      version > kResourceInitialVersion) {
    oss << "   <FONT>v" << version << "</FONT>";
  }
  oss << "<BR/>" << entry.toString() << "} | {Index: " << entry.getId()
      << "<BR/>"
      << "Refs : " << node.getRefCount() << "} }";
  return oss.str();
}

} // namespace

//
// Graph class:
//

Graph::Graph(Style style_) : style{std::move(style_)} {
  vertices.reserve(250);
  edges.reserve(100);
  imported.reserve(50);
}

//
// Writer class:
//

void Writer::operator()(const PassNode &node,
                        const std::vector<ResourceNode> &resourceNodes) {
  auto key = makeKey(node);
  auto &vertex = graph.vertices.emplace_back(
    key, stringify(node),
    node.canExecute() ? colors.pass.executed : colors.pass.culled);
  for (const auto id : node.each(PassNode::Create{})) {
    vertex.cluster.emplace_back(makeKey(resourceNodes[id]));
  }

  auto &edge = graph.edges.emplace_back(std::move(key), colors.edge.write);
  for (const auto [id, _] : node.each(PassNode::Write{})) {
    edge.vertices.emplace_back(makeKey(resourceNodes[id]));
  }
}

void Writer::operator()(const ResourceNode &node, const ResourceEntry &entry,
                        const std::vector<PassNode> &passNodes) {
  auto key = makeKey(node);
  graph.vertices.emplace_back(key, stringify(node, entry),
                              entry.isImported() ? colors.resource.imported
                                                 : colors.resource.transient);

  auto &edge = graph.edges.emplace_back(std::move(key), colors.edge.read);
  for (const auto &pass : passNodes) {
    if (pass.reads(node.getId())) {
      edge.vertices.emplace_back(makeKey(pass));
    }
  }
  if (entry.isImported()) graph.imported.emplace_back(entry.getId());
}

void Writer::flush(std::ostream &os) const { os << graph << "\n"; }

} // namespace graphviz

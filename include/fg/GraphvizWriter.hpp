#pragma once

#include "fg/ResourceNode.hpp"
#include "fg/ResourceEntry.hpp"

namespace graphviz {

// https://graphviz.org/doc/info/colors.html
enum class Color : uint8_t {
  aliceblue,
  antiquewhite,
  aqua,
  aquamarine,
  azure,
  beige,
  bisque,
  black,
  blanchedalmond,
  blue,
  blueviolet,
  brown,
  burlywood,
  cadetblue,
  chartreuse,
  chocolate,
  coral,
  cornflowerblue,
  cornsilk,
  crimson,
  cyan,
  darkblue,
  darkcyan,
  darkgoldenrod,
  darkgray,
  darkgreen,
  darkgrey,
  darkkhaki,
  darkmagenta,
  darkolivegreen,
  darkorange,
  darkorchid,
  darkred,
  darksalmon,
  darkseagreen,
  darkslateblue,
  darkslategray,
  darkslategrey,
  darkturquoise,
  darkviolet,
  deeppink,
  deepskyblue,
  dimgray,
  dimgrey,
  dodgerblue,
  firebrick,
  floralwhite,
  forestgreen,
  fuchsia,
  gainsboro,
  ghostwhite,
  gold,
  goldenrod,
  gray,
  grey,
  green,
  greenyellow,
  honeydew,
  hotpink,
  indianred,
  indigo,
  ivory,
  khaki,
  lavender,
  lavenderblush,
  lawngreen,
  lemonchiffon,
  lightblue,
  lightcoral,
  lightcyan,
  lightgoldenrodyellow,
  lightgray,
  lightgreen,
  lightgrey,
  lightpink,
  lightsalmon,
  lightseagreen,
  lightskyblue,
  lightslategray,
  lightslategrey,
  lightsteelblue,
  lightyellow,
  lime,
  limegreen,
  linen,
  magenta,
  maroon,
  mediumaquamarine,
  mediumblue,
  mediumorchid,
  mediumpurple,
  mediumseagreen,
  mediumslateblue,
  mediumspringgreen,
  mediumturquoise,
  mediumvioletred,
  midnightblue,
  mintcream,
  mistyrose,
  moccasin,
  navajowhite,
  navy,
  oldlace,
  olive,
  olivedrab,
  orange,
  orangered,
  orchid,
  palegoldenrod,
  palegreen,
  paleturquoise,
  palevioletred,
  papayawhip,
  peachpuff,
  peru,
  pink,
  plum,
  powderblue,
  purple,
  red,
  rosybrown,
  royalblue,
  saddlebrown,
  salmon,
  sandybrown,
  seagreen,
  seashell,
  sienna,
  silver,
  skyblue,
  slateblue,
  slategray,
  slategrey,
  snow,
  springgreen,
  steelblue,
  tan,
  teal,
  thistle,
  tomato,
  turquoise,
  violet,
  wheat,
  white,
  whitesmoke,
  yellow,
  yellowgreen,
};
enum class RankDir : uint8_t { TB, BT, LR, RL };

struct Graph {
  struct Style {
    RankDir rankDir{RankDir::TB};
    struct Font {
      std::string_view name{"helvetica"};
      uint16_t size{10};
    };
    Font font;
  };
  const Style style;

#ifdef __GNUG__
  static constexpr Style defaultStyle() { return Style{}; }
  explicit Graph(Style = defaultStyle());
#else
  explicit Graph(Style = {});
#endif

  struct Vertex {
    std::string key;
    std::string label;
    Color fillcolor;
    std::vector<std::string> cluster;
  };
  std::vector<Vertex> vertices;

  struct Edge {
    std::string key;
    Color color;
    std::vector<std::string> vertices;
  };
  std::vector<Edge> edges;

  std::vector<uint32_t> imported;
};

struct Writer {
  struct Colors {
    struct {
      Color executed{Color::orange};
      Color culled{Color::lightgray};
    } pass;
    struct {
      Color imported{Color::lightsteelblue};
      Color transient{Color::skyblue};
    } resource;
    struct {
      Color read{Color::yellowgreen};
      Color write{Color::orangered};
    } edge;
  };
  const Colors colors;

  Graph graph{};

  void operator()(const PassNode &, const std::vector<ResourceNode> &);
  void operator()(const ResourceNode &, const ResourceEntry &,
                  const std::vector<PassNode> &);

  void flush(std::ostream &) const;
};

} // namespace graphviz

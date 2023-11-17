#pragma once

#include "fg/TypeTraits.hpp"
#include <memory>

// Wrapper around a virtual resource.
class ResourceEntry final {
  friend class FrameGraph;

  enum class Type : uint8_t { Transient, Imported };

public:
  ResourceEntry() = delete;
  ResourceEntry(const ResourceEntry &) = delete;
  ResourceEntry(ResourceEntry &&) noexcept = default;

  ResourceEntry &operator=(const ResourceEntry &) = delete;
  ResourceEntry &operator=(ResourceEntry &&) noexcept = delete;

  static constexpr auto kInitialVersion{1u};

  [[nodiscard]] auto toString() const { return m_concept->toString(); }

  void create(void *allocator);
  void destroy(void *allocator);

  void preRead(uint32_t flags, void *context) {
    m_concept->preRead(flags, context);
  }
  void preWrite(uint32_t flags, void *context) {
    m_concept->preWrite(flags, context);
  }

  [[nodiscard]] auto getId() const { return m_id; }
  [[nodiscard]] auto getVersion() const { return m_version; }
  [[nodiscard]] auto isImported() const { return m_type == Type::Imported; }
  [[nodiscard]] auto isTransient() const { return m_type == Type::Transient; }

  template <typename T> [[nodiscard]] T &get();
  template <typename T>
  [[nodiscard]] const typename T::Desc &getDescriptor() const;

private:
  template <typename T>
  ResourceEntry(const Type, uint32_t id, const typename T::Desc &, T &&);

  // http://www.cplusplus.com/articles/oz18T05o/
  // https://www.modernescpp.com/index.php/c-core-guidelines-type-erasure-with-templates

  struct Concept {
    virtual ~Concept() = default;

    virtual void create(void *) = 0;
    virtual void destroy(void *) = 0;

    virtual void preRead(uint32_t flags, void *) = 0;
    virtual void preWrite(uint32_t flags, void *) = 0;

    virtual std::string toString() const = 0;
  };
  template <typename T> struct Model final : Concept {
    Model(const typename T::Desc &, T &&);

    void create(void *allocator) override;
    void destroy(void *allocator) override;

    void preRead(uint32_t flags, void *context) override {
#if __cplusplus >= 202002L
      if constexpr (has_preRead<T>)
#else
      if constexpr (has_preRead<T>::value)
#endif
        resource.preRead(descriptor, flags, context);
    }
    void preWrite(uint32_t flags, void *context) override {
#if __cplusplus >= 202002L
      if constexpr (has_preWrite<T>)
#else
      if constexpr (has_preWrite<T>::value)
#endif
        resource.preWrite(descriptor, flags, context);
    }

    std::string toString() const override;

    const typename T::Desc descriptor;
    T resource;
  };

  template <typename T> [[nodiscard]] auto *_getModel() const;

private:
  const Type m_type;
  const uint32_t m_id;
  uint32_t m_version; // Incremented on each (unique) write declaration.
  std::unique_ptr<Concept> m_concept;

  PassNode *m_producer{nullptr};
  PassNode *m_last{nullptr};
};

#include "ResourceEntry.inl"

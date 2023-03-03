#pragma once

#include <string>
#include <cstring>
#include <cassert>
#include <memory>
#include <iostream>

enum class PackageType {
  UNDEFINED,
  REGISTRY,
  KEEP_ALIVE,
  MSG
};

class Package {
 public:
  class BaseData;

  class RegistryData;

  class KeepAliveData;

  class MsgData;

 public:
  Package() = default;

  explicit Package(PackageType type);

  Package(const char* in_mem, size_t size);

  [[nodiscard]] void* Mem();

  [[nodiscard]] const void* Mem() const;

  [[nodiscard]] size_t MemSize() const;

  int Append(const char* add_data, size_t add_size);

  std::unique_ptr<BaseData> ExtractData();

 public:
  static constexpr size_t MAX_DATA_SIZE = 1000;

 public:
  class BaseData {
   public:
    class Visitor {
     public:
      virtual ~Visitor() = 0;

      virtual void Visit(RegistryData& package_data);

      virtual void Visit(KeepAliveData& package_data);

      virtual void Visit(MsgData& package_data);
    };

   public:
    virtual ~BaseData() = default;

    virtual Package MakePackage();

    virtual void Accept(Visitor& visitor);
  };

  class RegistryData : public BaseData {
   public:
    explicit RegistryData(std::string username);

    ~RegistryData() override = default;

    [[nodiscard]] const std::string& GetUsername() const;

    Package MakePackage() override;

    void Accept(Visitor& visitor) override;

   private:
    std::string username_;
  };

  class KeepAliveData : public BaseData {
   public:
    Package MakePackage() override;

    ~KeepAliveData() override = default;

    void Accept(Visitor& visitor) override;
  };

  class MsgData : public BaseData {
   public:
    explicit MsgData(std::string msg);

    ~MsgData() override = default;

    [[nodiscard]] const std::string& GetMsg() const;

    Package MakePackage() override;

    void Accept(Visitor& visitor) override;

   public:
    static constexpr size_t MAX_MSG_LEN = Package::MAX_DATA_SIZE;
   private:
    std::string msg_;
  };

 private:
  RegistryData* ParseAsRegistry();

  KeepAliveData* ParseAsKeepAlive();

  MsgData* ParseAsMsg();

 private:
  struct MemSample {
    PackageType type = PackageType::UNDEFINED;
    char data[MAX_DATA_SIZE + 1] = {};
  };
 public:
  static constexpr size_t MAX_MEM_SIZE = sizeof(MemSample) - 1;

 private:
  MemSample mem_sample_ = {};
  size_t data_size_ = 0;
};

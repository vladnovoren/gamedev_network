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

class BasePackageData;
class RegistryPackageData;
class KeepAlivePackageData;
class MsgPackageData;

class Package {
 public:
  Package() = default;
  explicit Package(PackageType type);
  Package(const char* in_mem, size_t size);

  int Append(const char* add_data, size_t add_size);
  std::unique_ptr<BasePackageData> ExtractData();

 public:
  static constexpr size_t MAX_DATA_SIZE = 1000;

 private:
  RegistryPackageData* ParseAsRegistryPackage();
  KeepAlivePackageData* ParseAsKeepAlivePackage();
  MsgPackageData* ParseAsMsgPackage();

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

class BasePackageData {
 public:
  virtual ~BasePackageData() = 0;

  virtual Package MakePackage() = 0;
};

class RegistryPackageData : public BasePackageData {
 public:
  explicit RegistryPackageData(const std::string& username);
  ~RegistryPackageData() override = default;

  [[nodiscard]] const std::string& GetUsername() const;

  Package MakePackage() override;

 private:
  const std::string& username_;
};

class KeepAlivePackageData : public BasePackageData {
 public:
  Package MakePackage() override;
  ~KeepAlivePackageData() override = default;
};

class MsgPackageData : public BasePackageData {
 public:
  explicit MsgPackageData(std::string msg);
  ~MsgPackageData() override = default;

  Package MakePackage() override;
 public:
  static constexpr size_t MAX_MSG_LEN = Package::MAX_DATA_SIZE;
 private:
  std::string msg_;
};
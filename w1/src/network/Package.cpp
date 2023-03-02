#include "Package.hpp"

Package::Package(PackageType type) {
  mem_sample_.type = type;
}

Package::Package(const char* in_mem, const size_t in_size) {
  assert(in_mem != nullptr);
  assert(in_size <= MAX_MEM_SIZE);

  memcpy(&mem_sample_, in_mem, in_size);
}

int Package::Append(const char* add_data, const size_t add_size) {
  if (data_size_ + add_size > MAX_DATA_SIZE)
    return -1;

  memcpy(mem_sample_.data + data_size_, add_data, add_size);
  data_size_ += add_size;

  return 0;
}

std::unique_ptr<BasePackageData> Package::ExtractData() {
  switch (mem_sample_.type) {
    case PackageType::REGISTRY:
      return std::unique_ptr<BasePackageData>(ParseAsRegistryPackage());
    case PackageType::KEEP_ALIVE:
      return std::unique_ptr<BasePackageData>(ParseAsKeepAlivePackage());
    case PackageType::MSG:
      return std::unique_ptr<BasePackageData>(ParseAsMsgPackage());
    case PackageType::UNDEFINED:
      std::cout << "package type undefined\n";
      return nullptr;
  }
}

RegistryPackageData* Package::ParseAsRegistryPackage() {
  return new RegistryPackageData(std::string(mem_sample_.data));
}

KeepAlivePackageData* Package::ParseAsKeepAlivePackage() {
  return new KeepAlivePackageData();
}

MsgPackageData* Package::ParseAsMsgPackage() {
  return new MsgPackageData(std::string(mem_sample_.data));
}

RegistryPackageData::RegistryPackageData(const std::string& username) :
  username_(username) {
}

const std::string& RegistryPackageData::GetUsername() const {
  return username_;
}

Package RegistryPackageData::MakePackage() {
  Package raw_package(PackageType::REGISTRY);
  raw_package.Append(username_.c_str(), username_.size());
  return raw_package;
}

Package KeepAlivePackageData::MakePackage() {
  return Package(PackageType::KEEP_ALIVE);
}

MsgPackageData::MsgPackageData(std::string msg) :
  msg_(std::move(msg)) {
  assert(msg_.size() <= MAX_MSG_LEN);
}

Package MsgPackageData::MakePackage() {
  Package raw_package(PackageType::MSG);
  raw_package.Append(msg_.c_str(), msg_.size());
  return raw_package;
}

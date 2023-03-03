#include "Package.hpp"

Package::Package(PackageType type) {
  mem_sample_.type = type;
}

Package::Package(const char* in_mem, const size_t in_size) {
  assert(in_mem != nullptr);
  assert(in_size <= MAX_MEM_SIZE);

  memcpy(&mem_sample_, in_mem, in_size);
}

void* Package::Mem() {
  return &mem_sample_;
}

const void* Package::Mem() const {
  return &mem_sample_;
}

size_t Package::MemSize() const {
  return sizeof(PackageType) + data_size_;
}

int Package::Append(const char* add_data, const size_t add_size) {
  if (data_size_ + add_size > MAX_DATA_SIZE)
    return -1;

  memcpy(mem_sample_.data + data_size_, add_data, add_size);
  data_size_ += add_size;

  return 0;
}

std::unique_ptr<Package::BaseData> Package::ExtractData() {
  switch (mem_sample_.type) {
    case PackageType::REGISTRY:
      return std::unique_ptr<BaseData>((BaseData*)ParseAsRegistryPackage());
    case PackageType::KEEP_ALIVE:
      return std::unique_ptr<BaseData>((BaseData*)ParseAsKeepAlivePackage());
    case PackageType::MSG:
      return std::unique_ptr<BaseData>((BaseData*)ParseAsMsgPackage());
    case PackageType::UNDEFINED:
      std::cout << "package type undefined\n";
      return nullptr;
  }
  return nullptr;
}

Package::BaseData::Visitor::~Visitor() = default;

void Package::BaseData::Visitor::Visit(RegistryData& package_data) {
}

void Package::BaseData::Visitor::Visit(KeepAliveData& package_data) {
}

void Package::BaseData::Visitor::Visit(MsgData& package_data) {
}

Package::RegistryData* Package::ParseAsRegistryPackage() {
  return new RegistryData(std::string(mem_sample_.data));
}

Package::KeepAliveData* Package::ParseAsKeepAlivePackage() {
  return new KeepAliveData();
}

Package::MsgData* Package::ParseAsMsgPackage() {
  return new MsgData(std::string(mem_sample_.data));
}

Package::RegistryData::RegistryData(const std::string& username) :
  username_(username) {
}

const std::string& Package::RegistryData::GetUsername() const {
  return username_;
}

Package Package::RegistryData::MakePackage() {
  Package raw_package(PackageType::REGISTRY);
  raw_package.Append(username_.c_str(), username_.size());
  return raw_package;
}

void Package::RegistryData::Accept(Visitor& visitor) {
  visitor.Visit(*this);
}

Package Package::KeepAliveData::MakePackage() {
  return Package(PackageType::KEEP_ALIVE);
}

void Package::KeepAliveData::Accept(Visitor& visitor) {
  visitor.Visit(*this);
}

Package::MsgData::MsgData(std::string msg) :
  msg_(std::move(msg)) {
  assert(msg_.size() <= MAX_MSG_LEN);
}

Package Package::MsgData::MakePackage() {
  Package raw_package(PackageType::MSG);
  raw_package.Append(msg_.c_str(), msg_.size());
  return raw_package;
}


const std::string& Package::MsgData::GetMsg() const {
  return msg_;
}

Package Package::BaseData::MakePackage() {
  return {};
}

void Package::MsgData::Accept(BaseData::Visitor& visitor) {
  visitor.Visit(*this);
}

void Package::BaseData::Accept(Visitor& visitor) {
}

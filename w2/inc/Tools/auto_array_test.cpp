#include "AutoArray.hpp"
#include "TypeID.hpp"
#include <iostream>
#include <functional>

class BasePacket {
 public:
  virtual int GetID() const {
    return TypeID<BasePacket>::GetID<BasePacket>();
  }

  void Hello() const {
    std::cout << "Hello\n";
  }
};

class DerivedPacket : public BasePacket {
 public:
  virtual int GetID() const {
    return TypeID<BasePacket>::GetID<DerivedPacket>();
  }

  void Introduce() const {
    std::cout << "I'm DerivedPacket\n";
  }
};

class Handler {
 private:
  using handler_func = std::function<void(Handler&, const BasePacket&)>;
 public:
  Handler() {
    handler_array_[TypeID<BasePacket>::GetID<BasePacket>()] = &Handler::HandleBase;
    handler_array_[TypeID<BasePacket>::GetID<DerivedPacket>()] = &Handler::HandleDerived;
  }

  void Handle(const BasePacket& packet) {
    handler_array_[packet.GetID()](*this, packet);
  }

 private:
  void HandleBase(const BasePacket& raw_packet) {
    const BasePacket& packet = raw_packet;
    packet.Hello();
  }

  void HandleDerived(const BasePacket& raw_packet) {
    const DerivedPacket& packet = static_cast<const DerivedPacket&>(raw_packet);
    packet.Introduce();
  }

 private:
  AutoArray<handler_func> handler_array_;
};

int main() {
  AutoArray<int> auto_array;
  auto_array[0] = 0;
  auto_array[5] = 5;
  auto_array[10] = 10;
  std::cout << auto_array[0] << ' ' << auto_array[5] << ' ' << auto_array[10]
            << '\n';
  BasePacket base_packet;
  DerivedPacket derived_packet;
  Handler handler;
  handler.Handle(base_packet);
  handler.Handle(derived_packet);
  return 0;
}

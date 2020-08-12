#ifndef _PACKET_HPP_
#define _PACKET_HPP_

#include <memory>
#include "pipert/Timer.h"
#include "src/PacketImp.h"

namespace pipert {

template <class T>
class Packet {
 public:
  Packet(const T& data);
  ~Packet() { }

  T GetData() const;
  T* GetDataPtr();
  Timer::Time GetCreatedTime() const;

 private:
  std::shared_ptr<PacketImp> imp_;
};

template <class T>
Packet<T>::Packet(const T& data) 
  : imp_(new PacketImp(reinterpret_cast<const Byte*>(std::addressof(data)), sizeof(T))) {}

template <class T>
T Packet<T>::GetData() const {
  return *(reinterpret_cast<T*>(imp_->GetData()));
}

template <class T>
T* Packet<T>::GetDataPtr() {
  return reinterpret_cast<T*>(imp_->GetData());
}

template <class T>
Timer::Time Packet<T>::GetCreatedTime() const {
  return imp_->GetCreatedTime();
}

}  // namespace pipert

#endif  //_PACKET_HPP_
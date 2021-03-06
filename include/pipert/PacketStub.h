#ifndef _PACKETSTUB_H_
#define _PACKETSTUB_H_

#include <cassert>
#include <utility>
#include "pipert/Channel.h"
#include "pipert/Packet.h"

namespace pipert {

template <class T>
class Channel;

/// A RAII object connecting a Packet and its containing Channel.
///
/// This object points to the connected Packet and Channel, so
/// the connected objects are never moved when this _stub_ is passed around.
/// Submitting this as a parameter or returning as a value is supported
/// through the implemented move operations.
///
/// \tparam T Follows the template paramter of Packet.
///           See template paramter T of Packet for details.
template <class T>
class PacketStub {
 public:
  PacketStub(const PacketStub&) = delete;
  PacketStub& operator=(const PacketStub&) = delete;
  PacketStub(PacketStub&&);
  PacketStub& operator=(PacketStub&&);

  /// This _stub_ is empty if the represented operation is done
  /// and nothing is connected anymore.
  /// \returns True if a Packet (and a Channel) is no longer connected.
  bool IsEmpty() const;

  Timer::Time timestamp() const;  ///< See PacketBase::timestamp()

  const T& data() const;  ///< See Packet::data()

  T& data();  ///< See Packet::data()

  /// Gets the Packet object which is connected to this PacketStub.
  /// \returns The referred Packet object or nullptr if this _stub_ is empty.
  Packet<T>* GetPacket();

 protected:
  PacketStub(Packet<T>* packet, Channel<T>* channel);
  void SetEmpty();  ///< Operation was finished, connection is over.

  Channel<T>* channel_;  ///< The connected Channel.
  Packet<T>* packet_;    ///< The connected Packet.

 private:
  void move(PacketStub&&);
};

template <class T>
PacketStub<T>::PacketStub(PacketStub&& o) {
  move(std::move(o));
}

template <class T>
PacketStub<T>& PacketStub<T>::operator=(PacketStub&& o) {
  move(std::move(o));
  return *this;
}

template <class T>
bool PacketStub<T>::IsEmpty() const {
  return packet_ == nullptr;
}

template <class T>
Timer::Time PacketStub<T>::timestamp() const {
  assert(!IsEmpty());
  return packet_->timestamp();
}

template <class T>
const T& PacketStub<T>::data() const {
  assert(!IsEmpty());
  return packet_->data();
}

template <class T>
T& PacketStub<T>::data() {
  assert(!IsEmpty());
  return packet_->data();
}

template <class T>
Packet<T>* PacketStub<T>::GetPacket() {
  return packet_;
}

template <class T>
PacketStub<T>::PacketStub(Packet<T>* packet, Channel<T>* channel)
    : channel_(channel), packet_(packet) {}

template <class T>
void PacketStub<T>::SetEmpty() {
  packet_ = nullptr;
}

template <class T>
void PacketStub<T>::move(PacketStub&& o) {
  channel_ = o.channel_;
  packet_ = o.packet_;
  o.packet_ = nullptr;
}

}  // namespace pipert

#endif  //_PACKETSTUB_H_
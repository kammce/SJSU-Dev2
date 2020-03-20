#pragma once

#include <array>

#include "L1_Peripheral/lpc40xx/pin.hpp"
#include "utility/status.hpp"
#include "utility/units.hpp"

namespace sjsu
{
/// An abstract interface for hardware that implements the Serial Peripheral
/// Interface (SPI) communication protocol.
/// @ingroup l1_peripheral
class Spi
{
 public:
  // ==============================
  // Interface Defintions
  // ==============================

  /// SSP data size for frame packets
  enum class DataSize : uint8_t
  {
    kFour = 0,  // The smallest standard frame sized allowed for SJSU-Dev2
    kFive,
    kSix,
    kSeven,
    kEight,
    kNine,
    kTen,
    kEleven,
    kTwelve,
    kThirteen,
    kFourteen,
    kFifteen,
    kSixteen,  // The largest standard frame sized allowed for SJSU-Dev2
  };

  // ==============================
  // Interface Methods
  // ==============================

  /// Initialize and enable hardware. This must be called before any other
  /// method in this interface is called.
  virtual Status_t Initialize() const = 0;
  /// Send a value via SPI and receive a value back from the serial port
  ///
  /// @param data - transfer data to external device via spi port
  ///
  /// @return byte read from the external device.
  virtual uint16_t Transfer(uint16_t data) const = 0;
  /// Set the number of bits to transmit over SPI
  ///
  /// @param size - number of bits to transmit over spi
  virtual void SetDataSize(DataSize size) const = 0;
  /// Set the clock frequency
  ///
  /// @param frequency - the clock rate in hertz to set the SPI device to.
  /// @param positive_clock_on_idle - if true, clock signal will be high when
  ///        nothing is being transmitted.
  /// @param read_miso_on_rising - if true, device will read the the MISO line
  ///        on rising edge of the clock line.
  virtual void SetClock(units::frequency::hertz_t frequency,
                        bool positive_clock_on_idle = false,
                        bool read_miso_on_rising    = false) const = 0;

  /// Transfer a std::array of data
  ///
  /// Usage:
  ///
  ///    std::array<uint8_t, 4> data = {1, 2, 3, 4};
  ///    spi.Transfer(data);
  ///
  /// @tparam T - deduced data type of the array. Must be less than or equal to
  ///             uint16_t.
  /// @tparam length - deduced length of the array.
  /// @param data - the array to be sent via SPI.
  /// @return std::array<T, length> - the results of the tranfer. The result can
  ///         be ignored with little cost to the program. C++20 performs copy
  ///         ellision, preventing a memcpy from occuring when the result is
  ///         returned.
  template <typename T, size_t length>
  std::array<T, length> Transfer(const std::array<T, length> & data)
  {
    // Compile time check that the datatype used is equal to or smaller than
    // datatype for Transfer. This will produce a better error message than the
    // generic template error message generated by the compiler.
    static_assert(sizeof(T) <= sizeof(uint16_t),
                  "Array datatype must be uint16_t or smaller.");

    std::array<T, length> result = { 0 };
    for (uint16_t i = 0; i < length; i++)
    {
      result[i] = static_cast<T>(Transfer(data[i]));
    }
    return result;
  }
};
}  // namespace sjsu

#include <cstdint>

#include "L0_Platform/stm32f10x/stm32f10x.h"
#include "L1_Peripheral/stm32f10x/pin.hpp"
#include "L4_Testing/testing_frameworks.hpp"

namespace sjsu::stm32f10x
{
EMIT_ALL_METHODS(Pin);

namespace
{
bit::Mask Mask4Bit(const sjsu::Pin & pin)
{
  return {
    .position = static_cast<uint32_t>(pin.GetPin() * 4),
    .width    = 4,
  };
}
}  // namespace

TEST_CASE("Testing stm32f10x Pin", "[stm32f10x-pin]")
{
  Mock<SystemController> mock_system_controller;
  Fake(Method(mock_system_controller, PowerUpPeripheral));
  SystemController::SetPlatformController(&mock_system_controller.get());

  GPIO_TypeDef local_gpio_a;
  GPIO_TypeDef local_gpio_b;
  GPIO_TypeDef local_gpio_c;
  GPIO_TypeDef local_gpio_d;
  GPIO_TypeDef local_gpio_e;
  GPIO_TypeDef local_gpio_f;
  GPIO_TypeDef local_gpio_g;
  GPIO_TypeDef local_gpio_h;
  GPIO_TypeDef local_gpio_i;
  AFIO_TypeDef local_afio;

  memset(&local_gpio_a, 0, sizeof(local_gpio_a));
  memset(&local_gpio_b, 0, sizeof(local_gpio_b));
  memset(&local_gpio_c, 0, sizeof(local_gpio_c));
  memset(&local_gpio_d, 0, sizeof(local_gpio_d));
  memset(&local_gpio_e, 0, sizeof(local_gpio_e));
  memset(&local_gpio_f, 0, sizeof(local_gpio_f));
  memset(&local_gpio_g, 0, sizeof(local_gpio_g));
  memset(&local_gpio_h, 0, sizeof(local_gpio_h));
  memset(&local_gpio_i, 0, sizeof(local_gpio_i));
  memset(&local_afio, 0, sizeof(local_afio));

  Pin::gpio[0] = &local_gpio_a;
  Pin::gpio[1] = &local_gpio_b;
  Pin::gpio[2] = &local_gpio_c;
  Pin::gpio[3] = &local_gpio_d;
  Pin::gpio[4] = &local_gpio_e;
  Pin::gpio[5] = &local_gpio_f;
  Pin::gpio[6] = &local_gpio_g;
  Pin::afio    = &local_afio;

  stm32f10x::Pin pin_a0('A', 0);    // A
  stm32f10x::Pin pin_a4('A', 4);    // Middle of first half word
  stm32f10x::Pin pin_b0('B', 0);    // B
  stm32f10x::Pin pin_b7('B', 7);    // End of first half word
  stm32f10x::Pin pin_c0('C', 0);    // C
  stm32f10x::Pin pin_c8('C', 8);    // First of last half word
  stm32f10x::Pin pin_d0('D', 0);    // D
  stm32f10x::Pin pin_d12('D', 12);  // Middle of last half word
  stm32f10x::Pin pin_e0('E', 0);    // E
  stm32f10x::Pin pin_e15('E', 15);  // Last of last half word
  stm32f10x::Pin pin_f0('F', 0);    // F
  stm32f10x::Pin pin_g0('G', 0);    // G

  auto power_up_matcher = [](sjsu::SystemController::PeripheralID expected_id) {
    return [expected_id](sjsu::SystemController::PeripheralID actual_id) {
      return expected_id.device_id == actual_id.device_id;
    };
  };

  struct TestStruct_t
  {
    sjsu::Pin & pin;
    GPIO_TypeDef & reg;
    SystemController::PeripheralID id;
  };

  std::array<TestStruct_t, 12> test = {
    TestStruct_t{
        // 0
        .pin = pin_a0,
        .reg = local_gpio_a,
        .id  = stm32f10x::SystemController::Peripherals::kGpioA,
    },
    TestStruct_t{
        // 1
        .pin = pin_a4,
        .reg = local_gpio_a,
        .id  = stm32f10x::SystemController::Peripherals::kGpioA,
    },
    TestStruct_t{
        // 2
        .pin = pin_b0,
        .reg = local_gpio_b,
        .id  = stm32f10x::SystemController::Peripherals::kGpioB,
    },
    TestStruct_t{
        // 3
        .pin = pin_b7,
        .reg = local_gpio_b,
        .id  = stm32f10x::SystemController::Peripherals::kGpioB,
    },
    TestStruct_t{
        // 4
        .pin = pin_c0,
        .reg = local_gpio_c,
        .id  = stm32f10x::SystemController::Peripherals::kGpioC,
    },
    TestStruct_t{
        // 5
        .pin = pin_c8,
        .reg = local_gpio_c,
        .id  = stm32f10x::SystemController::Peripherals::kGpioC,
    },
    TestStruct_t{
        // 6
        .pin = pin_d0,
        .reg = local_gpio_d,
        .id  = stm32f10x::SystemController::Peripherals::kGpioD,
    },
    TestStruct_t{
        // 7
        .pin = pin_d12,
        .reg = local_gpio_d,
        .id  = stm32f10x::SystemController::Peripherals::kGpioD,
    },
    TestStruct_t{
        // 8
        .pin = pin_e0,
        .reg = local_gpio_e,
        .id  = stm32f10x::SystemController::Peripherals::kGpioE,
    },
    TestStruct_t{
        // 9
        .pin = pin_e15,
        .reg = local_gpio_e,
        .id  = stm32f10x::SystemController::Peripherals::kGpioE,
    },
    TestStruct_t{
        // 10
        .pin = pin_f0,
        .reg = local_gpio_f,
        .id  = stm32f10x::SystemController::Peripherals::kGpioF,
    },
    TestStruct_t{
        // 11
        .pin = pin_g0,
        .reg = local_gpio_g,
        .id  = stm32f10x::SystemController::Peripherals::kGpioG,
    },
  };

  SECTION("Initialize()")
  {
    for (size_t i = 0; i < test.size(); i++)
    {
      // Setup
      INFO("Failure on test index: " << i);

      // Exercise
      test[i].pin.Initialize();

      // Verify
      Verify(Method(mock_system_controller, PowerUpPeripheral)
                 .Matching(power_up_matcher(test[i].id)));
      // Cleanup
      mock_system_controller.ClearInvocationHistory();
    }
  }

  SECTION("SetPinFunction()")
  {
    SECTION("Gpio full speed")
    {
      constexpr uint8_t kGpioFullSpeedCode = 0b0011;

      for (size_t i = 0; i < test.size(); i++)
      {
        INFO("Set as GPIO: Failure on test index: " << i);
        // Setup
        // Setup: Fill with 1s so that by setting it to input they get replaced
        //        with the correct input code of zero.
        test[i].reg.CRL = 0xFFFF'FFFF;
        test[i].reg.CRH = 0xFFFF'FFFF;

        // Exercise
        test[i].pin.SetPinFunction(0);
        // Exercise: Combine the two registers into 1 variable to make
        //           extraction easier.
        uint64_t crh = test[i].reg.CRH;
        uint64_t crl = test[i].reg.CRL;
        uint64_t cr  = (crh << 32) | crl;

        // Verify
        CHECK(bit::Extract(cr, Mask4Bit(test[i].pin)) == kGpioFullSpeedCode);
      }
    }

    SECTION("Alternative function full speed")
    {
      constexpr uint8_t kAlternativeFullSpeedCode = 0b1011;

      for (size_t i = 0; i < test.size(); i++)
      {
        INFO("Set as Alternative: Failure on test index: " << i);
        // Setup
        // Setup: Fill with 1s so that by setting it to input they get replaced
        //        with the correct input code of zero.
        test[i].reg.CRL = 0xFFFF'FFFF;
        test[i].reg.CRH = 0xFFFF'FFFF;

        // Exercise
        test[i].pin.SetPinFunction(1);
        // Exercise: Combine the two registers into 1 variable to make
        //           extraction easier.
        uint64_t crh = test[i].reg.CRH;
        uint64_t crl = test[i].reg.CRL;
        uint64_t cr  = (crh << 32) | crl;

        // Verify
        CHECK(bit::Extract(cr, Mask4Bit(test[i].pin)) ==
              kAlternativeFullSpeedCode);
      }
    }

    SECTION("Invalid function")
    {
      for (size_t i = 0; i < test.size(); i++)
      {
        // Exercise & Verify
        CHECK(test[i].pin.SetPinFunction(0b10).error().status ==
              Status::kInvalidParameters);
      }
    }
  }

  SECTION("SetPull()")
  {
    constexpr uint8_t kPullDownCode = 0b1000;
    constexpr uint8_t kFloating     = 0b0100;

    for (size_t i = 0; i < test.size(); i++)
    {
      // Setup
      INFO("Failure on test index: " << i);
      // Setup: Fill with 1s so that by setting it to input they get replaced
      //        with the correct input code of zero.
      test[i].reg.CRL = 0xFFFF'FFFF;
      test[i].reg.CRH = 0xFFFF'FFFF;

      {
        // Exercise
        test[i].pin.SetFloating();

        // Exercise: Combine the two registers into 1 variable to make
        //           extraction easier.
        uint64_t crh = test[i].reg.CRH;
        uint64_t crl = test[i].reg.CRL;
        uint64_t cr  = (crh << 32) | crl;

        // Verify
        CHECK(bit::Extract(cr, Mask4Bit(test[i].pin)) == kFloating);
      }

      {
        // Exercise
        test[i].pin.PullUp();

        // Exercise: Combine the two registers into 1 variable to make
        //           extraction easier.
        uint64_t crh = test[i].reg.CRH;
        uint64_t crl = test[i].reg.CRL;
        uint64_t cr  = (crh << 32) | crl;
        uint32_t odr = test[i].reg.ODR;

        // Verify
        CHECK(bit::Extract(cr, Mask4Bit(test[i].pin)) == kPullDownCode);
        // Verify: ODR bit is 1 for pull up
        CHECK(bit::Read(odr, test[i].pin.GetPin()));
      }

      {
        // Exercise
        test[i].pin.PullDown();

        // Exercise: Combine the two registers into 1 variable to make
        //           extraction easier.
        uint64_t crh = test[i].reg.CRH;
        uint64_t crl = test[i].reg.CRL;
        uint64_t cr  = (crh << 32) | crl;
        uint32_t odr = test[i].reg.ODR;

        // Verify
        CHECK(bit::Extract(cr, Mask4Bit(test[i].pin)) == kPullDownCode);
        // Verify: ODR bit is 0 for pull up
        CHECK(!bit::Read(odr, test[i].pin.GetPin()));
      }

      {
        // Exercise & Verify
        CHECK(test[i].pin.SetPull(Pin::Resistor::kRepeater).error().status ==
              Status::kInvalidSettings);
      }
    }
  }

  SECTION("SetAsOpenDrain()")
  {
    for (size_t i = 0; i < test.size(); i++)
    {
      // Setup
      INFO("Failure on test index: " << i);

      {
        // Setup: Fill with 1s so that by setting it to input they get replaced
        //        with the correct input code of zero.
        test[i].reg.CRL = 0;
        test[i].reg.CRH = 0;

        // Exercise
        test[i].pin.SetAsOpenDrain(true);

        // Exercise: Combine the two registers into 1 variable to make
        //           extraction easier.
        uint64_t crh = test[i].reg.CRH;
        uint64_t crl = test[i].reg.CRL;
        uint64_t cr  = (crh << 32) | crl;

        // Verify
        // Verify: Shifting bit position by 2 in order to put the read position
        //         of on the CNF0 portion of the mask.
        CHECK(bit::Read(cr, Mask4Bit(test[i].pin) << 2));
      }

      {
        // Setup: Fill with 1s so that by setting it to input they get replaced
        //        with the correct input code of zero.
        test[i].reg.CRL = 0xFFFF'FFFF;
        test[i].reg.CRH = 0xFFFF'FFFF;

        // Exercise
        test[i].pin.SetAsOpenDrain(false);

        // Exercise: Combine the two registers into 1 variable to make
        //           extraction easier.
        uint64_t crh = test[i].reg.CRH;
        uint64_t crl = test[i].reg.CRL;
        uint64_t cr  = (crh << 32) | crl;

        // Verify
        // Verify: Shifting bit position by 2 in order to put the read position
        //         of on the CNF0 portion of the mask.
        CHECK(!bit::Read(cr, Mask4Bit(test[i].pin) << 2));
      }
    }
  }

  SECTION("SetAsAnalogMode()")
  {
    constexpr uint8_t kAnalogCode = 0b0100;
    for (size_t i = 0; i < test.size(); i++)
    {
      // Setup
      INFO("Failure on test index: " << i);

      {
        // Setup
        // Setup: Fill with 1s so that by setting it to input they get replaced
        //        with the correct input code of zero.
        test[i].reg.CRL = 0xFFFF'FFFF;
        test[i].reg.CRH = 0xFFFF'FFFF;

        // Exercise
        test[i].pin.SetAsAnalogMode();
        // Exercise: Combine the two registers into 1 variable to make
        //           extraction easier.
        uint64_t crh = test[i].reg.CRH;
        uint64_t crl = test[i].reg.CRL;
        uint64_t cr  = (crh << 32) | crl;

        // Verify
        CHECK(kAnalogCode == bit::Extract(cr, Mask4Bit(test[i].pin)));
      }
    }
  }

  SECTION("ReleaseJTAGPins()")
  {
    // Setup
    local_afio.MAPR = 0;

    // Exercise
    Pin::ReleaseJTAGPins();

    // Verify
    Verify(Method(mock_system_controller, PowerUpPeripheral)
               .Matching(power_up_matcher(
                   stm32f10x::SystemController::Peripherals::kAFIO)));

    // Set the JTAG Release
    CHECK(0b010 == sjsu::bit::Extract(local_afio.MAPR,
                                      sjsu::bit::CreateMaskFromRange(24, 26)));
  }

  Pin::gpio[0] = GPIOA;
  Pin::gpio[1] = GPIOB;
  Pin::gpio[2] = GPIOC;
  Pin::gpio[3] = GPIOD;
  Pin::gpio[4] = GPIOE;
  Pin::gpio[5] = GPIOF;
  Pin::gpio[6] = GPIOG;
  Pin::afio    = AFIO;
}
}  // namespace sjsu::stm32f10x

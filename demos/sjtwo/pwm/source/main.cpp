#include "L1_Peripheral/lpc40xx/gpio.hpp"
#include "L1_Peripheral/lpc40xx/pwm.hpp"
#include "utility/log.hpp"
#include "utility/time.hpp"

int main()
{
  sjsu::LogInfo("Pwm Application Starting...");

  sjsu::LogInfo("Creating Gpio powered LED...P4.28");
  sjsu::LogInfo("This Gpio powered LED represents the maximum brightness");
  sjsu::LogInfo("and is used as reference against the Pwm powered LED");
  sjsu::LogInfo("during its sweep.");
  sjsu::LogInfo("Connect a 470 ohm resistor between the power line and");
  sjsu::LogInfo("the anode of the LED. The cathode is then connected to");
  sjsu::LogInfo("P4.28 and P2.0(Pwm1).");
  sjsu::lpc40xx::Gpio p4_28(4, 28);
  p4_28.SetAsOutput();
  p4_28.SetLow();

  sjsu::LogInfo("Creating Pwm powered LED...P2.0 (Pwm1)");
  sjsu::lpc40xx::Pwm p2_0(sjsu::lpc40xx::Pwm::Channel::kPwm0);
  // Initialize Pwm at 1 kHz
  p2_0.Initialize(1_kHz);
  float duty = 0;

  while (true)
  {
    p2_0.SetFrequency(1_kHz);
    for (int i = 0; i <= 255; i++)
    {
      duty = static_cast<float>(i) / 255.0f;
      p2_0.SetDutyCycle(duty);
      sjsu::Delay(10ms);
    }

    p2_0.SetDutyCycle(0.5);
    units::frequency::hertz_t frequency = 1_Hz;
    while (frequency < 20'000_Hz)
    {
      frequency = frequency * 2;
      p2_0.SetFrequency(frequency);
      sjsu::Delay(500ms);
    }

    while (frequency > 5_Hz)
    {
      frequency = frequency / 2;
      p2_0.SetFrequency(frequency);
      sjsu::Delay(500ms);
    }
  }
  return 0;
}

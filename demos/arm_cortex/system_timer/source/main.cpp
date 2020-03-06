#include "L1_Peripheral/cortex/system_timer.hpp"
#include "L1_Peripheral/inactive.hpp"
#include "L1_Peripheral/system_controller.hpp"
#include "utility/log.hpp"

void DemoSystemIsr()
{
  static int counter = 0;
  sjsu::LogInfo("System Timer Tick #%d ...", counter++);
}

int main()
{
  sjsu::LogInfo("System Timer Application Starting...");
  sjsu::cortex::SystemTimer system_timer;

  system_timer.SetCallback(DemoSystemIsr);
  system_timer.SetTickFrequency(10_Hz);
  system_timer.StartTimer();

  sjsu::LogInfo("Halting any action.");
  return 0;
}

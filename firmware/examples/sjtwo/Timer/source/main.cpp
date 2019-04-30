#include "L1_Peripheral/lpc40xx/timer.hpp"
#include "utility/log.hpp"
#include "utility/time.hpp"
// Put whatever functions you want to be triggered within these ISR's
// and you can use the timer to trigger whatever event you want.
void Timer0ISR(void)
{
  LOG_INFO("Successfully triggered interrupt from timer 0");
}
void Timer1ISR(void)
{
  LOG_INFO("Successfully triggered interrupt from timer 1");
}
void Timer2ISR(void)
{
  LOG_INFO("Successfully triggered interrupt from timer 2");
}
void Timer3ISR(void)
{
  LOG_INFO("Successfully triggered interrupt from timer 3");
}

// In this example all timers are initialized to 1 MHz, or 1 micro second per
// timer register count.
int main()
{
  LOG_INFO("Timer Application Starting...");

  sjsu::lpc40xx::Timer timer0(sjsu::lpc40xx::Timer::Channel::kTimer0);
  sjsu::lpc40xx::Timer timer1(sjsu::lpc40xx::Timer::Channel::kTimer1);
  sjsu::lpc40xx::Timer timer2(sjsu::lpc40xx::Timer::Channel::kTimer2);
  sjsu::lpc40xx::Timer timer3(sjsu::lpc40xx::Timer::Channel::kTimer3);

  timer0.Initialize(1'000'000, Timer0ISR);
  timer1.Initialize(1'000'000, Timer1ISR);
  timer2.Initialize(1'000'000, Timer2ISR);
  timer3.Initialize(1'000'000, Timer3ISR);

  timer0.SetTimer(1'000'000, sjsu::lpc40xx::Timer::kInterruptRestart);
  timer1.SetTimer(3'000'000, sjsu::lpc40xx::Timer::kInterruptRestart);
  timer2.SetTimer(5'000'000, sjsu::lpc40xx::Timer::kInterruptRestart);
  timer3.SetTimer(10'000'000, sjsu::lpc40xx::Timer::kInterruptRestart);

  sjsu::Halt();
  return 0;
}

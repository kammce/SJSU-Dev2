// SystemTimer abstracts the process of changing enabling and setting
// up the SystemTimer.
//
//   Usage:
//      PinConfigure P0_0(0, 0);
//      P0_0.SetAsActiveLow();
//      P0_0.SetPinMode(PinConfigureInterface::PinMode::kPullUp);
#pragma once

#include <cstdio>

#include "L0_LowLevel/LPC40xx.h"
#include "L2_Utilities/macros.hpp"
#include "config.hpp"

// LPC4076 does not include P3.26 so supporting methods are not available
class SystemTimerInterface
{
   public:
    virtual void SetIsrFunction(void (*isr)(void)) = 0;
    virtual bool StartTimer() = 0;
    virtual void DisableTimer() = 0;
    virtual float SetTickFrequency(uint32_t frequency) = 0;
};

class SystemTimer : public SystemTimerInterface
{
   public:
    // Source: "UM10562 LPC408x/407x User manual" table 83 page 132
    enum ControlBitMap : uint8_t
    {
        kEnableCounter = 0,
        kTickInterupt  = 1,
        kClkSource     = 2,
        kCountFlag     = 16
    };
    static void (*system_timer_isr)(void);
    static SysTick_Type * sys_tick;

    constexpr SystemTimer() {}
    void SetIsrFunction(void (*isr)(void)) final override
    {
        system_timer_isr = isr;
    }
    bool StartTimer() final override
    {
        bool successful = false;
        if(sys_tick->LOAD != 0)
        {
            sys_tick->VAL   = 0UL;
            sys_tick->CTRL |= (1 << ControlBitMap::kTickInterupt);
            sys_tick->CTRL |= (1 << ControlBitMap::kEnableCounter);
            sys_tick->CTRL |= (1 << ControlBitMap::kClkSource);
            successful = true;
        }
        return successful;
    }
    // WARNING: doing so will most likely disable FreeRTOS
    void DisableTimer() final override
    {
        sys_tick->CTRL &= ~(1 << ControlBitMap::kTickInterupt);
        sys_tick->CTRL &= ~(1 << ControlBitMap::kEnableCounter);
    }
    // @param frequency set the frequency that SystemTick counter will run.
    //        If it is above the maximum SystemTick value 2^24
    //        [SysTick_LOAD_RELOAD_Msk], the value is ceiled to
    //        SysTick_LOAD_RELOAD_Msk
    float SetTickFrequency(uint32_t frequency) final override
    {
        uint32_t reload_value;
        float remainder;
        if((frequency - 1) > SysTick_LOAD_RELOAD_Msk)
        {
            reload_value = SysTick_LOAD_RELOAD_Msk;
            remainder = SysTick_LOAD_RELOAD_Msk;
        }
        else
        {
            remainder = (SJ2_SYSTEM_CLOCK/static_cast<float>(frequency)) - 1;
            reload_value = SJ2_SYSTEM_CLOCK/frequency - 1;
            remainder = remainder - static_cast<float>(reload_value);
        }
        // TODO: change SJ2_SYSTEM_CLOCK macro to an inline function that gives
        sys_tick->LOAD = reload_value;
        return remainder;

    }
};


// =============================================================================
// Example HAL Tests
// =============================================================================
#include "L2_HAL/test/memory_access_protocol_test.cpp"  // NOLINT
#include "L2_HAL/test/example_test.cpp"                 // NOLINT

// =============================================================================
// Audio
// =============================================================================
#include "L2_HAL/audio/test/buzzer_test.cpp"  // NOLINT

// =============================================================================
// Communication
// =============================================================================
#include "L2_HAL/communication/test/tsop752_test.cpp"  // NOLINT

// =============================================================================
// Displays
// =============================================================================
#include "L2_HAL/displays/lcd/test/st7066u_test.cpp"   // NOLINT
#include "L2_HAL/displays/oled/test/ssd1306_test.cpp"  // NOLINT

// =============================================================================
// I/O
// =============================================================================
#include "L2_HAL/io/test/parallel_bus_test.cpp"                // NOLINT
#include "L2_HAL/io/parallel_bus/test/parallel_gpio_test.cpp"  // NOLINT

// =============================================================================
// Sensor/Optical
// =============================================================================
// #include "L2_HAL/sensors/optical/test/apds9960_test.cpp"  // NOLINT

// =============================================================================
// Sensor/Environment
// =============================================================================
#include "L2_HAL/sensors/environment/light/test/temt6000x01_test.cpp"  // NOLINT

// =============================================================================
// Sensor/Temperature
// =============================================================================hpp
#include "L2_HAL/sensors/environment/temperature/test/si7060_test.cpp"  // NOLINT
#include "L2_HAL/sensors/environment/temperature/test/tmp102_test.cpp"  // NOLINT

// =============================================================================
// Sensor/Movement
// =============================================================================
#include "L2_HAL/sensors/movement/accelerometer/test/mma8452q_test.cpp"  // NOLINT

// =============================================================================
// Sensor/Distance
// =============================================================================
#include "L2_HAL/sensors/distance/time_of_flight/test/tfmini_test.cpp"  // NOLINT

// =============================================================================
// Switches
// =============================================================================
#include "L2_HAL/switches/test/button_test.cpp"  // NOLINT

// =============================================================================
// Memory
// =============================================================================
#include "L2_HAL/memory/test/sd_test.cpp"  // NOLINT

// =============================================================================
// Actuators
// =============================================================================
#include "L2_HAL/actuators/servo/test/servo_test.cpp"  // NOLINT

// =============================================================================
// Battery
// =============================================================================
#include "L2_HAL/sensors/battery/test/ltc4150_test.cpp"  // NOLINT

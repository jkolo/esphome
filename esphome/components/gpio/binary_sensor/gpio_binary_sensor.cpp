#include "gpio_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace gpio {

static const char *TAG = "gpio.binary_sensor";

void GPIOBinarySensor::setup() {
  this->pin_->setup();
  this->publish_initial_state(this->read_state());
}

void GPIOBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "GPIO Binary Sensor", this);
  LOG_PIN("  Pin: ", this->pin_);
}

void GPIOBinarySensor::loop() { this->publish_state(this->read_state()); }

float GPIOBinarySensor::get_setup_priority() const { return setup_priority::HARDWARE; }

bool GPIOBinarySensor::read_state() {
  bool pin_state = this->pin_->digital_read();
    
  if (this->frequency_ > 0.0f) {
    int wait_time = 1000.0f / this->frequency_ * 0.6f;
    
    unsigned long start = millis();
    while (millis() - start < wait_time && pin_state) {
      delay(1);
      pin_state = pin_->digital_read();
    }
  }

  return pin_state;
}

}  // namespace gpio
}  // namespace esphome

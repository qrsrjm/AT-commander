#include "atcommander.h"

#include "WProgram.h"
#include <stdarg.h>

extern const AtCommanderPlatform AT_PLATFORM_RN42;

bool configured = false;
AtCommanderConfig config;

void write(void* device, uint8_t byte) {
    ((HardwareSerial*)device)->write(byte);
}

void begin(void* device, int baud) {
    ((HardwareSerial*)device)->begin(baud);
}

int read(void* device) {
    return ((HardwareSerial*)device)->read();
}

void debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char message[512];
    vsnprintf(message, 512, format, args);
    Serial.print(message);
    va_end(args);
}

void setup() {
    Serial.begin(9600);
    Serial.println("About to change baud rate of RN-42 to 115200");

    // Serial 1, pins 18 and 19 on the chipKIT, is where the RN-42 is attached
    config.platform = AT_PLATFORM_RN42;
    config.device = &Serial1;
    config.baud_rate_initializer = begin;
    config.write_function = write;
    config.read_function = read;
    config.delay_function = delay;
    config.log_function = debug;
}

void loop() {
    if(!configured) {
        if(at_commander_set_baud(&config, 115200)) {
            configured = true;
            at_commander_reboot(&config);
        } else {
            delay(5000);
        }
    } else {
        Serial1.println("Sending data over the RN-42 at 115200 baud!");
    }

}

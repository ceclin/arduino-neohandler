#include <Arduino.h>

#include <neohandler.h>

using neohandler::CallbackLoopHandler;

CallbackLoopHandler handler(1000, [] {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
});

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    handler.CallIfUpdated();
}

#include <Arduino.h>
#include <FS.h>

#include "config.h"
#include "WiFiManager.h"
#include "webServer.h"
#include "updater.h"
#include "fetch.h"

unsigned long prev_time_task;

void setup() 
{
    Serial.begin(115200);

    SPIFFS.begin();
    GUI.begin();
    WiFiManager.begin(PSTR(PROJECT_NAME));
    fetch.begin();
}

void loop() 
{
    WiFiManager.loop();
    updater.loop();

    //background task
    if (millis() - prev_time_task > 10000)
    {
        prev_time_task = millis();

        //do task
        Serial.println(ESP.getFreeHeap());
        
        fetch.GET("http://neverssl.com");

        while (fetch.busy())
        {
            if (fetch.available())
            {
                Serial.write(fetch.read());           
            }
        }
        
        fetch.clean();
    }
}
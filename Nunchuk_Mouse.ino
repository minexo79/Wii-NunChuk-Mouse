#include <Wire.h>
#include "Mouse.h"
#include "Nunchuk.h"

// #define DEBUG_EN

#define interval_ms 10                            // 設定間隔時間
unsigned long currentMillis = 0, preMillis = 0;   // 現在及前次時間
short joyx = 0, joyy = 0;
char is_scroll = 0;

void setup() {
#ifdef DEBUG_EN
    Serial.begin(9600);
#endif
    Wire.begin();
    Mouse.begin();

     // Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
    Wire.setClock(100000);

    // nunchuk_init_power(); // A1 and A2 is power supply
    nunchuk_init();
}

  void loop() {
    currentMillis = millis();                       // 獲取現在時間
    if(currentMillis - preMillis >= interval_ms)    // 比較前次時間，需要兩者之差大於或相等設定間隔
    {
        preMillis = currentMillis;                  // 前次時間更改
        
        if (nunchuk_read()) {
            // Work with nunchuk_data
//            nunchuk_print();

            joyx = (nunchuk_joystickX());
            joyy = (nunchuk_joystickY() * -1);

#ifdef DEBUG_EN
            Serial.print(joyx);
            Serial.print(',');
            Serial.println(joyy);
#endif

            // use map instead multiply, thanks will!
            joyx = map(joyx, -100, 100, -40, 40);
            joyy = map(joyy, -100, 100, -40, 40);

            if((joyx > 2 || joyx < -2) || (joyy > 2 || joyy < -2))
            {
                if(nunchuk_buttonC())
                {
                    is_scroll = 1;
                    if(joyy > 0)
                        Mouse.move(0, 0, 1);
                    else if(joyy < 0)
                        Mouse.move(0, 0, -1);
                }
                else
                {
                    is_scroll = 0;
                    Mouse.move(joyx, joyy, 0);
                }
            }
            
            if(nunchuk_buttonC() && !is_scroll)        // LEFT Button -> C
                Mouse.press(MOUSE_LEFT);
            else
                Mouse.release(MOUSE_LEFT);
        
            if(nunchuk_buttonZ())                      // RIGHT Button -> Z   
                Mouse.press(MOUSE_RIGHT);
            else
                Mouse.release(MOUSE_RIGHT);
        }
    }
} 

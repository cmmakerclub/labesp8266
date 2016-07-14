/******************************************************************************
* Project  : NECTEC IoT Camp 2016                                             
* Compiler : Arduino 1.6.7                                                    
* Board    : ESPresso Lite V2                                                 
* Device   : LCD i2c 16x2                                                     
* Author   : Chiang Mai Maker Club                                            
*******************************************************************************/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x3f for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f, 16, 2);

void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello form :    ");
  lcd.setCursor(0, 1);
  lcd.print("ESPresso lite v2");
}

void loop()
{
  // Do nothing here...
}

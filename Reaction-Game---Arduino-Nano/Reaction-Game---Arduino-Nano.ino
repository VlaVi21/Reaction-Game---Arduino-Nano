#define LED_PIN 5    //пин светодиодной ленты
#define LED_NUM 15   // количество светодиодов

#include "FastLED.h" //библиотека для упраления адресной светодиодной ленты
CRGB leds[LED_NUM];

#define B1_PIN 4      // пин кнопки 
#define BUZZ_PIN 7    // пин пищалки

// Мини класс кнопки //
#define BTN_DEB 20   // дебаунс, мс
struct Button {
  public:
    Button (byte pin) {
      _pin = pin;
      pinMode(_pin, INPUT_PULLUP);
    }
    bool click() {
      bool btnState = digitalRead(_pin);
      if (!btnState && !_flag && millis() - _tmr >= BTN_DEB) {
        _flag = true;
        _tmr = millis();
        return true;
      }
      if (btnState && _flag && millis() - _tmr >= BTN_DEB) {
        _flag = false;
        _tmr = millis();
      }
      return false;
    }
    uint32_t _tmr;
    byte _pin;
    bool _flag;
};

Button b1(B1_PIN);
void setup() {
  Serial.begin(9600);
  FastLED.addLeds< WS2812, LED_PIN, GRB>(leds, LED_NUM); //инициализация адресной светодиодной ленты
  FastLED.setBrightness(50); //яркость ленты

  pinMode(BUZZ_PIN, OUTPUT); //*пищалка*
}


byte center; //переменная *красного* светодиода (который "ловит" зелёный светодиод)
byte counter; //переменная зелёного светодиода

void loop() {
  FastLED.clear(); //очитска ленты 
  leds[center] = CRGB::Red; //масивы для светодиодов leds[переменная светодиода] = CRGB::цвет - https://github.com/FastLED/FastLED/wiki/Pixel-reference#colors
  leds[counter] = CRGB::Chartreuse;

  ruN(); //
  //вызываем функции для работы игры
  but();//

  FastLED.show(); //вывод цветов на ленту
}

uint32_t tmr; //переменная для таймера 

byte speeD = 200; //переменная для начальной скорости
byte val = 10; //на сколько будем уменьшать скорость 
//вы программист-меняйте и делайте как вам удобно

void but() { //функция отработки нажания кнопки
  if (b1.click()) {  //если кнопка нажата.....                          
    if (counter == center) {  //сравниваем положение зелёного и красного светодиода 
      tone(BUZZ_PIN, 2000, 500); //звучание *пищалки*
       speeD -= val; //уменьшаем скорость
     if(speeD <= 0) { //если скорость меньше 0 ,снова возобновляем начальную скорость
        speeD = 200;
       }
      center = random(LED_NUM); //случайное  положение красного светодиода
      Serial.println("Good");
      //Serial.println(speeD);
      //Serial.println(center);
    }
    else if (counter > center) {
      tone(BUZZ_PIN, 2000, 50);
      Serial.println("Fail2");
    }
    else if (counter < center) {
      tone(BUZZ_PIN, 1200, 50);
      Serial.println("Fail");
    }
  }
}

void ruN() { //движение зелёного светодиода
  if (millis() - tmr >= speeD) {
    tmr = millis();
    if (++counter >= LED_NUM) counter = 0;
  }
}

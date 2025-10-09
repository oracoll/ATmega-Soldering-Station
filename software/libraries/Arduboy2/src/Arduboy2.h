#ifndef Arduboy2_h
#define Arduboy2_h

#include <Arduino.h>
#include <Print.h>
#include <EEPROM.h>

#define ARDUBOY_LIB_VER 60000

#define BLACK 0
#define WHITE 1
#define INVERT 2

#define WIDTH 128
#define HEIGHT 64

class Arduboy2 : public Print
{
public:
  Arduboy2();
  void begin();
  void clear();
  void display();
  void drawPixel(int16_t x, int16_t y, uint8_t color);
  void drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color);
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
  void drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color);
  void fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color);
  void fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color);
  void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);
  void drawFastVLine(int16_t x, int16_t y, int8_t h, uint8_t color);
  void drawFastHLine(int16_t x, int16_t y, int8_t w, uint8_t color);
  void drawBitmapResize(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t size, uint8_t color);
  void setCursor(int16_t x, int16_t y);
  void setTextColor(uint8_t color);
  void setTextBackground(uint8_t bg);
  void setTextSize(uint8_t s);
  virtual size_t write(uint8_t);
  void setFrameRate(uint8_t rate);
  bool nextFrame();
  void sendLCDCommand(uint8_t command);
  void invert(bool inverse);

private:
  uint8_t *sBuffer;
  uint8_t cursor_x;
  uint8_t cursor_y;
  uint8_t text_color;
  uint8_t text_bg_color;
  uint8_t text_size;
  uint32_t frame_last_millis;
  uint16_t frame_duration_micros;
};

#endif
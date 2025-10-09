/*
  Arduboy2_SH1106.h  -- Header-only lightweight Arduboy2-like layer
  Target: ATmega328P (Uno/Nano)
  Display: SH1106 128x64 I2C via U8g2 (full buffer)
  Controls: Rotary encoder A=D7, B=D8, SW=D6
  Backend: U8g2 library (https://github.com/olikraus/u8g2)

  Usage:
    1) Put this single file into your Arduino/libraries/Arduboy2_SH1106/ folder
       or directly into your sketch folder.
    2) Install U8g2 library in Library Manager.
    3) #include <Arduboy2_SH1106.h>
    4) Use `Arduboy2 arduboy;` (global instance provided). Call arduboy.begin();

  Notes:
    - This intentionally omits audio/LED/timer code from Arduboy2.
    - Encoder is polled by calling arduboy.pollButtons(); typically once per loop().
    - Methods provided: begin(), clear(), display(), drawPixel(), drawBitmap(),
      setCursor(), print(), println(), just like a minimal Arduboy layer.
*/

#ifndef ARDUBOY2_SH1106_H
#define ARDUBOY2_SH1106_H

#include <Arduino.h>
#include <Print.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <avr/pgmspace.h>


// Using full buffer constructor for simplest semantics (draw then display)
static U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


// ---------- Arduboy2-like class (minimal) ----------
class Arduboy2 : public Print {
public:
  Arduboy2() {}

  void begin() {
    u8g2.begin();
    u8g2.setFont(u8g2_font_5x7_tf); // Default font
  }

  // Graphics helpers
  void clear() { u8g2.clearBuffer(); }
  void display() { u8g2.sendBuffer(); }

  // Basic drawing
  void drawPixel(int16_t x, int16_t y, uint8_t color) {
    if (color == 1) u8g2.drawPixel(x, y);
  }

  void drawFastVLine(int16_t x, int16_t y, int8_t h, uint8_t color) {
    u8g2.setDrawColor(color);
    u8g2.drawVLine(x, y, h);
  }

  void drawFastHLine(int16_t x, int16_t y, int8_t w, uint8_t color) {
    u8g2.setDrawColor(color);
    u8g2.drawHLine(x, y, w);
  }

  void fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color) {
    u8g2.setDrawColor(color);
    u8g2.drawBox(x, y, w, h);
  }

  void drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color) {
    u8g2.setDrawColor(color);
    u8g2.drawFrame(x, y, w, h);
  }

  void drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color) {
    u8g2.setDrawColor(color);
    u8g2.drawCircle(x0, y0, r);
  }

  void fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color) {
      u8g2.setDrawColor(color);
      u8g2.drawDisc(x0, y0, r);
  }

  void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color) {
    u8g2.setDrawColor(color);
    u8g2.drawBitmap(x, y, w/8, h, bitmap);
  }

  void drawBitmapResize(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t size, uint8_t color) {
    int16_t byteWidth = (w + 7) / 8;
    for (int16_t j = 0; j < h; j++) {
      for (int16_t i = 0; i < w; i++) {
        if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
          fillRect(x + (i * size), y + (j * size), size, size, color);
        }
      }
    }
  }

  // Text handling
  void setCursor(int16_t x, int16_t y) { u8g2.setCursor(x, y); }
  void setTextSize(uint8_t s) {
    if (s == 6) u8g2.setFont(u8g2_font_fub30_tr);
    else if (s == 4) u8g2.setFont(u8g2_font_7x14_tf);
    else u8g2.setFont(u8g2_font_5x7_tf);
  }
  void setTextColor(uint8_t color) { u8g2.setDrawColor(color); }
  void setTextBackground(uint8_t bg) { /* Not supported by u8g2 */ }
  virtual size_t write(uint8_t c) override { return u8g2.write(c); }

  // Screen control
  void flipScreen(bool flipped) { u8g2.setDisplayRotation(flipped ? U8G2_R2 : U8G2_R0); }
  void sendLCDCommand(uint8_t command) { u8g2.getU8x8()->user_cb(u8g2.getU8x8(), U8X8_MSG_CAD_SEND_CMD, 1, &command); }
  void setFrameRate(uint8_t rate) { /* Not implemented, u8g2 is immediate */ }
  bool nextFrame() { return true; /* Not needed for u8g2 */ }
  void invert(bool inverse) { u8g2.setInverseFont(inverse); }

};

// Global instance to match Arduboy2 convention
Arduboy2 arduboy;

#endif // ARDUBOY2_SH1106_H
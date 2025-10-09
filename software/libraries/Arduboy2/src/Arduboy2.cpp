#include "Arduboy2.h"
#include <SPI.h>
#include <avr/pgmspace.h>

// Screen hardware definitions
#define CS   10
#define DC   4
#define RST  5

// OLED controller commands
#define OLED_COMMAND_MODE 0x00
#define OLED_DATA_MODE 0x40

#define OLED_SET_COLUMN_ADDR 0x21
#define OLED_SET_PAGE_ADDR   0x22
#define OLED_SET_SEGMENT_REMAP_NORMAL 0xA1
#define OLED_SET_SEGMENT_REMAP_REVERSE 0xA0
#define OLED_SET_COM_SCAN_DIR_NORMAL 0xC8
#define OLED_SET_COM_SCAN_DIR_REVERSE 0xC0


static const uint8_t PROGMEM glcdfont[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x72, 0x49,
  0x49, 0x49, 0x46, 0x00, 0x22, 0x41, 0x49, 0x49, 0x36, 0x00, 0x00, 0x08, 0x14, 0x22, 0x41, 0x00,
  0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x06, 0x09, 0x7F,
  0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2F, 0x4A, 0x4A, 0x4A, 0x31, 0x00,
  0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t PROGMEM oled_init_data[] = {
  0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0x8D, 0x14, 0x20, 0x00,
  OLED_SET_SEGMENT_REMAP_NORMAL, OLED_SET_COM_SCAN_DIR_NORMAL,
  0xDA, 0x12, 0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6, 0xAF
};

Arduboy2::Arduboy2() {
  sBuffer = (uint8_t *)malloc(WIDTH * HEIGHT / 8);
}

void Arduboy2::begin() {
  pinMode(RST, OUTPUT);
  digitalWrite(RST, HIGH);
  delay(1);
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);

  pinMode(DC, OUTPUT);
  pinMode(CS, OUTPUT);

  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  for (uint8_t i = 0; i < sizeof(oled_init_data); i++) {
    sendLCDCommand(pgm_read_byte(&oled_init_data[i]));
  }

  clear();
  display();
}

void Arduboy2::sendLCDCommand(uint8_t command) {
  digitalWrite(DC, LOW);
  digitalWrite(CS, LOW);
  SPI.transfer(command);
  digitalWrite(CS, HIGH);
}

void Arduboy2::clear() {
  memset(sBuffer, 0, WIDTH * HEIGHT / 8);
}

void Arduboy2::display() {
  sendLCDCommand(OLED_SET_COLUMN_ADDR);
  sendLCDCommand(0);
  sendLCDCommand(WIDTH - 1);
  sendLCDCommand(OLED_SET_PAGE_ADDR);
  sendLCDCommand(0);
  sendLCDCommand(HEIGHT / 8 - 1);

  digitalWrite(DC, HIGH);
  digitalWrite(CS, LOW);
  for (int i = 0; i < (WIDTH * HEIGHT / 8); i++) {
    SPI.transfer(sBuffer[i]);
  }
  digitalWrite(CS, HIGH);
}

void Arduboy2::drawPixel(int16_t x, int16_t y, uint8_t color) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
  uint16_t index = (y / 8) * WIDTH + x;
  uint8_t bit = y % 8;
  if (color == WHITE) sBuffer[index] |= (1 << bit);
  else if (color == BLACK) sBuffer[index] &= ~(1 << bit);
  else sBuffer[index] ^= (1 << bit);
}

void Arduboy2::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  drawPixel(x0, y0 + r, color);
  drawPixel(x0, y0 - r, color);
  drawPixel(x0 + r, y0, color);
  drawPixel(x0 - r, y0, color);
  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

void Arduboy2::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) { swap(x0, y0); swap(x1, y1); }
  if (x0 > x1) { swap(x0, x1); swap(y0, y1); }
  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);
  int16_t err = dx / 2;
  int16_t ystep = (y0 < y1) ? 1 : -1;
  for (; x0 <= x1; x0++) {
    if (steep) drawPixel(y0, x0, color);
    else drawPixel(x0, y0, color);
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void Arduboy2::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color) {
  drawLine(x, y, x + w - 1, y, color);
  drawLine(x, y, x, y + h - 1, color);
  drawLine(x + w - 1, y, x + w - 1, y + h - 1, color);
  drawLine(x, y + h - 1, x + w - 1, y + h - 1, color);
}

void Arduboy2::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color) {
  for (int16_t i = x; i < x + w; i++) {
    for (int16_t j = y; j < y + h; j++) {
      drawPixel(i, j, color);
    }
  }
}

void Arduboy2::fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color) {
  for (int16_t y = -r; y <= r; y++) {
    for (int16_t x = -r; x <= r; x++) {
      if (x * x + y * y <= r * r) {
        drawPixel(x0 + x, y0 + y, color);
      }
    }
  }
}

void Arduboy2::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color) {
  int16_t byteWidth = (w + 7) / 8;
  for (int16_t j = 0; j < h; j++) {
    for (int16_t i = 0; i < w; i++) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i % 8))) {
        drawPixel(x + i, y + j, color);
      }
    }
  }
}

void Arduboy2::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void Arduboy2::setTextColor(uint8_t color) {
  text_color = color;
}

void Arduboy2::setTextBackground(uint8_t bg) {
  text_bg_color = bg;
}

void Arduboy2::setTextSize(uint8_t s) {
  text_size = s;
}

size_t Arduboy2::write(uint8_t c) {
  if (c == '\n') {
    cursor_y += text_size * 8;
    cursor_x = 0;
  } else if (c >= 32 && c <= 127) {
    drawChar(cursor_x, cursor_y, c, text_color, text_bg_color, text_size);
    cursor_x += text_size * 6;
  }
  return 1;
}

void Arduboy2::drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size) {
  if (x >= WIDTH || y >= HEIGHT || (x + 6 * size - 1) < 0 || (y + 8 * size - 1) < 0) return;
  for (int8_t i = 0; i < 6; i++) {
    uint8_t line;
    if (i == 5) line = 0x0;
    else line = pgm_read_byte(glcdfont + (c * 5) + i);
    for (int8_t j = 0; j < 8; j++) {
      if (line & 0x1) {
        if (size == 1) drawPixel(x + i, y + j, color);
        else fillRect(x + (i * size), y + (j * size), size, size, color);
      } else if (bg != color) {
        if (size == 1) drawPixel(x + i, y + j, bg);
        else fillRect(x + i * size, y + j * size, size, size, bg);
      }
      line >>= 1;
    }
  }
}


void Arduboy2::setFrameRate(uint8_t rate) {
  frame_duration_micros = 1000000 / rate;
}

bool Arduboy2::nextFrame() {
  uint32_t now_micros = micros();
  uint32_t last_micros = frame_last_millis * 1000;
  if (now_micros < last_micros) { // Handle micros() overflow
      last_micros = now_micros;
  }
  uint32_t frame_micros = now_micros - last_micros;
  if (frame_micros < frame_duration_micros) {
    delayMicroseconds(frame_duration_micros - frame_micros);
  }
  frame_last_millis = millis();
  return true;
}

void Arduboy2::invert(bool inverse) {
  sendLCDCommand(inverse ? 0xA7 : 0xA6);
}

void Arduboy2::drawFastVLine(int16_t x, int16_t y, int8_t h, uint8_t color) {
  drawLine(x, y, x, y + h - 1, color);
}

void Arduboy2::drawFastHLine(int16_t x, int16_t y, int8_t w, uint8_t color) {
  drawLine(x, y, x + w - 1, y, color);
}

void Arduboy2::drawBitmapResize(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t size, uint8_t color) {
  int16_t xi, yi, byteWidth = (w + 7) / 8;
  for (yi = 0; yi < h; yi++) {
    for (xi = 0; xi < w; xi++) {
      if (pgm_read_byte(bitmap + yi * byteWidth + xi / 8) & (128 >> (xi & 7))) {
        fillRect(x + xi * size, y + yi * size, size, size, color);
      }
    }
  }
}
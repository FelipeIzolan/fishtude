#ifndef __TDB_HEADER__
#define __TDB_HEADER__

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include <sys/ioctl.h>
#include <sys/types.h>

#define TDB_PIXEL "█"
#define TDB_PIXEL_QUOTE "#"
#define TDB_PIXEL_CIRCLE "●"
#define TDB_PIXEL_SQUARE "◼"

enum TDB_Keys {
  TDB_SPACE = 32,
  TDB_BACKSPACE = 127,
  TDB_UP = 1000,
  TDB_DOWN,
  TDB_LEFT,
  TDB_RIGHT,
  TDB_HOME,
  TDB_END
};

typedef struct TDB_Size { 
  int col;
  int row;
  int x;
  int y;
} TDB_Size;

typedef struct TDB_Cursor {
  int col;
  int row;
} TDB_Cursor;

typedef struct TDB_BitMap {
  ushort width;
  ushort height;
  ushort row_size;
  ushort bytes_per_pixel;
  uint8_t * data;
} TDB_BitMap;

static struct termios _default;
static void beforeExit();
void TDB_Setup(int blocking);

void TDB_Write(const char * t, int x, int y);
void TDB_WriteF(int x, int y, const char * t, ...);
void TDB_Clear();

void TDB_SetCursor(int col, int row);

TDB_Cursor TDB_GetCursor();
TDB_Size TDB_GetSize();
int TDB_GetKey();

void TDB_DrawLine(const char * c, int x1, int y1, int x2, int y2);
void TDB_DrawRect(const char * c, int x, int y, int w, int h);
void TDB_DrawTria(const char * c, int x, int y, int x1, int y1, int x2, int y2);
void TDB_DrawCirc(const char * c, int x, int y, int r);

TDB_BitMap TDB_LoadBitMap(const char * file);
void TDB_DrawBitMap(TDB_BitMap * bitmap, const char * c, int x, int y, int a);
void TDB_DestroyBitMap(TDB_BitMap * bitmap);

// --------------------------------------------------------------------------------------------------

void TDB_SetCursor(int col, int row) {
  char buffer[256];

  sprintf(buffer, "\x1b[%i;%iH", row, col);
  write(STDOUT_FILENO, buffer, strlen(buffer));
}

void TDB_Write(const char * t, int x, int y) {
  TDB_SetCursor(x, y);
  write(STDOUT_FILENO, t, strlen(t));
}

void TDB_WriteF(int x, int y, const char * t, ...) {
  va_list args;
  char buffer[256];

  va_start(args, t);
  vsprintf(buffer, t, args);
  TDB_Write(buffer, x, y);
  va_end(args);
}

void TDB_Clear() {
  TDB_Write("\x1b[2J\x1b[H", 0, 0);
}

TDB_Cursor TDB_GetCursor() {
  char col[5];
  char row[5];
  char c;

  write(STDOUT_FILENO, "\x1b[6n", 4);   
  read(STDIN_FILENO, col, 2);

  for (int i = 0; i < 5; i++) {
    if (read(STDIN_FILENO, &c, 1) != 1 || c == ';') { 
      row[i] = '\0';
      break;
    }

    row[i] = c;
  }

  for (int i = 0; i < 5; i++) {
    if (read(STDIN_FILENO, &c, 1) != 1 || c == 'R') { 
      col[i] = '\0';
      break;
    }

    col[i] = c;
  }

  return (TDB_Cursor) { atoi(col), atoi(row) }; 
}

static void beforeExit() {
  tcsetattr(STDIN_FILENO, TCSANOW, &_default);
  TDB_Write("\x1b[?25h", 0, 0); // show-cursor
  TDB_Clear();
}

void TDB_Setup(int blocking) {
  tcgetattr(STDIN_FILENO, &_default);
  struct termios raw = _default;
  atexit(beforeExit);

  raw.c_iflag &= ~(IXON);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = blocking;
  
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  TDB_Write("\x1b[?25l", 0, 0); // hide-cursor
}

int TDB_GetKey() {
  char c;
  
  if (read(STDIN_FILENO, &c, 1) != 1) return 0;

  if (c == '\x1b') {
    if (read(STDIN_FILENO, &c, 1) != 1) return 0;
    if (read(STDIN_FILENO, &c, 1) != 1) return 0;
    
    if (c == 'A') return TDB_UP;
    if (c == 'B') return TDB_DOWN;
    if (c == 'C') return TDB_RIGHT;
    if (c == 'D') return TDB_LEFT;
    if (c == 'H') return TDB_HOME;
    if (c == 'F') return TDB_END;
  }

  return c;
}

TDB_Size TDB_GetSize() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return (TDB_Size) { w.ws_col, w.ws_row, w.ws_xpixel, w.ws_ypixel };
}

void TDB_DrawLine(const char * c, int x1, int y1, int x2, int y2) {
  for (float i = 0; i < 1; i += 0.01) {
    int px = ceil(x1 + (x2 - x1) * i);
    int py = ceil(y1 + (y2 - y1) * i);
    
    TDB_Write(c, px, py);
  }
}

void TDB_DrawRect(const char * c, int x, int y, int w, int h) {
  TDB_DrawLine(c, x, y, x+w, y);
  TDB_DrawLine(c, x, y, x, y+h);
  TDB_DrawLine(c, x+w, y, x+w, y+h);
  TDB_DrawLine(c, x, y+h, x+w, y+h);
}

void TDB_DrawTria(const char * c, int x, int y, int x1, int y1, int x2, int y2) {
  int cx = x + x1;
  int cy = y + y1;
  int ex = cx + x2;
  int ey = cy + y2;

  TDB_DrawLine(c, x, y,  cx, cy);
  TDB_DrawLine(c, cx, cy, ex, ey);
  TDB_DrawLine(c, ex, ey, x, y);
}

void TDB_DrawCirc(const char * c, int x, int y, int r) {
  for (double a = 0; a < 2 * M_PI; a += 2 * M_PI / 180) {
    int px = round(2 * r * sin(a) + x);
    int py = round(y + r * cos(a));

    TDB_Write(c, px, py);
  }
}

void TDB_DrawASCII(char ** l, int r, int x, int y) {
  for (int py = 0; py < r; py++) {
    TDB_Write(l[py], x, y + py); 
  }
}

TDB_BitMap TDB_LoadBitMap(const char * file) {
  TDB_BitMap bitmap;
  FILE * stream = fopen(file, "r");
  
  uint bit_per_pixel;
  uint offset;

  if (stream == NULL) { fprintf(stderr, "TDB_LoadBitMap | %s | File stream not opened.\n", file); exit(EXIT_FAILURE); }
  if (fgetc(stream) != 'B' || fgetc(stream) != 'M') { fprintf(stderr, "TDB_LoadBitMap | %s | File not is a bitmap.\n", file); exit(EXIT_FAILURE); }

  fseek(stream, 4, SEEK_CUR);
  fseek(stream, 4, SEEK_CUR);
  fread(&offset, 4, 1, stream);

  fseek(stream, 4, SEEK_CUR);
  fread(&bitmap.width, 4, 1, stream);
  fread(&bitmap.height, 4, 1, stream);

  fseek(stream, 2, SEEK_CUR);
  fread(&bit_per_pixel, 2, 1, stream);

  bitmap.bytes_per_pixel = bit_per_pixel / 8;
  bitmap.row_size = (bit_per_pixel * bitmap.width + 31) / 32 * 4;
  bitmap.data = (uint8_t *) malloc((bitmap.width * bitmap.height) * bitmap.bytes_per_pixel);

  fseek(stream, offset, SEEK_SET);
  fread(bitmap.data, bitmap.bytes_per_pixel, bitmap.width * bitmap.height, stream);

  return bitmap;
}

void TDB_DrawBitMap(TDB_BitMap * bitmap, const char * c, int x, int y, int a) {
  for (int px = 0; px < bitmap->width; px++) {
    for (int py = 0; py < bitmap->height; py++) {
      uint of = bitmap->row_size * py + bitmap->bytes_per_pixel * px;
      if (bitmap->data[of+3] == 0) continue;

      for (int p = 0; p < a; p++) {
        TDB_WriteF(x + (px * a) - p, y + bitmap->height - py, "\x1b[38;2;%u;%u;%um%s\x1b[0m\n", bitmap->data[of+2], bitmap->data[of+1], bitmap->data[of], c);
      }
    }
  }
}

void TDB_DestroyBitMap(TDB_BitMap * bitmap) {
  free(bitmap->data);
  bitmap->data = NULL;
}
#endif

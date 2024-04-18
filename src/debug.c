#ifdef DEBUG

#include "../lib/tdb.h"
#include "./player.c"

static TDB_BitMap p1;
static TDB_BitMap p2;
static TDB_BitMap p3;
static TDB_BitMap p4;
static TDB_BitMap p5;
static TDB_BitMap co;

void debug_Setup() {
  TDB_Setup(0);

  p1 = TDB_LoadBitMap("./assets/passives/1.bmp");
  p2 = TDB_LoadBitMap("./assets/passives/2.bmp");
  p3 = TDB_LoadBitMap("./assets/passives/3.bmp");
  p4 = TDB_LoadBitMap("./assets/passives/4.bmp");
  p5 = TDB_LoadBitMap("./assets/passives/5.bmp");
  co = TDB_LoadBitMap("./assets/coin.bmp");
}

void debug_Draw(PlayerStatus * status, TDB_BitMap * p1, TDB_BitMap * p2, TDB_BitMap * p3, TDB_BitMap * p4, TDB_BitMap * p5, TDB_BitMap * coin) {
  TDB_Clear();
  TDB_Size s = TDB_GetSize();

  TDB_DrawLine("|", s.col / 2, 0, s.col / 2, s.row);
    
  TDB_DrawBitMap(p1, TDB_PIXEL, 4, 2, 2);
  TDB_Write("fish_spawn_quantity", 30, 8);
  TDB_WriteF(30, 9, "%hi", status->fish_spawn_quantity);

  TDB_DrawBitMap(p2, TDB_PIXEL, 4, 14, 2);
  TDB_Write("fish_spawn_time_decrease", 30, 20);
  TDB_WriteF(30, 21, "%f", status->fish_spawn_time_decrease);

  TDB_DrawBitMap(p3, TDB_PIXEL, 4, 26, 2);
  TDB_Write("fish_gold_base", 30, 32);
  TDB_WriteF(30, 33, "%hi", status->fish_gold_base);

  TDB_DrawBitMap(p4, TDB_PIXEL, 4, 38, 2);
  TDB_Write("fish_gold_multiplier", 30, 44);
  TDB_WriteF(30, 45, "%f", status->fish_gold_multiplier);

  TDB_DrawBitMap(p5, TDB_PIXEL, 4, 50, 2);
  TDB_SetCursor(30, 56);
  TDB_Write("gold_passive_income", 30, 56);
  TDB_WriteF(30, 57, "%hi", status->gold_passive_income);

  TDB_DrawBitMap(coin, TDB_PIXEL, (s.col / 2) + 4, 4, 2);
  TDB_WriteF((s.col / 2) + 12, 7, "price(100): %i", (int)(100 * status->passive_price_multiplier));
  TDB_WriteF((s.col / 2) + 12, 8, "price(200): %i", (int)(200 * status->passive_price_multiplier));
}

void debug_Destroy() {
  TDB_DestroyBitMap(&p1);
  TDB_DestroyBitMap(&p2);
  TDB_DestroyBitMap(&p3);
  TDB_DestroyBitMap(&p4);
  TDB_DestroyBitMap(&p5);
  TDB_DestroyBitMap(&co);
}
#endif

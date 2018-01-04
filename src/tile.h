#ifndef TILE_H
#define TILE_H

#define MAPTILE_VOID 0
#define MAPTILE_BLOCK 1
#define MAPTILE_ITEM 2
#define MAPTILE_ENTITY 3

struct tile
{
  void *tile;
  unsigned int type;
};


#endif

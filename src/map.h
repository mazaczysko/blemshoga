#ifndef MAP_H
#define MAP_H

struct map
{
  uint16_t width, height, depth;
  struct tile *map;
};

extern void map_init( struct map *map, uint16_t width, uint16_t height, uint16_t depth );

#endif

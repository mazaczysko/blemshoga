#ifndef ITEM_H
#define ITEM_H

struct item
{
  uint16_t id;
  const char *name;

  const char *spritename;
  ALLEGRO_BITMAP *sprite;

  unsigned int ground : 1; //Is this tile a ground tile?
  unsigned int solid : 1; //Can entities walk through this tile?
  unsigned int flammable : 1; //Can this tile be set on fire?
  unsigned int : 0;

};

#endif

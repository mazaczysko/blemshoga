#ifndef ENTITY_H
#define ENTITY_H

struct entity
{
  uint16_t id;
  const char *name;

  const char *spritename;
  ALLEGRO_BITMAP *sprite;

  //Interaction callbacks
  void ( *entityinteraction )( struct tile **entity, struct tile **tile );
};  

#endif

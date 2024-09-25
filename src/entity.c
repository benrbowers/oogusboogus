typedef enum EntityType {
	ENT_nil,
	ENT_player,
	ENT_crystal,
	ENT_crystal_drop,
	ENT_berry_stalk,
	ENT_berry_drop,
	ENT_TYPE_MAX
} EntityType;
Gfx_Image* ent_sprites[ENT_TYPE_MAX];

typedef struct Entity {
	bool is_valid;
	bool is_pickup;
	bool is_destroyable;
	EntityType type;
	Vector2i tile_pos;
	Vector2i tile_size;
	Range2f tile_rect;
	Vector2 pos;
	Vector2 size;
	Range2f rect;
	Vector2 offset;
	Gfx_Image* sprite;
	Vector4 color;
	int health;
	EntityType drop_type;
	int drop_count;
	float64 spawned_at;
	float64 died_at;
} Entity;

#define MAX_ENTITIES 1024

typedef struct World {
	Entity entities[MAX_ENTITIES];
} World;

World* world = NULL;

Entity* entity_allocate() {
	Entity* entity_found = NULL;

	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* existing_entity = &world->entities[i];
		if (!existing_entity->is_valid) {
			entity_found = existing_entity;
			break;
		}
	}

	assert(entity_found, "No more free entities.");

	return entity_found;
}

void entity_destroy(Entity* ent) {
	memset(ent, 0, sizeof(Entity));
}

Range2f entity_get_rect(Entity* ent) {
	return range2f_bottom_center(
		ent->pos, v2_add(ent->pos, ent->size)
	);
}

Range2f entity_get_tile_rect(Entity* ent) {
	Vector2 min = tile_to_world_pos(ent->tile_pos);
	Vector2 max = v2_add(
		min, v2i_to_v2(v2i_mul(ent->tile_size, tile_size))
	);

	return range2f(min, max);
}

void entity_move_tile(Entity* ent, Vector2i tile_pos) {
	ent->tile_pos = tile_pos;
	ent->pos = v2_add(
		tile_to_world_pos(ent->tile_pos),
		v2_mul(v2(ent->tile_size.x / 2.0f, 0.33f), tile_sizef)
	);
	ent->rect = entity_get_rect(ent);
	ent->tile_rect = entity_get_tile_rect(ent);
}

void entity_move_pos(Entity* ent, Vector2 pos) {
	ent->pos = pos;
	ent->tile_pos = world_to_tile_pos(ent->pos);
	ent->rect = entity_get_rect(ent);
	ent->tile_rect = entity_get_tile_rect(ent);
}

void setup_player(Entity* player) {
	player->is_valid = true;
	player->type = ENT_player;
	player->tile_size = v2i(1, 1);
	player->sprite = ent_sprites[ENT_player];
	player->color = COLOR_WHITE;
	player->size
		= v2(player->sprite->width, player->sprite->height);
	player->pos = v2(0, 0);
	player->rect = entity_get_rect(player);
	player->health = 100;
}

void setup_entity(Entity* ent, EntityType type) {
	ent->is_valid = true;
	ent->type = type;
	ent->sprite = ent_sprites[type];
	ent->color = COLOR_WHITE;
	ent->size = v2(ent->sprite->width, ent->sprite->height);
	ent->spawned_at = frame_now;

	switch (type) {
		case ENT_crystal:
			ent->tile_size = v2i(2, 2);
			ent->drop_type = ENT_crystal_drop;
			ent->drop_count = 3;
			ent->health = 3;
			break;
		case ENT_berry_stalk:
			ent->tile_size = v2i(2, 2);
			ent->drop_type = ENT_berry_drop;
			ent->drop_count = 2;
			ent->health = 10;
			break;
		default:
			ent->tile_size = v2i(1, 1);
			ent->drop_type = ENT_nil;
			ent->drop_count = 0;
			ent->health = 1;
			break;
	}
}

Entity* create_player() {
	Entity* player = entity_allocate();
	setup_player(player);

	return player;
}

Entity* create_entity(EntityType type) {
	Entity* ent = entity_allocate();
	setup_entity(ent, type);

	return ent;
}

Entity*
create_tile_entity(EntityType type, Vector2i tile_pos) {
	Entity* ent = create_entity(type);
	entity_move_tile(ent, tile_pos);

	return ent;
}

Entity* create_free_entity(EntityType type, Vector2 pos) {
	Entity* ent = create_entity(type);
	entity_move_pos(ent, pos);

	return ent;
}

Entity* create_destroyable_entity(EntityType type) {
	Entity* ent = create_entity(type);
	ent->is_destroyable = true;

	return ent;
}

Entity* create_destroyable_tile_entity(
	EntityType type,
	Vector2i tile_pos
) {
	Entity* ent = create_destroyable_entity(type);
	entity_move_tile(ent, tile_pos);

	return ent;
}

Entity* create_destroyable_free_entity(
	EntityType type,
	Vector2 pos
) {
	Entity* ent = create_destroyable_entity(type);
	entity_move_pos(ent, pos);

	return ent;
}

Entity* create_pickup(EntityType type, Vector2 pos) {
	Entity* ent = create_free_entity(type, pos);
	ent->is_pickup = true;

	return ent;
}

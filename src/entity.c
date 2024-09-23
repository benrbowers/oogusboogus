typedef enum EntityType {
	ENT_nil = 0,
	ENT_player = 1,
	ENT_crystal = 2,
	ENT_crystal_drop = 3,
	ENT_berry_stalk = 4,
	ENT_berry_drop = 5,
	ENT_TYPE_MAX = 8
} EntityType;
Gfx_Image* ent_sprites[ENT_TYPE_MAX];

typedef enum ItemType {
	ITEM_nil = 0,
	ITEM_crystal = 1,
	ITEM_berry = 2
} ItemType;

typedef struct Entity {
	bool is_valid;
	EntityType type;
	Vector2i tile_pos;
	Vector2i tile_size;
	Vector2 pos;
	Vector2 size;
	Range2f rect;
	Gfx_Image* sprite;
	Vector4 color;
	int health;
	ItemType item;
} Entity;

#define MAX_ENTITIES 1024

typedef struct World {
	Entity entities[MAX_ENTITIES];
} World;

World* world = NULL;

Entity* entity_create() {
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
	int width = WINDOW_WIDTH * SCALE / TILE_SIZE;
	int height = WINDOW_HEIGHT * SCALE / TILE_SIZE;
	int x = get_random_int_in_range(
		width / -2, (width / 2) - ent->tile_size.x
	);
	int y = get_random_int_in_range(
		height / -2, (height / 2) - ent->tile_size.y
	);

	ent->tile_pos = v2i(x, y);
	ent->tile_size = v2i(2, 2);
	ent->is_valid = true;
	ent->type = type;
	ent->sprite = ent_sprites[type];
	ent->color = COLOR_WHITE;
	ent->size = v2(ent->sprite->width, ent->sprite->height);
	ent->pos = v2_add(
		tile_to_world_pos(ent->tile_pos),
		v2_mul(v2(ent->tile_size.x / 2.0f, 0.33f), tile_sizef)
	);
	ent->rect = entity_get_rect(ent);
	ent->health = 5;
}
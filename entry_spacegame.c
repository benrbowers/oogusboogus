#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SCALE 0.1875 // 240 / 1280

typedef enum EntityType {
	ENT_nil = 0,
	ENT_player = 1,
	ENT_crystal = 2,
	ENT_crystal_drop = 3,
	ENT_berry_stalk = 4,
	ENT_berry_drop = 5
} EntityType;

typedef struct Entity {
	bool is_valid;
	EntityType type;
	Vector2 pos;
	Vector2 size;
	Gfx_Image* sprite;
} Entity;

#define MAX_ENTITIES 1024

typedef struct World {
	Entity entities[MAX_ENTITIES];
} World;
World* world = 0;

Entity* entity_create() {
	Entity* entity_found = 0;

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

void entity_destroy(Entity* entity) {
	memset(entity, 0, sizeof(Entity));
}

void draw(Entity* entity) {
	draw_image(
		entity->sprite,
		entity->pos,
		entity->size,
		COLOR_WHITE
	);
}

void setup_player(Entity* player, Gfx_Image* sprite) {
	player->is_valid = true;
	player->type = ENT_player;
	player->sprite = sprite;
	player->size = v2(sprite->width, sprite->height);
	player->pos = v2(sprite->width * -0.5, 0);
}

void setup_crystal(Entity* crystal, Gfx_Image* sprite) {
	float32 width = WINDOW_WIDTH * SCALE;
	float32 height = WINDOW_HEIGHT * SCALE;
	float32 x = get_random_float32_in_range(width / -2, (width / 2) - sprite->width);
	float32 y = get_random_float32_in_range(height / -2, (height / 2) - sprite->height);

	crystal->is_valid = true;
	crystal->type = ENT_crystal;
	crystal->sprite = sprite;
	crystal->size = v2(sprite->width, sprite->height);
	crystal->pos = v2(x, y);
}

void setup_berry_stalk(Entity* berry_stalk, Gfx_Image* sprite) {
	float32 width = WINDOW_WIDTH * SCALE;
	float32 height = WINDOW_HEIGHT * SCALE;
	float32 x = get_random_float32_in_range(width / -2, (width / 2) - sprite->width);
	float32 y = get_random_float32_in_range(height / -2, (height / 2) - sprite->height);

	berry_stalk->is_valid = true;
	berry_stalk->type = ENT_berry_stalk;
	berry_stalk->sprite = sprite;
	berry_stalk->size = v2(sprite->width, sprite->height);
	berry_stalk->pos = v2(x, y);
}

int entry(int argc, char **argv) {
	// This is how we (optionally) configure the window.
	// To see all the settable window properties, ctrl+f "struct Os_Window" in os_interface.c
	window.title = STR("Space Game Deluxe");
	window.clear_color = hex_to_rgba(0x2a2d3aff);
	window.width = WINDOW_WIDTH;
	window.height = WINDOW_HEIGHT;

	Allocator allocator = get_heap_allocator();
	assert(allocator.proc, "Failed to get heap allocator.");

	world = alloc(allocator, sizeof(World));

	Gfx_Image* player_spr = load_image_from_disk(fixed_string("assets/astronaut.png"), allocator);
	assert(player_spr, "Failed to load player sprite.");
	Gfx_Image* energy_crystal_spr = load_image_from_disk(fixed_string("assets/energy-crystal.png"), allocator);
	assert(energy_crystal_spr, "Failed to load energy_crystal sprite.");
	Gfx_Image* crystal_drop_spr = load_image_from_disk(fixed_string("assets/crystal-drop.png"), allocator);
	assert(crystal_drop_spr, "Failed to load crystal_drop sprite.");
	Gfx_Image* berry_stalk_spr = load_image_from_disk(fixed_string("assets/berry-stalk.png"), allocator);
	assert(berry_stalk_spr, "Failed to load berry_stalk sprite.");
	Gfx_Image* berry_drop_spr = load_image_from_disk(fixed_string("assets/berry-drop.png"), allocator);
	assert(berry_drop_spr, "Failed to load berry_drop sprite.");

	Entity* berry_stalk = entity_create();
	setup_berry_stalk(berry_stalk, berry_stalk_spr);

	for (int i = 0; i < 3; i++) {
		Entity* crystal = entity_create();
		setup_crystal(crystal, energy_crystal_spr);
	}

	Entity* player = entity_create();
	setup_player(player, player_spr);

	s32 frame_count = 0;
	float64 frame_timer = 0.0;

	Vector2 player_pos = v2(player_spr->width * -0.5, 0);
	float64 last_time = 0.0;

	printf("Player sprite size: %d x %d\n", player_spr->width, player_spr->height);
	
	while (!window.should_close) {
		reset_temporary_storage();

		draw_frame.camera_xform = m4_make_scale(v3(SCALE, SCALE, 1.0));

		float64 now = os_get_elapsed_seconds();
		float64 delta_time = now - last_time;
		last_time = now;

		os_update(); 

		if (is_key_just_pressed(KEY_ESCAPE)) {
			window.should_close = true;
		}

		Vector2 input_axis = v2(0, 0);
		if (is_key_down('W')) input_axis.y += 1.0;
		if (is_key_down('S')) input_axis.y -= 1.0;
		if (is_key_down('D')) input_axis.x += 1.0;
		if (is_key_down('A')) input_axis.x -= 1.0;

		input_axis = v2_normalize(input_axis);

		player->pos = v2_add(player->pos, v2_mulf(input_axis, delta_time * 100.0));

		for (int i = 0; i < MAX_ENTITIES; i++) {
			Entity* entity = &world->entities[i];

			if (entity->is_valid) {
				draw(entity);
			}
		}

		frame_count++;
		frame_timer += delta_time;
		if (frame_timer >= 1.0) {
			printf("FPS: %d\n", frame_count);
			frame_timer = 0.0;
			frame_count = 0;
		}
		
		gfx_update();
	}

	return 0;
}
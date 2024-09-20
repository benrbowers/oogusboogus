#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SCALE 0.1875	// 240 / 1280
#define TILE_SIZE 12

bool almost_equals(float a, float b, float epsilon);
bool animate_f32_to_target(
	float* value,
	float target,
	float delta_t,
	float rate
);
void animate_v2_to_target(
	Vector2* value,
	Vector2 target,
	float delta_t,
	float rate
);

Vector2 screen_to_world();
int world_to_tile_unit(float world_unit);
Vector2i world_to_tile_pos(Vector2 world_pos);

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
	Vector2i tile_pos;
	Vector2i tile_size;
	Vector2 pos;
	Vector2 size;
	Range2f rect;
	Gfx_Image* sprite;
	Vector4 color;
} Entity;

#define MAX_ENTITIES 1024

typedef struct World {
	Entity entities[MAX_ENTITIES];
} World;
World* world = NULL;

/* Setup Functions */

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

void draw(Entity* ent) {
	draw_image(
		ent->sprite,
		v2(ent->pos.x - (ent->size.x / 2), ent->pos.y),
		ent->size,
		ent->color
	);
}

void setup_player(Entity* player, Gfx_Image* sprite) {
	player->is_valid = true;
	player->type = ENT_player;
	player->sprite = sprite;
	player->color = COLOR_WHITE;
	player->size = v2(sprite->width, sprite->height);
	player->pos = v2(0, 0);
	player->rect = entity_get_rect(player);
}

void setup_entity(
	Entity* ent,
	EntityType type,
	Gfx_Image* sprite
) {
	float32 width = WINDOW_WIDTH * SCALE;
	float32 height = WINDOW_HEIGHT * SCALE;
	float32 x = get_random_float32_in_range(
		width / -2, (width / 2) - sprite->width
	);
	float32 y = get_random_float32_in_range(
		height / -2, (height / 2) - sprite->height
	);

	ent->is_valid = true;
	ent->type = type;
	ent->sprite = sprite;
	ent->color = COLOR_WHITE;
	ent->size = v2(sprite->width, sprite->height);
	ent->pos = v2(x, y);
	ent->rect = entity_get_rect(ent);
}

int entry(int argc, char** argv) {
	// This is how we (optionally) configure the window.
	// To see all the settable window properties, ctrl+f
	// "struct Os_Window" in os_interface.c
	window.title = STR("Space Game Deluxe");
	window.clear_color = hex_to_rgba(0x2a2d3aff);
	window.width = WINDOW_WIDTH;
	window.height = WINDOW_HEIGHT;

	Allocator allocator = get_heap_allocator();
	assert(allocator.proc, "Failed to get heap allocator.");

	world = alloc(allocator, sizeof(World));

	Gfx_Image* player_spr = load_image_from_disk(
		STR("assets/astronaut.png"), allocator
	);
	assert(player_spr, "Failed to load player sprite.");
	Gfx_Image* energy_crystal_spr = load_image_from_disk(
		STR("assets/energy-crystal.png"), allocator
	);
	assert(
		energy_crystal_spr,
		"Failed to load energy_crystal sprite."
	);
	Gfx_Image* crystal_drop_spr = load_image_from_disk(
		STR("assets/crystal-drop.png"), allocator
	);
	assert(
		crystal_drop_spr, "Failed to load crystal_drop sprite."
	);
	Gfx_Image* berry_stalk_spr = load_image_from_disk(
		STR("assets/berry-stalk.png"), allocator
	);
	assert(
		berry_stalk_spr, "Failed to load berry_stalk sprite."
	);
	Gfx_Image* berry_drop_spr = load_image_from_disk(
		STR("assets/berry-drop.png"), allocator
	);
	assert(
		berry_drop_spr, "Failed to load berry_drop sprite."
	);

	printf(
		"Player sprite size: %d x %d\n",
		player_spr->width,
		player_spr->height
	);

	Entity* berry_stalk = entity_create();
	setup_entity(
		berry_stalk, ENT_berry_stalk, berry_stalk_spr
	);

	for (int i = 0; i < 3; i++) {
		Entity* crystal = entity_create();
		setup_entity(crystal, ENT_crystal, energy_crystal_spr);
	}

	Entity* player = entity_create();
	setup_player(player, player_spr);

	Vector2i tile_size = v2i(TILE_SIZE, TILE_SIZE);
	Vector2 tile_sizef = v2(TILE_SIZE, TILE_SIZE);
	Vector4 tile_color = v4(1, 1, 1, 0.2);

	Vector2 camera_pos = v2(0, 0);

	s32 frame_count = 0;
	float64 frame_timer = 0.0;

	float64 last_time = 0.0;
	float64 hover_start = 0.0;

	int hovered_entity_index = -1;

	// Begin render loop
	while (!window.should_close) {
		reset_temporary_storage();

		float64 now = os_get_elapsed_seconds();
		float64 delta_time = now - last_time;
		last_time = now;

		os_update();

		{
			Vector2 target_pos = player->pos;
			animate_v2_to_target(
				&camera_pos, target_pos, (float)delta_time, 12.0f
			);
			// camera_pos = target_pos;

			draw_frame.camera_xform = m4_identity();
			draw_frame.camera_xform = m4_mul(
				draw_frame.camera_xform,
				m4_make_translation(
					v3(camera_pos.x, camera_pos.y, 1.0)
				)
			);
			draw_frame.camera_xform = m4_mul(
				draw_frame.camera_xform,
				m4_make_scale(v3(SCALE, SCALE, 1.0))
			);
		}

		if (is_key_just_pressed(KEY_ESCAPE)) {
			window.should_close = true;
		}

		Vector2 input_axis = v2(0, 0);
		if (is_key_down('W'))
			input_axis.y += 1.0;
		if (is_key_down('S'))
			input_axis.y -= 1.0;
		if (is_key_down('D'))
			input_axis.x += 1.0;
		if (is_key_down('A'))
			input_axis.x -= 1.0;

		input_axis = v2_normalize(input_axis);

		if (v2_length(input_axis) > 0.0) {
			player->pos = v2_add(
				player->pos, v2_mulf(input_axis, delta_time * 120.0)
			);
			player->rect = entity_get_rect(player);
		}

		Vector2i player_tile_pos
			= world_to_tile_pos(player->pos);
		const int tile_radius_x
			= window.width * SCALE / 2 / TILE_SIZE + 5;
		const int tile_radius_y
			= window.height * SCALE / 2 / TILE_SIZE + 5;

		// Draw grid tiles
		for (int x = player_tile_pos.x - tile_radius_x;
				 x < player_tile_pos.x + tile_radius_x;
				 x++)
		{
			for (int y = player_tile_pos.y - tile_radius_y;
					 y < player_tile_pos.y + tile_radius_y;
					 y++)
			{
				if ((x % 2 == 0 && y % 2 == 0)
						|| (abs(x % 2) == 1 && abs(y % 2) == 1))
				{
					draw_rect(
						v2i_to_v2(v2i_mul(v2i(x, y), tile_size)),
						tile_sizef,
						tile_color
					);
				}
			}
		}

		Vector2 mouse_pos = screen_to_world();

		// Check for hover
		for (int i = MAX_ENTITIES - 1; i >= 0; i--) {
			Entity entity = world->entities[i];

			if (entity.is_valid) {
				if (range2f_contains(entity.rect, mouse_pos)) {
					if (hovered_entity_index != i) {
						hovered_entity_index = i;
						hover_start = now;
					}

					break;
				}
			}

			if (i == 0) {
				if (hovered_entity_index >= 0) {
					hovered_entity_index = -1;
				}
			}
		}

		if (hovered_entity_index >= 0) {
			Entity hovered_entity
				= world->entities[hovered_entity_index];
			draw_rect(
				hovered_entity.rect.min,
				hovered_entity.size,
				v4(
					1, 1, 1, fabs(sin(5 * (now - hover_start))) * 0.2
				)
			);
		}

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
			printf(
				"player: %.3f, %.3f\n", player->pos.x, player->pos.y
			);
			printf(
				"mouse: %.3f, %.3f\n", mouse_pos.x, mouse_pos.y
			);
			print("--------------------------\n");

			frame_timer = 0.0;
			frame_count = 0;
		}

		gfx_update();
	}

	return 0;
}

/* Helper Functions */

bool almost_equals(float a, float b, float epsilon) {
	return fabs(a - b) <= epsilon;
}

bool animate_f32_to_target(
	float* value,
	float target,
	float delta_t,
	float rate
) {
	*value += (target - *value)
		* (1.0 - pow(2.0f, -rate * delta_t));
	if (almost_equals(*value, target, 0.075f)) {
		*value = target;
		return true;	// reached
	}
	return false;
}

void animate_v2_to_target(
	Vector2* value,
	Vector2 target,
	float delta_t,
	float rate
) {
	animate_f32_to_target(
		&(value->x), target.x, delta_t, rate
	);
	animate_f32_to_target(
		&(value->y), target.y, delta_t, rate
	);
}

Vector2 screen_to_world() {
	float mouse_x = input_frame.mouse_x;
	float mouse_y = input_frame.mouse_y;
	Matrix4 proj = draw_frame.projection;
	Matrix4 xform = draw_frame.camera_xform;
	float window_w = window.width;
	float window_h = window.height;

	// Normalize the mouse coordinates
	float unit_x = (mouse_x / (window_w * 0.5f)) - 1.0f;
	float unit_y = (mouse_y / (window_h * 0.5f)) - 1.0f;

	// Transform to world coordinates
	Vector4 world_pos = v4(unit_x, unit_y, 0, 1);
	world_pos = m4_transform(m4_inverse(proj), world_pos);
	world_pos = m4_transform(xform, world_pos);
	// log("%f, %f", world_pos.x, world_pos.y);

	// Return as 2D vector
	return (Vector2){world_pos.x, world_pos.y};
}

int world_to_tile_unit(float world_unit) {
	return world_unit / (float)TILE_SIZE;
}
float tile_to_world_unit(int tile_unit) {
	return tile_unit * TILE_SIZE;
}

Vector2i world_to_tile_pos(Vector2 world_pos) {
	return v2i(
		world_to_tile_unit(world_pos.x),
		world_to_tile_unit(world_pos.y)
	);
}
Vector2 tile_pos_to_world(Vector2i tile_pos) {
	return v2(
		tile_to_world_unit(tile_pos.x),
		tile_to_world_unit(tile_pos.y)
	);
}
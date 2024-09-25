// Global constants
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SCALE 0.1875	// 240 / 1280
#define TILE_SIZE 12
#define COLOR_MOSTLY_RED ((Vector4){1.0, 0.1, 0.1, 1.0})

const Vector2i tile_size = (Vector2i){TILE_SIZE, TILE_SIZE};
const Vector2 tile_sizef = (Vector2){TILE_SIZE, TILE_SIZE};
float64 frame_now = 0.0;

// Includes
#include "units.c"
#include "ui.c"
#include "entity.c"
#include "load.c"
#include "draw.c"

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

	load_sprites(allocator);

	printf(
		"Player sprite size: %d x %d\n",
		ent_sprites[ENT_player]->width,
		ent_sprites[ENT_player]->height
	);

	// Init draw_frame so camera_xform can be used for
	// screen_to_world_pos
	gfx_update();
	draw_frame.camera_xform = m4_identity();
	draw_frame.camera_xform = m4_mul(
		draw_frame.camera_xform,
		m4_make_scale(v3(SCALE, SCALE, 1.0))
	);

	// Make sure player is the first entity
	Entity* player = create_player();

	Entity* berry_stalk = create_destroyable_tile_entity(
		ENT_berry_stalk, get_random_tile_in_screen()
	);

	for (int i = 0; i < 3; i++) {
		create_destroyable_tile_entity(
			ENT_crystal, get_random_tile_in_screen()
		);
	}

	Vector4 grid_color = v4(1, 1, 1, 0.1);

	Vector2 camera_pos = v2(0, 0);

	int frame_count = 0;
	float64 frame_timer = 0.0;

	float64 last_time = os_get_elapsed_seconds();

	Vector2 mouse_pos = v2(0, 0);
	bool user_is_hovering_entity = false;
	float64 hover_start = 0.0;

	Entity* hovered_entity = NULL;

	// Begin render loop
	while (!window.should_close) {
		reset_temporary_storage();

		frame_now = os_get_elapsed_seconds();
		float64 delta_time = frame_now - last_time;
		last_time = frame_now;

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

		const Vector2i player_tile_pos
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
						grid_color
					);
				}
			}
		}

		mouse_pos = mouse_world_pos();
		user_is_hovering_entity = false;

		// Check for hover
		for (int i = 0; i < MAX_ENTITIES; i++) {
			Entity* entity = &world->entities[i];

			if (entity->is_valid && entity->is_destroyable) {
				if (entity->is_pickup)
					continue;
				if (entity->type == ENT_player)
					continue;

				if (range2f_contains(entity->rect, mouse_pos)) {
					if (entity != hovered_entity) {
						hovered_entity = entity;
						hover_start = frame_now;
					}

					user_is_hovering_entity = true;
					break;
				}
			}
		}

		if (!user_is_hovering_entity) {
			if (hovered_entity) {
				hovered_entity = NULL;
			}
		}

		if (hovered_entity) {
			draw_tile_selector(
				hovered_entity, frame_now - hover_start
			);
		}

		if (is_key_just_pressed(MOUSE_BUTTON_LEFT)) {
			consume_key_just_pressed(MOUSE_BUTTON_LEFT);

			if (hovered_entity) {
				hovered_entity->health -= 1;
				hovered_entity->color = COLOR_MOSTLY_RED;
			}
		}

		if (is_key_just_released(MOUSE_BUTTON_LEFT)) {
			consume_key_just_released(MOUSE_BUTTON_LEFT);

			if (hovered_entity) {
				hovered_entity->color = COLOR_WHITE;
			}
		}

		// Destroy dead entities
		for (int i = 0; i < MAX_ENTITIES; i++) {
			Entity* entity = &world->entities[i];

			if (entity->is_valid && entity->health < 1) {
				if (entity == hovered_entity) {
					hovered_entity = NULL;
				}

				if (entity->drop_type) {
					for (int j = 0; j < entity->drop_count; j++) {
						create_pickup(
							entity->drop_type,
							get_random_pos_in_range(entity->tile_rect)
						);
					}
				}

				entity_destroy(entity);
			}
		}

		// Animate pickups as "floating"
		for (int i = 0; i < MAX_ENTITIES; i++) {
			Entity* entity = &world->entities[i];

			// Magnitude based on sprite height
			if (entity->is_valid && entity->is_pickup) {
				entity->offset = v2(
					0.0,
					(entity->size.y + 2.0) / 3.0
						* sin(
							5.0
							* (frame_now - entity->spawned_at - (i * 0.2))
						)
				);
			}
		}

		// Draw entities
		for (int i = 1; i < MAX_ENTITIES; i++) {
			Entity* entity = &world->entities[i];

			if (entity->is_valid) {
				draw(entity);
			}
		}

		// Draw player last
		draw(player);

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
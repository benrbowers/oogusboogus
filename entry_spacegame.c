
int entry(int argc, char **argv) {
	// This is how we (optionally) configure the window.
	// To see all the settable window properties, ctrl+f "struct Os_Window" in os_interface.c
	window.title = STR("Space Game Deluxe");
	window.clear_color = hex_to_rgba(0x2a2d3aff);

	float scale = 0.1875; // 240 / 1280

	Gfx_Image* player = load_image_from_disk(fixed_string("assets/astronaut.png"), get_heap_allocator());
	assert(player, "Failed to load player sprite.");

	s32 frame_count = 0;
	float64 frame_timer = 0.0;

	Vector2 player_pos = v2(player->width * -0.5, 0);
	float64 last_time = 0.0;

	printf("Player sprite size: %d x %d\n", player->width, player->height);
	
	while (!window.should_close) {
		reset_temporary_storage();

		draw_frame.camera_xform = m4_make_scale(v3(scale, scale, 1.0));

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

		player_pos = v2_add(player_pos, v2_mulf(input_axis, delta_time * 100.0));
		
		draw_image(
			player,
			// v2(sin(now)*window.width*0.4-60, -60),
			player_pos,
			v2(player->width, player->height),
			COLOR_WHITE
		);

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
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

Vector2 screen_to_world_pos(Vector2 screen_pos) {
	Matrix4 proj = draw_frame.projection;
	Matrix4 xform = draw_frame.camera_xform;

	// Normalize the mouse coordinates
	float unit_x
		= (screen_pos.x / (window.width * 0.5f)) - 1.0f;
	float unit_y
		= (screen_pos.y / (window.height * 0.5f)) - 1.0f;

	// Transform to world coordinates
	Vector4 world_pos = v4(unit_x, unit_y, 0, 1);
	world_pos = m4_transform(m4_inverse(proj), world_pos);
	world_pos = m4_transform(xform, world_pos);
	// log("%f, %f", world_pos.x, world_pos.y);

	// Return as 2D vector
	return (Vector2){world_pos.x, world_pos.y};
}

Vector2 mouse_world_pos() {
	return screen_to_world_pos(
		v2(input_frame.mouse_x, input_frame.mouse_y)
	);
}

Vector2 screen_min_world_pos() {
	return screen_to_world_pos(v2(0, 0));
}

Vector2 screen_max_world_pos() {
	int x = window.width;
	int y = window.height;

	return screen_to_world_pos(v2(x, y));
}

Range2f screen_world_rect() {
	Vector2 min = screen_min_world_pos();
	Vector2 max = screen_max_world_pos();

	return range2f(min, max);
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
Vector2 tile_to_world_pos(Vector2i tile_pos) {
	return v2(
		tile_to_world_unit(tile_pos.x),
		tile_to_world_unit(tile_pos.y)
	);
}

Vector2 get_random_pos_in_range(Range2f range) {
	float x
		= get_random_float32_in_range(range.min.x, range.max.x);
	float y
		= get_random_float32_in_range(range.min.y, range.max.y);

	return v2(x, y);
}

Vector2i get_random_tile_in_range(Range2i range) {
	int x = get_random_int_in_range(range.min.x, range.max.x);
	int y = get_random_int_in_range(range.min.y, range.max.y);

	return v2i(x, y);
}

Vector2 get_random_pos_in_screen() {
	return get_random_pos_in_range(screen_world_rect());
}

Vector2i get_random_tile_in_screen() {
	Range2i range = range2i(
		world_to_tile_pos(screen_min_world_pos()),
		world_to_tile_pos(screen_max_world_pos())
	);

	return get_random_tile_in_range(range);
}
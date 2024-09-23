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
Vector2 tile_to_world_pos(Vector2i tile_pos) {
	return v2(
		tile_to_world_unit(tile_pos.x),
		tile_to_world_unit(tile_pos.y)
	);
}
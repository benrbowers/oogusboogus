void draw(Entity* ent) {
	if (!ent->is_valid)
		return;

	draw_image(
		ent->sprite,
		v2_add(
			v2(ent->pos.x - (ent->size.x / 2), ent->pos.y),
			ent->offset
		),
		ent->size,
		ent->color
	);
}

void draw_tile_selector(Entity* ent, float64 elapsed_time) {
	if (!ent->is_valid)
		return;
	if (elapsed_time < 0.0)
		return;

	float offset = 6 * fabs(sin(6.5 * elapsed_time)) + 2.0;

	Vector2 pos = tile_to_world_pos(ent->tile_pos);
	pos = v2_sub(pos, v2(offset / 2.0f, offset / 2.0f));
	Vector2 size
		= v2_mul(v2i_to_v2(ent->tile_size), tile_sizef);
	size = v2_add(size, v2(offset, offset));

	float spr_width = ui_sprites[UI_tile_selector_tl]->width;
	Vector2 spr_size = v2(spr_width, spr_width);
	size = v2_sub(size, spr_size);

	draw_image(
		ui_sprites[UI_tile_selector_tl],
		v2(pos.x, pos.y + size.y),
		spr_size,
		COLOR_WHITE
	);
	draw_image(
		ui_sprites[UI_tile_selector_tr],
		v2_add(pos, size),
		spr_size,
		COLOR_WHITE
	);
	draw_image(
		ui_sprites[UI_tile_selector_bl],
		pos,
		spr_size,
		COLOR_WHITE
	);
	draw_image(
		ui_sprites[UI_tile_selector_br],
		v2(pos.x + size.x, pos.y),
		spr_size,
		COLOR_WHITE
	);
}
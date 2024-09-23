void load_sprites(Allocator allocator) {
	ent_sprites[ENT_player] = load_image_from_disk(
		STR("assets/astronaut.png"), allocator
	);
	assert(
		ent_sprites[ENT_player], "Failed to load player sprite."
	);
	ent_sprites[ENT_crystal] = load_image_from_disk(
		STR("assets/energy-crystal.png"), allocator
	);
	assert(
		ent_sprites[ENT_crystal],
		"Failed to load energy_crystal sprite."
	);
	ent_sprites[ENT_crystal_drop] = load_image_from_disk(
		STR("assets/crystal-drop.png"), allocator
	);
	assert(
		ent_sprites[ENT_crystal_drop],
		"Failed to load crystal_drop sprite."
	);
	ent_sprites[ENT_berry_stalk] = load_image_from_disk(
		STR("assets/berry-stalk.png"), allocator
	);
	assert(
		ent_sprites[ENT_berry_stalk],
		"Failed to load berry_stalk sprite."
	);
	ent_sprites[ENT_berry_drop] = load_image_from_disk(
		STR("assets/berry-drop.png"), allocator
	);
	assert(
		ent_sprites[ENT_berry_drop],
		"Failed to load berry_drop sprite."
	);
	ui_sprites[UI_tile_selector_tl] = load_image_from_disk(
		STR("assets/tile-selector-tl.png"), allocator
	);
	assert(
		ui_sprites[UI_tile_selector_tl],
		"Failed to load tile_selector_tl sprite."
	);
	ui_sprites[UI_tile_selector_bl] = load_image_from_disk(
		STR("assets/tile-selector-bl.png"), allocator
	);
	assert(
		ui_sprites[UI_tile_selector_bl],
		"Failed to load tile_selector_bl sprite."
	);
	ui_sprites[UI_tile_selector_tr] = load_image_from_disk(
		STR("assets/tile-selector-tr.png"), allocator
	);
	assert(
		ui_sprites[UI_tile_selector_tr],
		"Failed to load tile_selector_tr sprite."
	);
	ui_sprites[UI_tile_selector_br] = load_image_from_disk(
		STR("assets/tile-selector-br.png"), allocator
	);
	assert(
		ui_sprites[UI_tile_selector_br],
		"Failed to load tile_selector_br sprite."
	);
}
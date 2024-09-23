void load_sprites(Allocator allocator) {
	sprites[SPRITE_player] = load_image_from_disk(
		STR("assets/astronaut.png"), allocator
	);
	assert(
		sprites[SPRITE_player], "Failed to load player sprite."
	);
	sprites[SPRITE_energy_crystal] = load_image_from_disk(
		STR("assets/energy-crystal.png"), allocator
	);
	assert(
		sprites[SPRITE_energy_crystal],
		"Failed to load energy_crystal sprite."
	);
	sprites[SPRITE_crystal_drop] = load_image_from_disk(
		STR("assets/crystal-drop.png"), allocator
	);
	assert(
		sprites[SPRITE_crystal_drop],
		"Failed to load crystal_drop sprite."
	);
	sprites[SPRITE_berry_stalk] = load_image_from_disk(
		STR("assets/berry-stalk.png"), allocator
	);
	assert(
		sprites[SPRITE_berry_stalk],
		"Failed to load berry_stalk sprite."
	);
	sprites[SPRITE_berry_drop] = load_image_from_disk(
		STR("assets/berry-drop.png"), allocator
	);
	assert(
		sprites[SPRITE_berry_drop],
		"Failed to load berry_drop sprite."
	);
	sprites[SPRITE_tile_selector_tl] = load_image_from_disk(
		STR("assets/tile-selector-tl.png"), allocator
	);
	assert(
		sprites[SPRITE_tile_selector_tl],
		"Failed to load tile_selector_tl sprite."
	);
	sprites[SPRITE_tile_selector_bl] = load_image_from_disk(
		STR("assets/tile-selector-bl.png"), allocator
	);
	assert(
		sprites[SPRITE_tile_selector_bl],
		"Failed to load tile_selector_bl sprite."
	);
	sprites[SPRITE_tile_selector_tr] = load_image_from_disk(
		STR("assets/tile-selector-tr.png"), allocator
	);
	assert(
		sprites[SPRITE_tile_selector_tr],
		"Failed to load tile_selector_tr sprite."
	);
	sprites[SPRITE_tile_selector_br] = load_image_from_disk(
		STR("assets/tile-selector-br.png"), allocator
	);
	assert(
		sprites[SPRITE_tile_selector_br],
		"Failed to load tile_selector_br sprite."
	);
}
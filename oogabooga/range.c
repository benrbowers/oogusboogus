// randy: we might wanna remove the f by default, similar to
// the Vector2 ? I know that we'll have a Range2i at some
// point, so maybe it's better to be explicit for less
// confusion? I'll leave this decision up to u charlie just
// delete this whenever u see it

typedef struct Range1f {
	float min, max;
} Range1f;
typedef struct Range2f {
	Vector2 min, max;
} Range2f;

typedef struct Range1i {
	int min, max;
} Range1i;
typedef struct Range2i {
	Vector2i min, max;
} Range2i;

inline Range1f range1f(float min, float max) {
	return (Range1f){min, max};
}

inline Range2f range2f(Vector2 min, Vector2 max) {
	return (Range2f){min, max};
}

inline Range1i range1i(int min, int max) {
	return (Range1i){min, max};
}

inline Range2i range2i(Vector2i min, Vector2i max) {
	return (Range2i){min, max};
}

Range2f pos_size_to_range2f(Vector2 pos, Vector2 size) {
	return range2f(pos, v2_add(pos, size));
}
Range2i pos_size_to_range2i(Vector2i pos, Vector2i size) {
	return range2i(pos, v2i_add(pos, size));
}

inline Range1f range1f_shift(Range1f r, float shift) {
	r.min += shift;
	r.max += shift;
	return r;
}
inline float range1f_size(Range1f range) {
	return range.max - range.min;
}
inline bool range1f_contains(Range1f range, float v) {
	return v >= range.min && v <= range.max;
}
inline bool range1f_empty(Range1f range) {
	return range.min == range.max;
}

inline Range2f range2f_shift(Range2f r, Vector2 shift) {
	r.min = v2_add(r.min, shift);
	r.max = v2_add(r.max, shift);
	return r;
}
inline Vector2 range2f_size(Range2f range) {
	return v2_sub(range.max, range.min);
}
Range2f range2f_bottom_center(Vector2 min, Vector2 max) {
	Range2f range = range2f(min, max);
	Vector2 size = range2f_size(range);
	range = range2f_shift(range, v2(size.x * -0.5, 0.0));
	return range;
}
inline bool range2f_contains(Range2f range, Vector2 v) {
	return v.x >= range.min.x && v.x <= range.max.x
		&& v.y >= range.min.y && v.y <= range.max.y;
}
inline bool range2f_empty(Range2f range) {
	return range.min.x == range.max.x
		&& range.min.y == range.max.y;
}

inline Range1i range1i_shift(Range1i r, int shift) {
	r.min += shift;
	r.max += shift;
	return r;
}
inline int range1i_size(Range1i range) {
	return range.max - range.min;
}
inline bool range1i_contains(Range1i range, int v) {
	return v >= range.min && v <= range.max;
}
inline bool range1i_empty(Range1i range) {
	return range.min == range.max;
}

inline Range2i range2i_shift(Range2i r, Vector2i shift) {
	r.min = v2i_add(r.min, shift);
	r.max = v2i_add(r.max, shift);
	return r;
}
inline Vector2i range2i_size(Range2i range) {
	return v2i_sub(range.max, range.min);
}
Range2i range2i_bottom_center(Vector2i min, Vector2i max) {
	Range2i range = range2i(min, max);
	Vector2i size = range2i_size(range);
	range = range2i_shift(range, v2i(size.x * -0.5, 0.0));
	return range;
}
inline bool range2i_contains(Range2i range, Vector2 v) {
	return v.x >= range.min.x && v.x <= range.max.x
		&& v.y >= range.min.y && v.y <= range.max.y;
}
inline bool range2i_empty(Range2i range) {
	return range.min.x == range.max.x
		&& range.min.y == range.max.y;
}
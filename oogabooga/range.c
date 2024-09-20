// randy: we might wanna remove the f by default, similar to
// the Vector2 ? I know that we'll have a Range2i at some
// point, so maybe it's better to be explicit for less
// confusion? I'll leave this decision up to u charlie just
// delete this whenever u see it

typedef struct Range1f {
	float min, max;
} Range1f;
// ...

typedef struct Range2f {
	Vector2 min, max;
} Range2f;

inline Range1f range1f(float min, float max) {
	return (Range1f){min, max};
}

inline Range2f range2f(Vector2 min, Vector2 max) {
	return (Range2f){min, max};
}

Range2f range2f_shift(Range2f r, Vector2 shift) {
	r.min = v2_add(r.min, shift);
	r.max = v2_add(r.max, shift);
	return r;
}

Vector2 range2f_size(Range2f range) {
	return v2_sub(range.max, range.min);
}

Range2f range2f_bottom_center(Vector2 min, Vector2 max) {
	Range2f range = range2f(min, max);
	Vector2 size = range2f_size(range);
	range = range2f_shift(range, v2(size.x * -0.5, 0.0));
	return range;
}

bool range2f_contains(Range2f range, Vector2 v) {
	return v.x >= range.min.x && v.x <= range.max.x
		&& v.y >= range.min.y && v.y <= range.max.y;
}
#include <math.h>
#include <cassert>
#include <stdint.h>

#include <raylib.h>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

int selectedPoint = 0;

#define CURVE_TYPES_LIST(X) \
	X(CURVE_TYPE_LINEAR, "linear") \
	X(CURVE_TYPE_BEZIER, "bezier") \
	X(CURVE_TYPE_CUBIC_BEZIER, "cubic bezier")

#define X(var, name) var,
enum CurveType : uint8_t {
	CURVE_TYPES_LIST(X)
	CURVE_TYPE_COUNT
};
#undef X

#define X(var, name) name,
const char* strCurveTypes[CURVE_TYPE_COUNT] = {
	CURVE_TYPES_LIST(X)
};

struct Curve {
	static constexpr float CIRCLE_RADIUS = 6.0f; ///< Use this variable to render the points as circles.

	rl::Vector2 p0{}, p1{}, p2{}, p3{}, result{};

	float t = 0.0f;
	float timeMultiplier = 1.0f;

	CurveType type = CURVE_TYPE_CUBIC_BEZIER;
};

static inline float lerp(float a, float b, float t) {
	assert(t >= 0 && t <= 1);
	return a + (b - a) * t;
}

static inline rl::Vector2 lerp(rl::Vector2 a, rl::Vector2 b, float t) {
	a.x = lerp(a.x, b.x, t);
	a.y = lerp(a.y, b.y, t);
	return a;
}

static inline void lerp(Curve& curve) {
	curve.result = lerp(curve.p0, curve.p3, curve.t);
}


static inline void bezier(Curve& curve) {
	rl::Vector2 a = lerp(curve.p0, curve.p1, curve.t);
	rl::Vector2 b = lerp(curve.p1, curve.p3, curve.t);
	curve.result = lerp(a, b, curve.t);
}

static inline void cubicBezier(Curve& curve) {
	rl::Vector2 a = lerp(curve.p0, curve.p1, curve.t);
	rl::Vector2 b = lerp(curve.p1, curve.p2, curve.t);
	rl::Vector2 c = lerp(curve.p2, curve.p3, curve.t);
	rl::Vector2 d = lerp(a, b, curve.t);
	rl::Vector2 e = lerp(b, c, curve.t);
	curve.result = lerp(d, e, curve.t);
}

static inline void update(Curve& curve) {
	// Update curve.c position based on curve type.
	switch (curve.type) {
	case CURVE_TYPE_LINEAR:
		lerp(curve);
		break;

	case CURVE_TYPE_BEZIER:
		bezier(curve);	
		break;

	case CURVE_TYPE_CUBIC_BEZIER:
		cubicBezier(curve);
		break;

	default:
		break;
	}

	// Update curve time.
	curve.t += rl::GetFrameTime()*curve.timeMultiplier;
	if (curve.t > 1.0f) {
		curve.t = 1.0f;
		curve.timeMultiplier *= -1.0f;

	} else if (curve.t < 0.0f) {
		curve.t = 0.0f;
		curve.timeMultiplier *= -1.0f;
	}

	// Change selected point.
	if (rl::IsKeyPressed(rl::KEY_ONE)) {
		selectedPoint = 0;
	} else if (rl::IsKeyPressed(rl::KEY_TWO)) {
		selectedPoint = 1;
	} else if (rl::IsKeyPressed(rl::KEY_THREE)) {
		selectedPoint = 2;
	} else if (rl::IsKeyPressed(rl::KEY_FOUR)) {
		selectedPoint = 3;
	}

	// Change curve type.
	if (rl::IsKeyPressed(rl::KEY_B)) {
		curve.type = CURVE_TYPE_BEZIER;
	} else if (rl::IsKeyPressed(rl::KEY_L)) {
		curve.type = CURVE_TYPE_LINEAR;
	} else if (rl::IsKeyPressed(rl::KEY_C)) {
		curve.type = CURVE_TYPE_CUBIC_BEZIER;
	}

	// Move selected point.
	if (rl::IsMouseButtonDown(rl::MOUSE_BUTTON_LEFT)) {
		rl::Vector2 mousePos = rl::GetMousePosition();

		switch (selectedPoint) {
		case 0:
			curve.p0 = mousePos;
			break;

		case 1:
			curve.p1 = mousePos;
			break;

		case 2:
			curve.p2 = mousePos;
			break;

		case 3:
			curve.p3 = mousePos;
			break;

		default:
			break;
		}
	}
}


static inline void drawCurve(const Curve& curve) {
	rl::DrawCircleV(curve.p0, Curve::CIRCLE_RADIUS, rl::RED);

	switch (curve.type) {
	case CURVE_TYPE_LINEAR:
		break;

	case CURVE_TYPE_CUBIC_BEZIER:
		rl::DrawCircleV(curve.p2, Curve::CIRCLE_RADIUS, rl::YELLOW);
	case CURVE_TYPE_BEZIER:
		rl::DrawCircleV(curve.p1, Curve::CIRCLE_RADIUS, rl::GREEN);
		break;

	default:
		break;
	}

	rl::DrawCircleV(curve.p3, Curve::CIRCLE_RADIUS, rl::BLUE);
	rl::DrawCircleV(curve.result, Curve::CIRCLE_RADIUS, rl::PINK);
}

static inline void drawInfo(const Curve& curve) {
	rl::DrawText("Press 1, 2, 3 or 4 to select a point and move it by pressing left mouse button", 0,0, 20, rl::RAYWHITE);
	rl::DrawText(rl::TextFormat("Point: %d", selectedPoint+1), 0,20, 20, rl::RAYWHITE);
	rl::DrawText("Press L for linear", 0,40, 20, rl::RAYWHITE);
	rl::DrawText("Press B for bezier", 0,60, 20, rl::RAYWHITE);
	rl::DrawText("Press C for cubic bezier", 0,80, 20, rl::RAYWHITE);
	rl::DrawText(rl::TextFormat("Curve: %s", strCurveTypes[curve.type]), 0,100, 20, rl::RAYWHITE);
}


int main() {
	rl::SetConfigFlags(rl::FLAG_MSAA_4X_HINT|rl::FLAG_WINDOW_HIGHDPI);
	rl::InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "2D Curve Animations");

	Curve curve{};

	curve.p0 = {
		.x = 8.0f,
		.y = WINDOW_HEIGHT-8.0f,
	};

	curve.p1 = {
		.x = WINDOW_WIDTH/2.0f-3.0f,
		.y = WINDOW_HEIGHT/2.0f-3.0f
	};

	curve.p2 = {
		.x = WINDOW_WIDTH/2.0f-3.0f,
		.y = WINDOW_HEIGHT/2.0f-3.0f
	};

	curve.p3 = {
		.x = WINDOW_WIDTH-8.0f,
		.y = WINDOW_HEIGHT-8.0f
	};

	while (!rl::WindowShouldClose()) {
		update(curve);

		rl::BeginDrawing();
			rl::ClearBackground(rl::BLACK);
			drawCurve(curve);
			drawInfo(curve);
		rl::EndDrawing();
	}
}
#include <math.h>
#include <cassert>
#include <stdint.h>

#include <raylib.h>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

int selectedPoint = 0;

enum CurveType : uint8_t {
	CURVE_TYPE_LINEAR = 0,
	CURVE_TYPE_BEZIER
};

struct Curve {
	static constexpr float CIRCLE_RADIUS = 6.0f; ///< Use this variable to render the points as circles.

	rl::Vector2 a{}, p0{};
	rl::Vector2 b{}, p1{};
	rl::Vector2 c{}, p2{};

	float t = 0.0f;
	float timeMultiplier = 1.0f;

	CurveType type = CURVE_TYPE_BEZIER;
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
	curve.c = lerp(curve.p0, curve.p2, curve.t);
}


static inline void bezier(Curve& curve) {
	curve.a = lerp(curve.p0, curve.p1, curve.t);
	curve.b = lerp(curve.p1, curve.p2, curve.t);
	curve.c = lerp(curve.a, curve.b, curve.t);
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
	}

	// Change curve type.
	if (rl::IsKeyPressed(rl::KEY_B)) {
		curve.type = CURVE_TYPE_BEZIER;
	} else if (rl::IsKeyPressed(rl::KEY_L)) {
		curve.type = CURVE_TYPE_LINEAR;
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

		default:
			break;
		}
	}
}


static inline void drawCurve(const Curve& curve) {
	rl::DrawCircleV(curve.p0, Curve::CIRCLE_RADIUS, rl::RED);
	if (curve.type == CURVE_TYPE_BEZIER) {
		rl::DrawCircleV(curve.p1, Curve::CIRCLE_RADIUS, rl::GREEN);
	}
	rl::DrawCircleV(curve.p2, Curve::CIRCLE_RADIUS, rl::BLUE);
	rl::DrawCircleV(curve.c, Curve::CIRCLE_RADIUS, rl::PINK);
}

static inline void drawInfo(const Curve& curve) {
	rl::DrawText("Press 1, 2, 3 to select a point and move them by pressing left mouse button", 0,0, 20, rl::RAYWHITE);
	rl::DrawText(rl::TextFormat("Point: %d", selectedPoint), 0,20, 20, rl::RAYWHITE);
	rl::DrawText("Press B for bezier", 0,40, 20, rl::RAYWHITE);
	rl::DrawText("Press L for linear", 0,60, 20, rl::RAYWHITE);
	rl::DrawText(rl::TextFormat("Curve: %s", curve.type == CURVE_TYPE_BEZIER? "bezier" : "linear"), 0,80, 20, rl::RAYWHITE);
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


//
// Based on joelgwebber's Box2D benchmarks,
// https://github.com/joelgwebber/bench2d/blob/master/c/Bench2d.cpp
//


// Settings =====================
// Turn this on to include the y-position of the top box in the output.
#define DEBUG 0

int WARMUP;
int FRAMES;

typedef struct {
  float mean;
  float stddev;
} result_t;
// ==============================



#include <cstdio>
#include <time.h>
#include <math.h>

#include "Box2D/Box2D.h"

using namespace std;

const int e_count = 40;

result_t measure(clock_t times[FRAMES]) {
  float values[FRAMES];
  result_t r;

	float total = 0;
	for (int i = 0; i < FRAMES; ++i) {
		values[i] = (float)times[i] / CLOCKS_PER_SEC * 1000;
		total += values[i];
	}
  r.mean = total / FRAMES;

  float variance = 0;
	for (int i = 0; i < FRAMES; ++i) {
		float diff = values[i] - r.mean;
		variance += diff * diff;
	}
  r.stddev = sqrt(variance / FRAMES);

  return r;
}

int main(int argc, char **argv) {
  int arg = argc > 1 ? argv[1][0] - '0' : 1;
  switch(arg) {
    case 0: WARMUP = 32; FRAMES = 161; break;
    case 1: WARMUP = 64; FRAMES = 333; break;
    case 2: WARMUP = 5*64; FRAMES = 7*333; break;
    case 3: WARMUP = 10*64; FRAMES = 17*333; break;
    default: printf("error: %d\\n", arg); return -1;
  }

	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);
  world.SetAllowSleeping(false);

	{
		b2BodyDef bd;
		b2Body* ground = world.CreateBody(&bd);

		b2EdgeShape shape;
		shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		ground->CreateFixture(&shape, 0.0f);
	}

  b2Body* topBody;

	{
		float32 a = 0.5f;
		b2PolygonShape shape;
		shape.SetAsBox(a, a);

		b2Vec2 x(-7.0f, 0.75f);
		b2Vec2 y;
		b2Vec2 deltaX(0.5625f, 1);
		b2Vec2 deltaY(1.125f, 0.0f);

		for (int32 i = 0; i < e_count; ++i) {
			y = x;

			for (int32 j = i; j < e_count; ++j) {
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position = y;
				b2Body* body = world.CreateBody(&bd);
				body->CreateFixture(&shape, 5.0f);

        topBody = body;

				y += deltaY;
			}

			x += deltaX;
		}
	}

	for (int32 i = 0; i < WARMUP; ++i) {
		world.Step(1.0f/60.0f, 3, 3);
  }

	clock_t times[FRAMES]; 
	for (int32 i = 0; i < FRAMES; ++i) {
		clock_t start = clock();
		world.Step(1.0f/60.0f, 3, 3);
		clock_t end = clock();
		times[i] = end - start;
#if DEBUG
    printf("%f :: ", topBody->GetPosition().y);
		printf("%f\n", (float32)(end - start) / CLOCKS_PER_SEC * 1000);
#endif
	}

  result_t result = measure(times);

  printf("frame averages: %.3f +- %.3f\n", result.mean, result.stddev);

  return 0;
}


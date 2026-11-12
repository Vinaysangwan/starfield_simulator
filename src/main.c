#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include <raymath.h>

// #############################################################################
//                           Defines                                            
// #############################################################################
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 720

#define STARS_COUNT 500
#define STAR_RADIUS 1
#define STAR_MAX_RADIUS 4
#define STAR_COLOR WHITE
#define STAR_SPEED 12
#define MAX_DEPTH 1200
#define MIN_DEPTH 1200

// #############################################################################
//                           Structs                                            
// #############################################################################
typedef struct
{
  float x;
  float y;
  float z;

  float pz;
} Star;

// #############################################################################
//                           Statics                                            
// #############################################################################
static Star stars[STARS_COUNT] = {0};
static char draw_circle = 1;

// #############################################################################
//                           Functions                                            
// #############################################################################
float map_range(float val, float input_min, float input_max, float output_min, float output_max)
{
  return output_min + (val - input_min) * (output_max - output_min) / (input_max - input_min);
}

float get_rand_float(float min, float max)
{
  return min + (float)rand() * (max - min) / RAND_MAX;
}

Star star_create()
{
  Star self = {
    .x = get_rand_float(-WINDOW_WIDTH / 2.0f, WINDOW_WIDTH / 2.0f),
    .y = get_rand_float(-WINDOW_HEIGHT / 2.0f, WINDOW_HEIGHT / 2.0f),
    // .x = get_rand_float(-100.0f, 100.0f),
    // .y = get_rand_float(-100.0f, 100.0f),
    .z = get_rand_float(1.0f, MAX_DEPTH),
  };

  self.pz = self.z;

  return self;
}

void star_update(Star* self)
{
  self->z -= STAR_SPEED;
  
  if (self->z <= 0.0f)
  {
    self->x = get_rand_float(-WINDOW_WIDTH / 2.0f, WINDOW_WIDTH / 2.0f);
    self->y = get_rand_float(-WINDOW_HEIGHT / 2.0f, WINDOW_HEIGHT / 2.0f);
    self->z = MAX_DEPTH;

    self->pz = self->z;
  }
}

void star_render(Star* self)
{
  float sx = map_range(self->x / self->z, -1.0f, 1.0f, 0.0f, WINDOW_WIDTH);
  float sy = map_range(self->y / self->z, -1.0f, 1.0f, 0.0f, WINDOW_HEIGHT);

  if (draw_circle)
  {
    DrawCircleV(
      (Vector2){sx, sy},
      // STAR_RADIUS,
      map_range(self->z, 0.0f, MAX_DEPTH, STAR_MAX_RADIUS, STAR_RADIUS),
      STAR_COLOR
    );
  }

  float psx = map_range(self->x / self->pz, -1.0f, 1.0f, 0.0f, WINDOW_WIDTH);
  float psy = map_range(self->y / self->pz, -1.0f, 1.0f, 0.0f, WINDOW_HEIGHT);

  self->pz = self->z;
  
  DrawLine(psx, psy, sx, sy, STAR_COLOR);
}

int main()
{
  srand((unsigned int)time(NULL));
  
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Starfield Simulator");
  SetTargetFPS(60);

  // init stars
  for (int i=0; i<STARS_COUNT; ++i)
  {
    stars[i] = star_create();
  }

  while (!WindowShouldClose())
  {
    if (IsKeyPressed(KEY_C))
    {
      draw_circle = !draw_circle;
    }
    
    // update stars
    for (int i=0; i<STARS_COUNT; ++i)
    {
      star_update(&stars[i]);
    }
    
    BeginDrawing();
    ClearBackground(BLACK);

    // render stars
    for (int i=0; i<STARS_COUNT; ++i)
    {
      star_render(&stars[i]);
    }

    DrawFPS(20, 20);

    EndDrawing();
  }
  
  return 0;
}

#include "../include/object.h"
#include "../include/sphere.h"

#include <graphics.h>

std::vector<Object> objects;
std::vector<Sphere> spheres;


bool within_x(float x, Object& obj)
{
	return (x >= obj.x && x <= obj.x + obj.xl);
}

bool within_y(float y, Object& obj)
{
	return (y >= obj.y && y <= obj.y + obj.yl);
}

bool within_z(float z, Object& obj)
{
	return (z >= obj.z && z <= obj.z + obj.zl);
}

bool inside_sphere(float x, float y, float z, Sphere& s)
{
	float xf = (x - s.x) * (x - s.x);
	float yf = (y - s.y) * (y - s.y);
	float zf = (z - s.z) * (z - s.z);

	return sqrt(xf + yf + zf) <= s.r;
}


int main()
{
	int screen_w = 120;
	int screen_h = 30;

	gfx::init(screen_w, screen_h);

	float px = 0.0f, py = 0.0f, pz = 0.0f;
	float pha = 0.0f;
	float pva = 0.0f;
	float fov = 1.0f;

	float render_dist = 35.0F;

	objects.push_back(Object(1.0f, -3.0f, 0.0f));

	spheres.push_back(Sphere(0.0f, 0.0f, 4.0f, 2));
	spheres.push_back(Sphere(5.0f, 2.0f, 3.0f, 3));
	spheres.push_back(Sphere(0.0f, -1.0f, 0.0f, 1));
	
	int yo = 0;


	while (true)
	{
		yo = !yo;

		gfx::limit_fps(60);

		if (GetAsyncKeyState('W') < 0)
		{
			pz += cosf(pha) * 0.1f;
			px += sinf(pha) * 0.1f;
		}

		if (GetAsyncKeyState('S') < 0)
		{
			pz -= cosf(pha) * 0.1f;
			px -= sinf(pha) * 0.1f;
		}

		if (GetAsyncKeyState('A') < 0)
		{
			px += sinf(pha - (3.1415 / 2)) * 0.1f;
			pz += cosf(pha - (3.1415 / 2)) * 0.1f;
		}

		if (GetAsyncKeyState('D') < 0)
		{
			px += sinf(pha + (3.1415 / 2)) * 0.1f;
			pz += cosf(pha + (3.1415 / 2)) * 0.1f;
		}

		if (GetAsyncKeyState(VK_RIGHT) < 0)
			pha += 0.1f;

		if (GetAsyncKeyState(VK_LEFT) < 0)
			pha -= 0.1f;

		if (GetAsyncKeyState(VK_UP) < 0)
			if (pva - 0.1f > -0.4f)
				pva -= 0.1f;

		if (GetAsyncKeyState(VK_DOWN) < 0)
			if (pva + 0.1f < 0.4f)
				pva += 0.1f;

		if (GetAsyncKeyState(VK_SPACE) < 0)
			py -= 0.1f;

		if (GetAsyncKeyState(VK_SHIFT) < 0)
			py += 0.1f;

		for (int y = 0; y < screen_h; y++)
		{
			if ((y + yo) % 2 == 0)
				continue;

			float dist = 0.0f;

			for (int x = 0; x < screen_w; x++)
			{
				bool check_for_behind = false;

				if (x % 2 == 0)
					check_for_behind = true;

				float rha = (pha - (fov / 2.0f)) + ((float)x / (float)screen_w) * fov;
				float rva = (pva - (fov / 2.0f)) + ((float)y / (float)(screen_h + 20)) * fov;

				bool hit_obj = false;

				float tx, ty, tz;

				while (dist < render_dist && !hit_obj)
				{
					dist += 0.2f;

					tx = (float)(px + dist * sinf(rha));
					ty = (float)(py + dist * sinf(rva));
					tz = (float)(pz + dist * cosf(rha));

					for (Object& obj : objects)
					{
						if (within_x(tx, obj) && within_z(tz, obj) && within_y(ty, obj))
						{
							while (within_x(tx, obj) && within_y(ty, obj) && within_z(tz, obj))
							{
								dist -= 0.2f;

								tx = (float)(px + dist * sinf(rha));
								ty = (float)(py + dist * sinf(rva));
								tz = (float)(pz + dist * cosf(rha));
							}

							hit_obj = true;
							break;
						}
					}

					for (Sphere& s : spheres)
					{
						if (inside_sphere(tx, ty, tz, s))
						{
							while (inside_sphere(tx, ty, tz, s))
							{
								dist -= 0.2f;

								tx = (float)(px + dist * sinf(rha));
								ty = (float)(py + dist * sinf(rva));
								tz = (float)(pz + dist * cosf(rha));
							}

							hit_obj = true;
							break;
						}
					}
				}
				
				if (hit_obj)
				{
					char shade = ' ';
					if (dist < render_dist / 11)     shade = '#';
					else if (dist < render_dist / 8) shade = '=';
					else if (dist < render_dist / 6) shade = ':';
					else if (dist < render_dist / 5) shade = '.';

					gfx::draw_char(x, y, shade);
				}
				else
				{
					gfx::draw_char(x, y, ' ');
				}

				if (check_for_behind)
					dist = 0.0f;
			}
		}

		gfx::draw_text(1, 1, "x: " + std::to_string(px));
		gfx::draw_text(1, 2, "y: " + std::to_string(py));
		gfx::draw_text(1, 3, "z: " + std::to_string(pz));

		gfx::draw_text(1, 5, "horizontal angle: " + std::to_string(pha));
		gfx::draw_text(1, 6, "vertical angle: " + std::to_string(pva));

		gfx::display();
	}


	gfx::cleanup();

	return 0;
}
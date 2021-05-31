#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class application : public olc::PixelGameEngine
{
private:
	std::wstring m_entities;
	int m_width;
	int m_height;
	int high_threshold;
	int low_threshold;
	float interval;
	float elapsed;

public:
	application()
	{
		sAppName = "Game of Life";
		m_width = 64;
		m_height = 64;
		high_threshold = 3;
		low_threshold = 2;
		interval = 1.f;
		elapsed = 0.f;
	}

	bool OnUserCreate() override
	{
		m_entities += L"..#.............................................................";
		m_entities += L"..#.............................................................";
		m_entities += L"..#.............................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";

		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";

		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";

		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";
		m_entities += L"................................................................";

		//seed_grid();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		elapsed += fElapsedTime;

		if (elapsed < interval)
		{
			return true;
		}

		elapsed = 0;

		// Draw entities

		const int fill_width = 16;
		const int fill_height = 16;

		for (auto y = 0; y < m_height; ++y)
		{
			for (auto x = 0; x < m_width; ++x)
			{
				const auto entity = get_entity(x, y);
				switch (entity)
				{
				case L'.': // dead
					FillRect(fill_width * x, fill_height * y, fill_width, fill_height, olc::DARK_GREY);
					break;
				case L'#': // alive
					FillRect(fill_width * x, fill_height * y, fill_width, fill_height, olc::GREY);
					break;
				default:
					FillRect(fill_width * x, fill_height * y, fill_width, fill_height, olc::BLACK);
					break;
				}
			}
		}

		// Update entities

		for (auto y = 0; y < m_height; ++y)
		{
			for (auto x = 0; x < m_width; ++x)
			{
				const auto alive_neighbors = get_alive_neighbors(x, y);
				const auto entity = get_entity(x, y);

				if (entity == L'#' && (alive_neighbors == low_threshold || alive_neighbors == high_threshold)) // Any live cell with two or three live neighbors survives.
				{
					continue;
				}
				
				if (entity == L'.' && alive_neighbors == high_threshold) // Any dead cell with three live neighbors becomes a live cell.
				{
					set_entity(x, y, L'#');
				}
				else // All other live cells die in the next generation.Similarly, all other dead cells stay dead.
				{
					set_entity(x, y, L'.');
				}
			}
		}

		return true;
	}

	bool OnUserDestroy() override
	{
		return true;
	}

	wchar_t get_entity(int x, int y)
	{
		//std::cout << "get_entity x: " << x << " y: " << y << std::endl;
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			return m_entities[y * m_width + x]; // access 1d array like a 2d array
		}
		return L' '; // sudo null
	}

	void set_entity(int x, int y, wchar_t c)
	{
		//std::cout << "set_entity x: " << x << " y: " << y << " char: " << c << std::endl;
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			m_entities[y * m_width + x] = c;
		}
	}

	int get_alive_neighbors(int x, int y)
	{
		//std::cout << "get_alive_neighbors x: " << x << " y: " << y << std::endl;

		const int x_start = x > 0 ? x - 1 : x;
		const int x_end = x < m_width ? x + 1 : x;
		//std::cout << "start x: " << x_start << "end x: " << x_end << std::endl;

		const int y_start = y > 0 ? y - 1 : y;
		const int y_end = y < m_height ? y + 1 : y;
		//std::cout << "start y: " << y_start << "end y: " << y_end << std::endl;

		int alive_count = 0;

		for (auto y_pos = y_start; y_pos <= y_end; ++y_pos)
		{
			for (auto x_pos = x_start; x_pos <= x_end; ++x_pos)
			{
				if (y_pos == y && x_pos == x)
				{
					continue; // ignore self
				}

				const auto entity = get_entity(x_pos, y_pos);

				if (entity == L'#')
				{
					alive_count++;
				}
			}
		}

		return alive_count;
	}

	void seed_grid()
	{
		for (auto y = 0; y < m_height; ++y)
		{
			for (auto x = 0; x < m_width; ++x)
			{
				if (rand() % 100 > 91)
				{
					set_entity(x, y, L'#');
					//std::cout << "Setting #" << std::endl;
				}
				else
				{
					set_entity(x, y, L'.');
					//std::cout << "Setting ." << std::endl;
				}
			}
		}
	}

	void clear_grid()
	{
		for (auto y = 0; y < m_height; ++y)
		{
			for (auto x = 0; x < m_width; ++x)
			{
				set_entity(x, y, L'.');
			}
		}
	}
};

int main()
{
	application app;
	if (app.Construct(1024, 1024, 1, 1))
	{
		app.Start();
	}

	return 0;
}
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
	int random_threshold;
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
		random_threshold = 50;
		interval = 0.2f;
		elapsed = 0.f;		
	}

	bool OnUserCreate() override
	{
		m_entities += L"..#.............................................................";
		m_entities += L"#.#.............................................................";
		m_entities += L".##.............................................................";
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

		seed_grid(random_threshold);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// check for mouse click

		if (GetMouse(0).bReleased)
		{			
			clear_grid();
			random_threshold = std::min(random_threshold + 10, 100);
			seed_grid(random_threshold);
			return true;
		}
		
		if (GetMouse(1).bReleased)
		{
			clear_grid();	
			random_threshold = std::max(random_threshold - 10, 0);			
			seed_grid(random_threshold);
			return true;
		}

		// delay update
		
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

		// Update entity states

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
					set_entity(x, y, L'0'); // to become alive
				}
				else // All other live cells die in the next generation.Similarly, all other dead cells stay dead.
				{
					set_entity(x, y, entity == L'#' ? L'X' : L'.'); // to be killed
				}
			}
		}

		// Finalize entity states

		for (auto y = 0; y < m_height; ++y)
		{
			for (auto x = 0; x < m_width; ++x)
			{
				const auto entity = get_entity(x, y);

				switch (entity)
				{
				case L'X': // to be killed
					set_entity(x, y, L'.');
					break;
				case L'0': // to become alive
					set_entity(x, y, L'#');
					break;
				default:					
					break;
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
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			return m_entities[y * m_width + x]; // access 1d array like a 2d array
		}
		return L' '; // sudo null
	}

	void set_entity(int x, int y, wchar_t c)
	{		
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			m_entities[y * m_width + x] = c;
		}
	}

	int get_alive_neighbors(int x, int y)
	{
		const int x_start = x > 0 ? x - 1 : x;
		const int x_end = x < m_width ? x + 1 : x;		

		const int y_start = y > 0 ? y - 1 : y;
		const int y_end = y < m_height ? y + 1 : y;

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

				if (entity == L'#' || entity == L'X') // if alive or flagged for death
				{
					alive_count++;
				}
			}
		}

		return alive_count;
	}

	void seed_grid(int threshold)
	{
		for (auto y = 0; y < m_height; ++y)
		{
			for (auto x = 0; x < m_width; ++x)
			{
				if (rand() % 100 > threshold)
				{
					set_entity(x, y, L'#');					
				}
				else
				{
					set_entity(x, y, L'.');					
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
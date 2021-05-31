#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


class application : public olc::PixelGameEngine
{
private:
	std::wstring m_cells;
	int m_cell_width;
	int m_cell_height;
	int m_grid_width;
	int m_grid_height;
	int m_become_alive;
	int m_become_dead;
	int m_seed_threshold;
	float m_update_interval;
	float m_time_since_last_update;

public:
	static const int screen_width = 1920; // modify
	static const int screen_height = 1080; // modify
	static const int pixel_width = 1;
	static const int pixel_height = 1;

	application()
		: m_grid_width(0)
		, m_grid_height(0)
		, m_become_alive(3)
		, m_become_dead(2)
		, m_seed_threshold(50)
		, m_update_interval(0.2f)
		, m_time_since_last_update(0.f)
	{
		sAppName = "Game of Life";
		m_cell_width = 4; // modify
		m_cell_height = 4; // modify
	}

	bool OnUserCreate() override
	{
		const auto screen_size = GetWindowSize();

		m_grid_width = screen_size.x / m_cell_width;
		m_grid_height = screen_size.y / m_cell_height;

		const auto cell_count = m_grid_width * m_grid_height;

		for (int i = 0; i < cell_count; ++i)
		{
			m_cells += L'.';
		}

		seed_grid(m_seed_threshold);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// check for mouse click

		if (GetMouse(0).bReleased)
		{
			clear_grid();
			m_seed_threshold = std::min(m_seed_threshold + 10, 100);
			seed_grid(m_seed_threshold);
			return true;
		}

		if (GetMouse(1).bReleased)
		{
			clear_grid();
			m_seed_threshold = std::max(m_seed_threshold - 10, 0);
			seed_grid(m_seed_threshold);
			return true;
		}

		// delay update

		m_time_since_last_update += fElapsedTime;

		if (m_time_since_last_update < m_update_interval)
		{
			return true;
		}

		m_time_since_last_update = 0;

		// Draw cells

		for (auto y = 0; y < m_grid_height; ++y)
		{
			for (auto x = 0; x < m_grid_width; ++x)
			{
				const auto cell = get_cell(x, y);
				switch (cell)
				{
				case L'.': // dead
					FillRect(m_cell_width * x, m_cell_height * y, m_cell_width, m_cell_height, olc::DARK_GREY);
					break;
				case L'#': // alive
					FillRect(m_cell_width * x, m_cell_height * y, m_cell_width, m_cell_height, olc::GREY);
					break;
				default:
					FillRect(m_cell_width * x, m_cell_height * y, m_cell_width, m_cell_height, olc::BLACK);
					break;
				}
			}
		}

		// Update cell states

		for (auto y = 0; y < m_grid_height; ++y)
		{
			for (auto x = 0; x < m_grid_width; ++x)
			{
				const auto alive_neighbors = get_alive_neighbors(x, y);
				const auto cell = get_cell(x, y);

				if (cell == L'#' && (alive_neighbors == m_become_dead || alive_neighbors == m_become_alive)) // Any live cell with two or three live neighbors survives.
				{
					continue;
				}

				if (cell == L'.' && alive_neighbors == m_become_alive) // Any dead cell with three live neighbors becomes a live cell.
				{
					set_cell(x, y, L'0'); // to become alive
				}
				else // All other live cells die in the next generation.Similarly, all other dead cells stay dead.
				{
					set_cell(x, y, cell == L'#' ? L'X' : L'.'); // to be killed
				}
			}
		}

		// Finalize cell states

		for (auto y = 0; y < m_grid_height; ++y)
		{
			for (auto x = 0; x < m_grid_width; ++x)
			{
				const auto cell = get_cell(x, y);

				switch (cell)
				{
				case L'X': // to be killed
					set_cell(x, y, L'.');
					break;
				case L'0': // to become alive
					set_cell(x, y, L'#');
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

	wchar_t get_cell(int x, int y)
	{
		if (x >= 0 && x < m_grid_width && y >= 0 && y < m_grid_height)
		{
			return m_cells[y * m_grid_width + x]; // access 1d array like a 2d array
		}
		return L' '; // sudo null
	}

	void set_cell(int x, int y, wchar_t c)
	{
		if (x >= 0 && x < m_grid_width && y >= 0 && y < m_grid_height)
		{
			m_cells[y * m_grid_width + x] = c;
		}
	}

	int get_alive_neighbors(int x, int y)
	{
		const int x_start = x > 0 ? x - 1 : x;
		const int x_end = x < m_grid_width ? x + 1 : x;

		const int y_start = y > 0 ? y - 1 : y;
		const int y_end = y < m_grid_height ? y + 1 : y;

		int alive_count = 0;

		for (auto y_pos = y_start; y_pos <= y_end; ++y_pos)
		{
			for (auto x_pos = x_start; x_pos <= x_end; ++x_pos)
			{
				if (y_pos == y && x_pos == x)
				{
					continue; // ignore self
				}

				const auto cell = get_cell(x_pos, y_pos);

				if (cell == L'#' || cell == L'X') // if alive or flagged for death
				{
					alive_count++;
				}
			}
		}

		return alive_count;
	}

	void seed_grid(int threshold)
	{
		for (auto y = 0; y < m_grid_height; ++y)
		{
			for (auto x = 0; x < m_grid_width; ++x)
			{
				if (rand() % 100 > threshold)
				{
					set_cell(x, y, L'#');
				}
				else
				{
					set_cell(x, y, L'.');
				}
			}
		}
	}

	void clear_grid()
	{
		for (auto y = 0; y < m_grid_height; ++y)
		{
			for (auto x = 0; x < m_grid_width; ++x)
			{
				set_cell(x, y, L'.');
			}
		}
	}
};

int main()
{
	application app;
	if (app.Construct(application::screen_width, application::screen_height, application::pixel_width, application::pixel_height))
	{
		app.Start();
	}

	return 0;
}
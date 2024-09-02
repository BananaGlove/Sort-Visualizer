#include <iostream>
#include <algorithm>
#include <string>
#include <array>
#include <queue>
#include <raylib.h>

// ---------- CONSTEXPRS ---------- //
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;
constexpr int ARR_SIZE = 200;

// ---------- TYPEDEFS ---------- //
typedef std::array<std::pair<int, Color>, ARR_SIZE> VisualisedArray;
typedef std::pair<int, int> SwapPair;
typedef std::queue<SwapPair> SwapQueue;

// ---------- SIGNATURES ---------- //
void drawVerticalBands(const VisualisedArray& array, const SwapPair& current_swap);
void drawHorizontalBands(const VisualisedArray& array, const SwapPair& current_swap);
void generateBubbleSort(const VisualisedArray& array, SwapQueue& swap_queue);
void generateInsertionSort(const VisualisedArray& array, SwapQueue& swap_queue);
void permutate(VisualisedArray& array, int i1, int i2);

int main() {

	// ---------- INITIALIZATIONS ---------- //
	VisualisedArray array;
	SwapQueue swap_queue{};
	bool isVertical = true, shuffled = false;

	constexpr float to_rgb = UINT8_MAX / (float)ARR_SIZE;
	for (int i = 0; i < ARR_SIZE; i++) {
		const uint8_t color_start = i * to_rgb;
		const uint8_t color_end = UINT8_MAX - color_start;

		array[i].first = i + 1;
		array[i].second = Color{ color_start, 0, color_end, 255 };
	}

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sorting Algorithm Visualizer");
	SetTargetFPS(1000);

	// ---------- EVENT LOOP ---------- //
	while (!WindowShouldClose()) {
		const std::string title = "Sorting Algorithm Visualizer, fps: " + std::to_string(GetFPS());
		SetWindowTitle(title.c_str());

		switch (GetKeyPressed()) {
		case KEY_SPACE:
			isVertical = !isVertical;
			break;
		case KEY_ZERO:
			std::random_shuffle(array.begin(), array.end());
			swap_queue = SwapQueue(); // reset swap queue
			shuffled = true;
			break;
		case KEY_ONE:
			if (shuffled) {
				generateBubbleSort(array, swap_queue);
				shuffled = false;
			}
			break;
		case KEY_TWO:
			if (shuffled) {
				generateInsertionSort(array, swap_queue);
				shuffled = false;
			}
			break;
		}

		// ---------- DRAW ---------- //
		BeginDrawing();
		ClearBackground(BLACK);

		// Handle swap queue
		if (!swap_queue.empty()) {
			const SwapPair current_swap = swap_queue.front();
			swap_queue.pop();

			if (isVertical) drawVerticalBands(array, current_swap);
			else drawHorizontalBands(array, current_swap);

			permutate(array, current_swap.first, current_swap.second);
		}
		else {
			if (isVertical) drawVerticalBands(array, SwapPair(-1, -1));
			else drawHorizontalBands(array, SwapPair(-1, -1));

			// No more swaps + Not shuffled
			if (!shuffled) DrawText("SORTED!", 20, 20, 50, YELLOW);
		}

		EndDrawing();
	}

	return EXIT_SUCCESS;
}



void drawVerticalBands(const VisualisedArray& array, const SwapPair& current_swap) {
	constexpr int band_width = WINDOW_WIDTH / ARR_SIZE;
	constexpr float band_height_ratio = WINDOW_HEIGHT / (float)ARR_SIZE;

	for (int i = 0; i < ARR_SIZE; i++) {
		const Color bar_color = (i == current_swap.first || i == current_swap.second) ? YELLOW : array[i].second;
		const int band_height = (float)(array[i].first * band_height_ratio);

		DrawRectangle(i * band_width + 1, WINDOW_HEIGHT - band_height, band_width - 1, band_height, bar_color);
	}
}



void drawHorizontalBands(const VisualisedArray& array, const SwapPair& current_swap) {
	constexpr int band_height = WINDOW_HEIGHT / ARR_SIZE;
	constexpr float band_width_ratio = WINDOW_WIDTH / (float)ARR_SIZE;

	for (int i = 0; i < ARR_SIZE; i++) {
		const Color bar_color = (i == current_swap.first || i == current_swap.second) ? YELLOW : array[i].second;
		const int band_width = array[i].first * band_width_ratio;

		DrawRectangle((WINDOW_WIDTH - band_width) / 2, (i * band_height) + 1, band_width, band_height - 1, bar_color);
	}
}



void generateBubbleSort(const VisualisedArray& array, SwapQueue& swap_queue) {
	VisualisedArray array_copy = array;
	swap_queue = SwapQueue(); // reset queue

	for (int i = ARR_SIZE - 1; i >= 0; i--) {
		for (int j = 0; j < i; j++) {
			if (array_copy[j].first <= array_copy[j + 1].first) continue;

			permutate(array_copy, j, j + 1);
			swap_queue.push(SwapPair(j, j + 1));
		}
	}
}



void generateInsertionSort(const VisualisedArray& array, SwapQueue& swap_queue) {
	VisualisedArray array_copy = array;
	swap_queue = SwapQueue(); // reset queue

	for (int i = 0; i < ARR_SIZE; i++) {
		for (int j = i; j > 0; j--) {
			if (array_copy[j - 1].first <= array_copy[j].first) continue;

			permutate(array_copy, j, j - 1);
			swap_queue.push(SwapPair(j, j - 1));
		}
	}
}



void permutate(VisualisedArray& array, int i1, int i2) {
	std::pair<int, Color> temp = array[i1];
	array[i1] = array[i2];
	array[i2] = temp;
}



int WinMain() {
	main();
}
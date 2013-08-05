#pragma once

#include <random>
#include <utility>

namespace Utility
{
	template <typename T> T clamp(const T& value, const T& low, const T& high)
	{
		return value < low ? low : (value > high ? high : value);
	}

	template <typename T> T next_random(std::mt19937& generator, T bottom, T top)
	{
		std::uniform_int_distribution<T> dist(bottom, top);

		return dist(generator);
	}

	template <typename T> T next_random(std::mt19937& generator, std::pair<T, T> range)
	{
		std::uniform_int_distribution<T> dist(range.first, range.second);

		return dist(generator);
	}

	template <> float next_random<float>(std::mt19937& generator, float bottom, float top)
	{
		std::uniform_real_distribution<float> dist(bottom, top);

		return dist(generator);
	}

	template <> float next_random<float>(std::mt19937& generator, std::pair<float, float> range)
	{
		std::uniform_real_distribution<float> dist(range.first, range.second);

		return dist(generator);
	}

	bool bin_random(std::mt19937& generator)
	{
		std::uniform_int_distribution<unsigned int> dist(1, 100);

		return dist(generator) > 50;
	}
}

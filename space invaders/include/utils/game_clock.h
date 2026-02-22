#pragma once
#include <chrono>

using milliseconds = std::chrono::milliseconds;

class GameClock {
private:
	using clock = std::chrono::steady_clock;
	clock::time_point prev_{};

	size_t frames_count_ = 0;
	float delta_time_ = 0.0f;

public:
	GameClock() = default;
	void in_loop() {
		const auto now = clock::now();
		if (frames_count_ == 0) {
			prev_ = now;
			delta_time_ = 0.0f;
		}
		else {
			delta_time_ = std::chrono::duration<float>(now - prev_).count();
			prev_ = now;
		}
		frames_count_++;
	}

	size_t get_frames_count() const { return frames_count_; };
	float get_delta_time() const { return delta_time_; };
};

class GameTimer {
private:
	using clock = std::chrono::steady_clock;
	clock::time_point prev_{};
	milliseconds duration_{ 0 };
	bool started_ = false;
public:
	bool is_running() {
		const auto now = clock::now();
		return started_ && now - prev_ < duration_;
	}

	/// Tries to trigger the timer. Triggering is not possible until the previously set duration has finished.
	/// @param duration The duration for which the trigger will run.
	/// @return True if the trigger occurred; otherwise, false.
	bool try_trigger(milliseconds duration) {
		if (is_running()) return false;

		started_ = true;
		prev_ = clock::now();
		duration_ = duration;
		return true;
	}

	void release() {
		started_ = true;
	}
};
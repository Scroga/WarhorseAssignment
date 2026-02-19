#pragma once

#include <windows.h>

class InputHandler {
private:
	static constexpr int key_mask_ = 0xFF; // 255

	bool key_down_[256] = {};
	int key_hit_[256] = {};
public:

	InputHandler() = default;
	void on_key_down(int vk) { 
		vk &= key_mask_;
		key_down_[vk] = true;  
		key_hit_[vk]++; 
	}

	void on_key_up(int vk) { 
		vk &= key_mask_;
		key_down_[vk] = false; 
	}

	void on_mouse_down(int vk) { 
		vk &= key_mask_;
		key_down_[vk] = true;
		key_hit_[vk]++;
	}

	void on_mouse_up(int vk) { 
		vk &= key_mask_;
		key_down_[vk] = false; 
	}

	void clear_hits() { memset(key_hit_, 0, sizeof(key_hit_)); }

	bool is_key_down(int key) const { return key_down_[key & key_mask_];}
	bool was_key_hit(int key) const { return key_hit_[key & key_mask_] > 0; }
};
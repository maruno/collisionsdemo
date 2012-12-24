#pragma once

namespace util {
	template<typename T> bool between(T x, T min, T max) {
		if(x > min && x < max) {
			return true;
		}
	
		return false;
	}
}

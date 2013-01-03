#pragma once

#include <memory>
#include <iterator>

#include <boost/random/mersenne_twister.hpp>

#include "tunnelsegment.hpp"

/**
 * Input iterator generator for a tunnel consisting of \ref TunnelSegment
 */
class TunnelGenerator : public std::iterator<std::input_iterator_tag, TunnelSegment> {
 private:
	std::unique_ptr<TunnelSegment> current;

	boost::random::mt19937 gen;
 public:
	TunnelGenerator();

	inline TunnelSegment& operator*();
	inline TunnelSegment* operator->();

	/**
	 * Advances the iterator, creating a new \ref TunnelSegment.
	 *
	 * Invalidates the old iterator.
	 *
	 * \return An advanced iterator.
	 */
	TunnelGenerator& operator++();
	inline TunnelGenerator& operator++(int);
};

TunnelSegment& TunnelGenerator::operator*() {
	return *current;
}

TunnelSegment* TunnelGenerator::operator->() {
	return current.get();
}

TunnelGenerator& TunnelGenerator::operator++(int) {
	return ++(*this);
}
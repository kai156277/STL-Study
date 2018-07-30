#pragma once

#include "tiny_algobase.h"
#include "tiny_config.h"

TINY_STL_BEGIN_NAMESPACE

template<typename InputIterator, typename T>
inline InputIterator find(InputIterator begin, InputIterator end, const T& value)
{
	while (begin != end && *begin != value)
		++begin;

	return begin;
}

TINY_STL_END_NAMESPACE

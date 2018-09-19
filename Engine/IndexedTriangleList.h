#pragma once

#include <vector>
#include "Vec3.h"

template<typename T>
class IndexedTriangleList
{
public:
	IndexedTriangleList( std::vector<T> vert_in, std::vector<size_t> index_in )
		:
		vertices( std::move( vert_in ) ),
		indices( std::move( index_in ) )
	{
		assert( vertices.size() > 2 );
		assert( indices.size() % 3 == 0 );
	}
	std::vector<T> vertices;
	std::vector<size_t> indices;
};
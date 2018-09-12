#pragma once

#include <vector>
#include "Vec3.h"

struct IndexedTriangleList
{
	IndexedTriangleList( std::vector<Vec3> vert_in, std::vector<size_t> index_in )
		:
		vertices( std::move( vert_in ) ),
		indices( std::move( index_in ) )
	{
		assert( vertices.size() > 2 );
		assert( indices.size() % 3 == 0 );
		cullFlags.resize( indices.size() / 3, false );
	}
	std::vector<Vec3> vertices;
	std::vector<size_t> indices;
	std::vector<bool> cullFlags;
};
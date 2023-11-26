#ifndef SYSTEMS_SIMPLEX_HPP
#define SYSTEMS_SIMPLEX_HPP

#include <algorithm>
#include <array>
#include <initializer_list>
#include <glm/glm.hpp>

namespace Systems 
{
	using namespace glm;

	struct Simplex {
	private:
		std::array<vec3, 4> mPoints;
		int mSize;

	public:
		Simplex();

		Simplex& operator=(std::initializer_list<vec3> list);

		void push_front(vec3 point);

		vec3& operator[](int i);
		size_t size() const;

		auto begin() const;
		auto end() const;
	};

	inline Simplex::Simplex()
	{
		mSize = 0;
		mPoints = {};
	}

	inline Simplex& Simplex::operator=(std::initializer_list<vec3> list)
	{
		for (auto v = list.begin(); v != list.end(); v++) {
			mPoints[std::distance(list.begin(), v)] = *v;
		}
		mSize = list.size();

		return *this;
	}

	inline void Simplex::push_front(vec3 point)
	{
		mPoints = { point, mPoints[0], mPoints[1], mPoints[2] };
		mSize = std::min(mSize + 1, 4);
	}

	inline vec3& Simplex::operator[](int i)
	{
		return mPoints[i];
	}

	inline size_t Simplex::size() const
	{
		return mSize;
	}

	inline auto Simplex::begin() const
	{
		return mPoints.begin();
	}

	inline auto Simplex::end() const
	{
		return mPoints.end() - (4 - mSize);
	}
}

#endif
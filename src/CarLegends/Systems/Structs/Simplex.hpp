#ifndef SYSTEMS_SIMPLEX_HPP
#define SYSTEMS_SIMPLEX_HPP

#include <array>
#include <glm/vec3.hpp>

namespace Systems
{
	using namespace glm;
	struct Simplex
	{
		Simplex();

		Simplex& operator=(std::initializer_list<vec3> list);
		void push_front(vec3 point);
		vec3& operator[](int i);
		size_t size() const;

		auto begin() const
		{
			return m_points.begin();
		}

		auto end() const
		{
			return m_points.end() - (4 - m_size);
		}

	private:
		std::array<vec3, 4> m_points;
		int m_size;
	};
}
#endif

#ifndef SYSTEMS_SIMPLEX_HPP
#define SYSTEMS_SIMPLEX_HPP

#include <array>
#include <glm/vec3.hpp>

namespace Systems
{
	using namespace glm;
	struct Simplex
	{
	public:
		Simplex();

		Simplex& operator=(std::initializer_list<vec3> list);
		void push_front(vec3 point);
		vec3& operator[](int i);
		size_t size() const;
		auto begin() const;
		auto end() const;

	private:
		std::array<vec3, 4> m_points;
		int m_size;
	};
}
#endif

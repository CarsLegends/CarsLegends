#include "Simplex.hpp"

namespace Systems
{
	Simplex::Simplex()
		: m_points(), m_size(0)
	{}

	Simplex& Simplex::operator=(std::initializer_list<glm::vec3> list)
	{
		for (auto v = list.begin(); v != list.end(); v++) {
			m_points[std::distance(list.begin(), v)] = *v;
		}
		m_size = list.size();

		return *this;
	}

	void Simplex::push_front(vec3 point)
	{
		m_points = { point, m_points[0], m_points[1], m_points[2] };
		m_size = std::min(m_size + 1, 4);
	}

	vec3& Simplex::operator[](int i)
	{
		return m_points[i];
	}

	size_t Simplex::size() const
	{
		return m_size;
	}
}
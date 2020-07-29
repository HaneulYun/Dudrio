#pragma once

struct Collider {
	float m_x1, m_z1;
	float m_x2, m_z2;

	Collider() {}
	Collider(float x1, float z1, float x2, float z2)
	{
		m_x1 = x1; m_z1 = z1; m_x2 = x2; m_z2 = z2;
	}

	float getMaxBound()
	{
		float xbound = (m_x2 - m_x1) / 2;
		float zbound = (m_z2 - m_z1) / 2;

		return xbound > zbound ? xbound : zbound;
	}
};

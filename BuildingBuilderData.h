#pragma once

struct BuildingBuilderData
{
	std::wstring buildingName{ L"none" };
	union
	{
		GameObject* prefab{ nullptr };
		struct
		{
			Mesh* mesh;
			Material* material;
		};
	};
};

#include "pch.h"
#include "BuildingBuilder.h"


void BuildingBuilder::Start(/*초기화 코드를 작성하세요.*/)
{
	buildingBuilder = this;
}

void BuildingBuilder::Update(/*업데이트 코드를 작성하세요.*/)
{
	if (prefab)
	{
		// 토글 설정
		if (Input::GetKeyDown(KeyCode::T))
			rotationToggle = rotationToggle ? false : true;

		// 건물 회전
		if (Input::GetMouseButtonDown(2))
			lastMousePos = Input::mousePosition;
		else if (Input::GetMouseButton(2))
		{
			if (rotationToggle)
			{
				if (abs(lastMousePos.y - Input::mousePosition.y) > 30)
				{
					prefab->transform->Rotate(Vector3{ 0.0f,1.0f,0.0f }, (lastMousePos.y - Input::mousePosition.y) / abs(lastMousePos.y - Input::mousePosition.y) * 30.0f);
					lastMousePos = Input::mousePosition;
				}
			}
			else
			{
				prefab->transform->Rotate(Vector3{ 0.0f,1.0f,0.0f }, (lastMousePos.y - Input::mousePosition.y) * Time::deltaTime * 60.0f);
				lastMousePos = Input::mousePosition;
			}
		}

		// 건물 건설
		else if (Input::GetMouseButtonUp(0) && prefab->collisionType.empty() && isOnLand() != nullptr)
		{
			GameObject* prePrefab = prefab;
			Matrix4x4 localToWorldMatrix = prefab->GetMatrix();
			makePrefab(curPrefabType, curPrefabIndex);
			prefab->transform->localToWorldMatrix = localToWorldMatrix;

			auto p = prefab->transform->position;
			prefab->AddComponent<Building>()->setBuildingInform(curLandmark, curPrefabType, curPrefabIndex);
			prefab->tag = TAG_BUILDING;

			if (HostNetwork::network->isConnect) {
				Vector3 building_forward = prefab->transform->forward;
				building_forward.y = 0;
				building_forward.Normalize();
				Vector3 forward = { 0,0,1 };
				float angle = Vector3::DotProduct(forward, building_forward);
				Vector3 dir = Vector3::CrossProduct(forward, building_forward);
				angle = XMConvertToDegrees(acos(angle));
				angle *= (dir.y > 0.0f) ? 1.0f : -1.0f;
				HostNetwork::network->send_construct_packet(curPrefabType, curPrefabIndex, p.x, p.z, angle);
			}
			updateTerrainNodeData(prefab, true);

			GameWorld::gameWorld->buildInGameWorld(curLandmark, prefab, curPrefabType, curPrefabIndex);

			// 연속 건설
			if (!Input::GetKey(KeyCode::Shift))
			{
				prefab = nullptr;
				Scene::scene->PushDelete(prePrefab);
				Scene::scene->spatialPartitioningManager.tagData.SetTagCollision(TAG_BUILDING, TAG_PREVIEW, false);
			}
			else
				prefab = prePrefab;
		}
		else
			prefab->transform->position = getPosOnTerrain();
	}

	else if (Input::GetKey(KeyCode::X))
		pickToDelete();
}

void BuildingBuilder::serializeBuildings()
{
	/*AdvancedVillagePack*/
	building[Landmark].push_back(makeBuilderDataAsPrefab(L"Well", ASSET PREFAB("Well")));
	building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"House01", ASSET MESH("SM_House_Var01"), ASSET MATERIAL("house01")));
	building[House].push_back(makeBuilderDataAsPrefab(L"House02", ASSET PREFAB("House01")));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_01", ASSET MESH("SM_Tree_Var01"), { ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_02", ASSET MESH("SM_Tree_Var02"), { ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_03", ASSET MESH("SM_Tree_Var03"), { ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_04", ASSET MESH("SM_Tree_Var04"), { ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_05", ASSET MESH("SM_Tree_Var05"), { ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_01", ASSET MESH("SM_Mushroom_Var01"), ASSET MATERIAL("material_02")));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_02", ASSET MESH("SM_Mushroom_Var02"), ASSET MATERIAL("material_02")));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_03", ASSET MESH("SM_Mushroom_Var03"), ASSET MATERIAL("material_02")));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_04", ASSET MESH("SM_Mushroom_Var04"), ASSET MATERIAL("material_02")));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_05", ASSET MESH("SM_Mushroom_Var05"), ASSET MATERIAL("material_02")));
	building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_06", ASSET MESH("SM_Mushroom_Var06"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence_01", ASSET MESH("SM_Fence_Var01"), ASSET MATERIAL("material_01")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence_02", ASSET MESH("SM_Fence_Var02"), ASSET MATERIAL("material_01")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence_03", ASSET MESH("SM_Fence_Var03"), ASSET MATERIAL("material_01")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence_04", ASSET MESH("SM_Fence_Var04"), ASSET MATERIAL("material_01")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Bucket", ASSET MESH("SM_Bucket"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Barrel", ASSET MESH("SM_Barrel"), ASSET MATERIAL("material_01")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Spike",	ASSET MESH("SM_Spike"),	ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Cart_01", ASSET MESH("SM_Cart_Var01"), ASSET MATERIAL("material_01")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Cart_02", ASSET MESH("SM_Cart_Var02"), ASSET MATERIAL("material_01")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Cauldron", ASSET MESH("SM_Cauldron"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_01", ASSET MESH("SM_Pot_Var01"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_02", ASSET MESH("SM_Pot_Var02"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_03", ASSET MESH("SM_Pot_Var03"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_04", ASSET MESH("SM_Pot_Var04"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_05", ASSET MESH("SM_Pot_Var05"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Pot_06", ASSET MESH("SM_Pot_Var06"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Apple", ASSET MESH("SM_Apple"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Potato", ASSET MESH("SM_Potato"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Tomato", ASSET MESH("SM_Tomato"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Watermellon", ASSET MESH("SM_Watermellon"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_Apple", ASSET MESH("SM_Sack_Apple"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_Flour", ASSET MESH("SM_Sack_Flour"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_Potato", ASSET MESH("SM_Sack_Potato"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_Tomato", ASSET MESH("SM_Sack_Tomato"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_01", ASSET MESH("SM_Sack_Var01"), ASSET MATERIAL("material_02")));
	building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"Sack_02", ASSET MESH("SM_Sack_Var02"), ASSET MATERIAL("material_02")));




	/*HandPaintedEnviroment*/
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Altar", ASSET MESH("Altar"), ASSET MATERIAL("Lampposts")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Anvil", ASSET MESH("Anvil"), ASSET MATERIAL("Hammer_anvil")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Baker_house", ASSET MESH("Baker_house"), ASSET MATERIAL("Baker_house")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Barrel", ASSET MESH("Barrel"), ASSET MATERIAL("Barrel")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Box_1", ASSET MESH("Box_1"), ASSET MATERIAL("Boxes")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Box_2", ASSET MESH("Box_2"), ASSET MATERIAL("Boxes")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Box_3", ASSET MESH("Box_3"), ASSET MATERIAL("Boxes")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Box_4", ASSET MESH("Box_4"), ASSET MATERIAL("Boxes")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Bush_1", ASSET MESH("Bush_1"), ASSET MATERIAL("Plants")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Bush_2", ASSET MESH("Bush_2"), ASSET MATERIAL("Plants")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Cauldron", ASSET MESH("Cauldron"), ASSET MATERIAL("Cauldron")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Crystal_1", ASSET MESH("Crystal_1"), ASSET MATERIAL("Crystals")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Crystal_2", ASSET MESH("Crystal_2"), ASSET MATERIAL("Crystals")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Crystal_3", ASSET MESH("Crystal_3"), ASSET MATERIAL("Crystals")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Crystal_4", ASSET MESH("Crystal_4"), ASSET MATERIAL("Crystals")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Crystal_5", ASSET MESH("Crystal_5"), ASSET MATERIAL("Crystals")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence", ASSET MESH("Fence"), ASSET MATERIAL("Fence")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence_1", ASSET MESH("Fence_1"), ASSET MATERIAL("Fence")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Fence_2", ASSET MESH("Fence_2"), ASSET MATERIAL("Fence")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Fern", ASSET MESH("Fern"), ASSET MATERIAL("Fern")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Flower", ASSET MESH("Flower"), ASSET MATERIAL("Plants")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Flowers_1", ASSET MESH("Flowers_1"), ASSET MATERIAL("Plants")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Flowers_2", ASSET MESH("Flowers_2"), ASSET MATERIAL("Plants")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Forge", ASSET MESH("Forge"), ASSET MATERIAL("Forge")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Hammer", ASSET MESH("Hammer"), ASSET MATERIAL("Hammer_anvil")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Lamppost", ASSET MESH("Lamppost"), ASSET MATERIAL("Lampposts")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Large_house", ASSET MESH("Large_house"), ASSET MATERIAL("Large_house")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Log", ASSET MESH("Log"), ASSET MATERIAL("Log_stump_mushrooms")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Log_1", ASSET MESH("Log_1"), ASSET MATERIAL("Logs")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Log_2", ASSET MESH("Log_2"), ASSET MATERIAL("Logs")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Log_3", ASSET MESH("Log_3"), ASSET MATERIAL("Logs")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Mill", ASSET MESH("Mill"), ASSET MATERIAL("Mill")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Mine", ASSET MESH("Mine"), ASSET MATERIAL("Mine")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_1", ASSET MESH("Mushroom_1"), ASSET MATERIAL("Log_stump_mushrooms")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_2", ASSET MESH("Mushroom_2"), ASSET MATERIAL("Log_stump_mushrooms")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_3", ASSET MESH("Mushroom_3"), ASSET MATERIAL("Log_stump_mushrooms")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Mushroom_4", ASSET MESH("Mushroom_4"), ASSET MATERIAL("Log_stump_mushrooms")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Pillar", ASSET MESH("Pillar"), ASSET MATERIAL("Pillars_arch")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Pillar_ruined_1", ASSET MESH("Pillar_ruined_1"), ASSET MATERIAL("Pillars_arch")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Pine_tree", ASSET MESH("Pine_tree"), ASSET MATERIAL("Pine_tree")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Pine_tree_2", ASSET MESH("Pine_tree_2"), ASSET MATERIAL("Pine_tree_2")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Plant", ASSET MESH("Plant"), ASSET MATERIAL("Plant")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Rock_1", ASSET MESH("Rock_1"), ASSET MATERIAL("Rocks_1_2")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Rock_2", ASSET MESH("Rock_2"), ASSET MATERIAL("Rocks_1_2")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Rock_3", ASSET MESH("Rock_3"), ASSET MATERIAL("Rocks_3_4")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Rock_4", ASSET MESH("Rock_4"), ASSET MATERIAL("Rocks_3_4")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Small_house", ASSET MESH("Small_house"), ASSET MATERIAL("Small_house")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Stone_fence", ASSET MESH("Stone_fence"), ASSET MATERIAL("Stone_fence")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Street_light", ASSET MESH("street_light"), ASSET MATERIAL("Street_light")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Stump", ASSET MESH("Stump"), ASSET MATERIAL("Log_stump_mushrooms")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tavern", ASSET MESH("Tavern"), ASSET MATERIAL("Tavern")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tile_1", ASSET MESH("Tile_1"), ASSET MATERIAL("Stone_tiles")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tile_2", ASSET MESH("Tile_2"), ASSET MATERIAL("Stone_tiles")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tile_3", ASSET MESH("Tile_3"), ASSET MATERIAL("Stone_tiles")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tile_4", ASSET MESH("Tile_4"), ASSET MATERIAL("Stone_tiles")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tile_5", ASSET MESH("Tile_5"), ASSET MATERIAL("Stone_tiles")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tile_6", ASSET MESH("Tile_6"), ASSET MATERIAL("Stone_tiles")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tile_7", ASSET MESH("Tile_7"), ASSET MATERIAL("Stone_tiles")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tower", ASSET MESH("Tower"), ASSET MATERIAL("Ruined_tower")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Town_Hall", ASSET MESH("Town_Hall"), ASSET MATERIAL("Town_Hall")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tree_1", ASSET MESH("Tree_1"), ASSET MATERIAL("Tree_1_1")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tree_2", ASSET MESH("Tree_2"), ASSET MATERIAL("Tree_2_1")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tree_2_1", ASSET MESH("Tree_2_1"), ASSET MATERIAL("Tree_2_2")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Tree_4", ASSET MESH("Tree_4"), ASSET MATERIAL("Tree_1_2")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Wall_1", ASSET MESH("Wall_1"), ASSET MATERIAL("Ruined_walls")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Wall_2", ASSET MESH("Wall_2"), ASSET MATERIAL("Ruined_walls")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Well", ASSET MESH("Well"), ASSET MATERIAL("Well")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Witch_house", ASSET MESH("Witch_house"), ASSET MATERIAL("Witch_house")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"Woodmill", ASSET MESH("Woodmill"), ASSET MATERIAL("Woodmill")));



	/*SpringEnviromnet*/
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Baloon_01_SM", ASSET MESH("SE_Baloon_01_SM_LOD0"), ASSET MATERIAL("SE_Baloons")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Baloon_02_SM", ASSET MESH("SE_Baloon_02_SM_LOD0"), ASSET MATERIAL("SE_Baloons")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Baloon_03_SM", ASSET MESH("SE_Baloon_03_SM_LOD0"), ASSET MATERIAL("SE_Baloons")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Baloon_04_SM", ASSET MESH("SE_Baloon_04_SM_LOD0"), ASSET MATERIAL("SE_Baloons")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Baloons_01_SM", ASSET MESH("SE_Baloons_01_SM_LOD0"), ASSET MATERIAL("SE_Baloons")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bench_01a_SM", ASSET MESH("SE_Bench_01a_SM_LOD0"), ASSET MATERIAL("SE_Bench_01a")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bench_01b_SM", ASSET MESH("SE_Bench_01b_SM_LOD0"), ASSET MATERIAL("SE_Bench_01b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_01_SM", ASSET MESH("SE_Birch_01_SM_LOD0"), ASSET MATERIAL("SE_Birch")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_01b_SM", ASSET MESH("SE_Birch_01b_SM_LOD0"), ASSET MATERIAL("SE_Birch_01b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_02_SM", ASSET MESH("SE_Birch_02_SM_LOD0"), ASSET MATERIAL("SE_Birch")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_02b_SM", ASSET MESH("SE_Birch_02b_SM_LOD0"), ASSET MATERIAL("SE_Birch_01b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_03_SM", ASSET MESH("SE_Birch_03_SM_LOD0"), ASSET MATERIAL("SE_Birch")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_03b_SM", ASSET MESH("SE_Birch_03b_SM_LOD0"), ASSET MATERIAL("SE_Birch_01b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Blanket_01_SM", ASSET MESH("SE_Blanket_01_SM_LOD0"), ASSET MATERIAL("SE_Blanket")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Blanket_02_SM", ASSET MESH("SE_Blanket_02_SM_LOD0"), ASSET MATERIAL("SE_Blanket")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Blossom_01_SM", ASSET MESH("SE_Blossom_01_SM_LOD0"), ASSET MATERIAL("SE_Bushese")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Blossom_02_SM", ASSET MESH("SE_Blossom_02_SM_LOD0"), ASSET MATERIAL("SE_Bushese")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bonsai_01_SM", ASSET MESH("SE_Bonsai_01_SM_LOD0"), ASSET MATERIAL("SE_Bonsai")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bonsai_02_SM", ASSET MESH("SE_Bonsai_02_SM_LOD0"), ASSET MATERIAL("SE_Bonsai")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Booth_01_SM", ASSET MESH("SE_Booth_01_SM_LOD0"), ASSET MATERIAL("SE_Booth")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Booth_02_SM", ASSET MESH("SE_Booth_02_SM_LOD0"), ASSET MATERIAL("SE_Booth")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Branch_01_SM", ASSET MESH("SE_Branch_01_SM_LOD0"), ASSET MATERIAL("SE_Bushese")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Branch_02_SM", ASSET MESH("SE_Branch_02_SM_LOD0"), ASSET MATERIAL("SE_Bushese")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bush_06_SM", ASSET MESH("SE_Bush_06_SM_LOD0"), ASSET MATERIAL("SE_Bonsai")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bush_07_SM", ASSET MESH("SE_Bush_07_SM_LOD0"), ASSET MATERIAL("SE_Bonsai")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Candy_01_SM", ASSET MESH("SE_Candy_01_SM_LOD0"), ASSET MATERIAL("SE_Candies")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Candy_02_SM", ASSET MESH("SE_Candy_02_SM_LOD0"), ASSET MATERIAL("SE_Candies")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Candy_03_SM", ASSET MESH("SE_Candy_03_SM_LOD0"), ASSET MATERIAL("SE_Candies")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Candy_04_SM", ASSET MESH("SE_Candy_04_SM_LOD0"), ASSET MATERIAL("SE_Candies")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_CandyHouse_01_SM", ASSET MESH("SE_CandyHouse_01_SM_LOD0"), ASSET MATERIAL("SE_Candyhouse")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Carousel_01_SM", ASSET MESH("SE_Carousel_01_SM_LOD0"), ASSET MATERIAL("SE_Carousel")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Cart_01_SM", ASSET MESH("SE_Cart_01_SM_LOD0"), ASSET MATERIAL("SE_Cart")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Chair_01a_SM", ASSET MESH("SE_Chair_01a_SM_LOD0"), ASSET MATERIAL("SE_Bench_01a")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Climber_01_SM", ASSET MESH("SE_Climber_01_SM_LOD0"), ASSET MATERIAL("SE_Climbers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Climber_02_SM", ASSET MESH("SE_Climber_02_SM_LOD0"), ASSET MATERIAL("SE_Climbers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_CottonCandy_01_SM", ASSET MESH("SE_CottonCandy_01_SM_LOD0"), ASSET MATERIAL("SE_CottonCandyTree")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_CottonCandyTree_01_SM", ASSET MESH("SE_CottonCandyTree_01_SM_LOD0"), ASSET MATERIAL("SE_CottonCandyTree")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_CottonCandyTree_02_SM", ASSET MESH("SE_CottonCandyTree_02_SM_LOD0"), ASSET MATERIAL("SE_CottonCandyTree")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_CottonCandyTree_03_SM", ASSET MESH("SE_CottonCandyTree_03_SM_LOD0"), ASSET MATERIAL("SE_CottonCandyTree")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_FabricHouse_01_SM", ASSET MESH("SE_FabricHouse_01_SM_LOD0"), ASSET MATERIAL("SE_FabricHouse")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Factory_01_SM", ASSET MESH("SE_Factory_01_SM_LOD0"), ASSET MATERIAL("SE_Factory")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Fence_07a_SM", ASSET MESH("SE_Fence_07a_SM_LOD0"), ASSET MATERIAL("SE_Fence_01a")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Fence_07b_SM", ASSET MESH("SE_Fence_07b_SM_LOD0"), ASSET MATERIAL("SE_Fence_01b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Fence_08a_SM", ASSET MESH("SE_Fence_08a_SM_LOD0"), ASSET MATERIAL("SE_Fence_01a")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Fence_08b_SM", ASSET MESH("SE_Fence_08b_SM_LOD0"), ASSET MATERIAL("SE_Fence_01b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_01_SM", ASSET MESH("SE_Flower_01_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_03_SM", ASSET MESH("SE_Flower_03_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_04_SM", ASSET MESH("SE_Flower_04_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_06_SM", ASSET MESH("SE_Flower_06_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_07_SM", ASSET MESH("SE_Flower_07_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_08_SM", ASSET MESH("SE_Flower_08_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_10_SM", ASSET MESH("SE_Flower_10_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_11_SM", ASSET MESH("SE_Flower_11_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_13_SM", ASSET MESH("SE_Flower_13_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_14_SM", ASSET MESH("SE_Flower_14_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_16_SM", ASSET MESH("SE_Flower_16_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_40_SM", ASSET MESH("SE_Flower_40_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_41_SM", ASSET MESH("SE_Flower_41_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_42_SM", ASSET MESH("SE_Flower_42_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_43_SM", ASSET MESH("SE_Flower_43_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_44_SM", ASSET MESH("SE_Flower_44_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_45_SM", ASSET MESH("SE_Flower_45_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_46_SM", ASSET MESH("SE_Flower_46_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_47_SM", ASSET MESH("SE_Flower_47_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_48_SM", ASSET MESH("SE_Flower_48_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_49_SM", ASSET MESH("SE_Flower_49_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_50_SM", ASSET MESH("SE_Flower_50_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_51_SM", ASSET MESH("SE_Flower_51_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_52_SM", ASSET MESH("SE_Flower_52_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_71_SM", ASSET MESH("SE_Flower_71_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_72_SM", ASSET MESH("SE_Flower_72_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_73_SM", ASSET MESH("SE_Flower_73_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_74_SM", ASSET MESH("SE_Flower_74_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_75_SM", ASSET MESH("SE_Flower_75_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_76_SM", ASSET MESH("SE_Flower_76_SM_LOD0"), ASSET MATERIAL("SE_Flowers")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Gate_01b_SM", ASSET MESH("SE_Gate_01b_SM_LOD0"), ASSET MATERIAL("SE_Gate")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Gazebo_01_SM", ASSET MESH("
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Grill_02_SM", ASSET MESH("SE_Grill_02_SM_LOD0"), ASSET MATERIAL("SE_Grill")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_HeartHouse_01_SM", ASSET MESH("SE_HeartHouse_01_SM_LOD0"), ASSET MATERIAL("SE_HeartHouse")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_01_SM", ASSET MESH("SE_Hedge_01_SM_LOD0"), ASSET MATERIAL("SE_Hedges")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_02_SM", ASSET MESH("SE_Hedge_02_SM_LOD0"), ASSET MATERIAL("SE_Hedges")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_03_SM", ASSET MESH("SE_Hedge_03_SM_LOD0"), ASSET MATERIAL("SE_Hedges")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_04_SM", ASSET MESH("SE_Hedge_04_SM_LOD0"), ASSET MATERIAL("SE_Hedges")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_05_SM", ASSET MESH("SE_Hedge_05_SM_LOD0"), ASSET MATERIAL("SE_Hedges")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_06_SM", ASSET MESH("SE_Hedge_06_SM_LOD0"), ASSET MATERIAL("SE_Hedges")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_07_SM", ASSET MESH("SE_Hedge_07_SM_LOD0"), ASSET MATERIAL("SE_Hedges")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_08_SM", ASSET MESH("SE_Hedge_08_SM_LOD0"), ASSET MATERIAL("SE_Hedges")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_HotAirBaloon_SM", ASSET MESH("SE_HotAirBaloon_SM_LOD0"), ASSET MATERIAL("SE_HotAirBaloon")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_01a_SM", ASSET MESH("SE_House_01a_SM_LOD0"), ASSET MATERIAL("SE_House_01a")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_01b_SM", ASSET MESH("SE_House_01b_SM_LOD0"), ASSET MATERIAL("SE_House_01b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_01c_SM", ASSET MESH("SE_House_01c_SM_LOD0"), ASSET MATERIAL("SE_House_01c")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_01d_SM", ASSET MESH("SE_House_01d_SM_LOD0"), ASSET MATERIAL("SE_House_01d")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_02a_SM", ASSET MESH("SE_House_02a_SM_LOD0"), ASSET MATERIAL("SE_House_02a")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_02b_SM", ASSET MESH("SE_House_02b_SM_LOD0"), ASSET MATERIAL("SE_House_02b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_02c_SM", ASSET MESH("SE_House_02c_SM_LOD0"), ASSET MATERIAL("SE_House_02c")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_02d_SM", ASSET MESH("SE_House_02d_SM_LOD0"), ASSET MATERIAL("SE_House_02d")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_03d_SM", ASSET MESH("SE_House_03d_SM_LOD0"), ASSET MATERIAL("SE_House_03d")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_03a_SM", ASSET MESH("SE_House_03a_SM_LOD0"), ASSET MATERIAL("SE_House_03a")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_03b_SM", ASSET MESH("SE_House_03b_SM_LOD0"), ASSET MATERIAL("SE_House_03b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_03c_SM", ASSET MESH("SE_House_03c_SM_LOD0"), ASSET MATERIAL("SE_House_03c")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_04a_SM", ASSET MESH("SE_House_04a_SM_LOD0"), ASSET MATERIAL("SE_House_04a")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_04b_SM", ASSET MESH("SE_House_04b_SM_LOD0"), ASSET MATERIAL("SE_House_04b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_05a_SM", ASSET MESH("SE_House_05a_SM_LOD0"), ASSET MATERIAL("SE_House_05a")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_05b_SM", ASSET MESH("SE_House_05b_SM_LOD0"), ASSET MATERIAL("SE_House_05b")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Lamp_01_SM", ASSET MESH("SE_Lamp_01_SM_LOD0"), ASSET MATERIAL("SE_Lamps")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Lamp_02_SM", ASSET MESH("SE_Lamp_02_SM_LOD0"), ASSET MATERIAL("SE_Lamps")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Maypole_01_SM", ASSET MESH("SE_Maypole_01_SM_LOD0"), ASSET MATERIAL("SE_Maypole_01")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_01_SM", ASSET MESH("SE_Present_01_SM_LOD0"), ASSET MATERIAL("SE_Presents")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_02_SM", ASSET MESH("SE_Present_02_SM_LOD0"), ASSET MATERIAL("SE_Presents")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_03_SM", ASSET MESH("SE_Present_03_SM_LOD0"), ASSET MATERIAL("SE_Presents")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_04_SM", ASSET MESH("SE_Present_04_SM_LOD0"), ASSET MATERIAL("SE_Presents")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_05_SM", ASSET MESH("SE_Present_05_SM_LOD0"), ASSET MATERIAL("SE_Presents")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_06_SM", ASSET MESH("SE_Present_06_SM_LOD0"), ASSET MATERIAL("SE_Presents")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Pump_01_SM", ASSET MESH("SE_Pump_01_SM_LOD0"), ASSET MATERIAL("SE_Pump")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_01_SM", ASSET MESH("SE_Rose_01_SM_LOD0"), ASSET MATERIAL("SE_Roses")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_02_SM", ASSET MESH("SE_Rose_02_SM_LOD0"), ASSET MATERIAL("SE_Roses")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_03_SM", ASSET MESH("SE_Rose_03_SM_LOD0"), ASSET MATERIAL("SE_Roses")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_04_SM", ASSET MESH("SE_Rose_04_SM_LOD0"), ASSET MATERIAL("SE_Roses")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_05_SM", ASSET MESH("SE_Rose_05_SM_LOD0"), ASSET MATERIAL("SE_Roses")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_06_SM", ASSET MESH("SE_Rose_06_SM_LOD0"), ASSET MATERIAL("SE_Roses")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_SeeSaw_01_SM", ASSET MESH("SE_SeeSaw_01_SM_LOD0"), ASSET MATERIAL("SE_Seesaw")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Sign_01_SM", ASSET MESH("SE_Sign_01_SM_LOD0"), ASSET MATERIAL("SE_Sign")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Slide_01_SM", ASSET MESH("SE_Slide_01_SM_LOD0"), ASSET MATERIAL("SE_Slide")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_01_SM", ASSET MESH("SE_Stone_01_SM_LOD0"), ASSET MATERIAL("SE_Stones")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_02_SM", ASSET MESH("SE_Stone_02_SM_LOD0"), ASSET MATERIAL("SE_Stones")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_03_SM", ASSET MESH("SE_Stone_03_SM_LOD0"), ASSET MATERIAL("SE_Stones")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_04_SM", ASSET MESH("SE_Stone_04_SM_LOD0"), ASSET MATERIAL("SE_Stones")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_05_SM", ASSET MESH("SE_Stone_05_SM_LOD0"), ASSET MATERIAL("SE_Stones")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_06_SM", ASSET MESH("SE_Stone_06_SM_LOD0"), ASSET MATERIAL("SE_Stones")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Swing_01_SM", ASSET MESH("SE_Swing_01_SM_LOD0"), ASSET MATERIAL("SE_Swing")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Swing_02_SM", ASSET MESH("SE_Swing_02_SM_LOD0"), ASSET MATERIAL("SE_Swing")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Table_01a_SM", ASSET MESH("SE_Table_01a_SM_LOD0"), ASSET MATERIAL("SE_Tables")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Table_01b_SM", ASSET MESH("SE_Table_01b_SM_LOD0"), ASSET MATERIAL("SE_Tables")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Table_02a_SM", ASSET MESH("SE_Table_02a_SM_LOD0"), ASSET MATERIAL("SE_Tables")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Table_02b_SM", ASSET MESH("SE_Table_02b_SM_LOD0"), ASSET MATERIAL("SE_Tables")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Theater_01_SM", ASSET MESH("SE_Theater_01_SM_LOD0"), ASSET MATERIAL("SE_Theater")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_TownHall_SM", ASSET MESH("SE_TownHall_SM_LOD0"), ASSET MATERIAL("SE_TownHall")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Trampoline_01_SM", ASSET MESH("SE_Trampoline_01_SM_LOD0"), ASSET MATERIAL("SE_Trampoline")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Willow_01a_SM", ASSET MESH("SE_Willow_01a_SM_LOD0"), ASSET MATERIAL("SE_Willow")));
	//building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Willow_01b_SM", ASSET MESH("SE_Willow_01b_SM_LOD0"), ASSET MATERIAL("SE_Willow")));


	//ofstream out("colliders.txt");
	//for (int i = 0; i < BuildingType::Count; ++i) {
	//	int j = 0;
	//	for (auto b : building[i]) {
	//		if (b.prefab) {
	//			auto box = b.prefab->GetComponent<BoxCollider>();
	//			out << i << " " << j << " " << box->center.x - box->extents.x << " " << box->center.y - box->extents.y << " " << box->center.x + box->extents.x << " " << box->center.y + box->extents.y << endl;
	//		}
	//		else {
	//			auto box = b.mesh->Bounds;
	//			out << i << " " << j << " " << box.Center.x - box.Extents.x << " " << box.Center.x - box.Extents.y << " " << box.Center.x + box.Extents.x << " " << box.Center.x + box.Extents.y << endl;
	//		}
	//		++j;
	//	}
	//}
	//out.close();
}

BuildingBuilderData BuildingBuilder::makeBuilderDataAsPrefab(wstring name, GameObject* prefab)
{
	BuildingBuilderData data{ name };
	data.prefab = prefab;
	return data;
}

BuildingBuilderData BuildingBuilder::makeBuilderDataAsMeshAndMaterial(wstring name, Mesh* mesh, Material* material)
{
	BuildingBuilderData data{ name };
	data.mesh = mesh;
	data.material = material;
	return data;
}

BuildingBuilderData BuildingBuilder::makeBuilderDataAsMeshAndMaterials(wstring name, Mesh* mesh, vector<Material*> materials)
{
	BuildingBuilderData data{ name };
	data.mesh = mesh;
	data.materials = materials;
	return data;
}

GameObject* BuildingBuilder::isOnLand()
{
	for (auto& landmark : GameWorld::gameWorld->buildingList)
	{
		float distance = sqrt(pow(prefab->transform->position.x - landmark.first->transform->position.x, 2) + pow(prefab->transform->position.z - landmark.first->transform->position.z, 2));

		if (distance <= landmark.first->GetComponent<Village>()->radiusOfLand)
		{
			curLandmark = landmark.first;
			return landmark.first;
		}
	}
	return nullptr;
}

void BuildingBuilder::updateTerrainNodeData(GameObject* building, bool collision)
{
	BoundingBox boundingBox = building->GetComponent<BoxCollider>()->boundingBox;

	Vector3 pos = building->transform->position;
	Vector3 right = Vector3::Normalize(building->transform->right) + boundingBox.Extents.x;
	Vector3 forward = Vector3::Normalize(building->transform->forward) + boundingBox.Extents.z;

	for (int x = (pos - right).x; x <= (pos + right).x; ++x)
	{
		for (int z = (pos - forward).z; z <= (pos + forward).z; ++z)
		{

			BoundingOrientedBox obbBox{};
			obbBox.Center = pos.xmf3;
			obbBox.Extents = boundingBox.Extents;
			obbBox.Orientation = building->transform->localToWorldMatrix.QuaternionRotationMatrix().xmf4;

			if (obbBox.Contains(XMLoadFloat3(&XMFLOAT3(x, pos.y, z))))
			{
				terrainNodeData->extraData[x + (z * terrain->terrainData.heightmapHeight)].collision = collision;

				// 노드 확인용
				//if (cube)
				//{
				//	auto go = Scene::scene->Duplicate(cube);
				//	go->transform->position = Vector3(x, terrain->terrainData.GetHeight(x, z), z);
				//}
			}
		}
	}

}

GameObject* BuildingBuilder::build(Vector2 position, float angle, int type, int index)
{
	if (index < building[type].size())
	{
		GameObject* obj;

		auto data = building[type][index];
		if (data.prefab)
			obj = Scene::scene->Duplicate(data.prefab);
		else
		{
			obj = Scene::scene->CreateEmpty();
			obj->AddComponent<BoxCollider>()->boundingBox = data.mesh->Bounds;

			auto child = obj->AddChild();
			child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			child->AddComponent<MeshFilter>()->mesh = data.mesh;
			if (data.material)
				child->AddComponent<Renderer>()->materials.push_back(data.material);
			else
			{
				auto renderer = child->AddComponent<Renderer>();
				int i = 0;
				for (auto& sm : data.mesh->DrawArgs)
					renderer->materials.push_back(data.materials[i++]);
			}
		}

		Vector3 pos{ position.x, terrain->terrainData.GetHeight(position.x,position.y), position.y };
		obj->transform->position = pos;
		obj->transform->Rotate(Vector3(0, 1, 0), angle);

		updateTerrainNodeData(obj, true);

		return obj;
	}
	return nullptr;
}

void BuildingBuilder::build(Vector3 position)
{
	prefab->transform->position = position;
	prefab = nullptr;
}

void BuildingBuilder::makePrefab(int type, int index)
{
	if (index < building[type].size())
	{
		auto data = building[type][index];
		if (data.prefab)
			prefab = Scene::scene->Duplicate(data.prefab);
		else
		{
			prefab = Scene::scene->CreateEmpty();
			prefab->AddComponent<BoxCollider>()->boundingBox = data.mesh->Bounds;

			auto child = prefab->AddChild();
			child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			child->AddComponent<MeshFilter>()->mesh = data.mesh;
			if (data.material)
				child->AddComponent<Renderer>()->materials.push_back(data.material);
			else
			{
				auto renderer = child->AddComponent<Renderer>();
				int i = 0;
				for (auto& sm : data.mesh->DrawArgs)
					renderer->materials.push_back(data.materials[i++]);
			}
		}
	}
}

void BuildingBuilder::exitBuildMode()
{

}

void BuildingBuilder::enterBuildMode(int type, int index)
{
	makePrefab(type, index);

	if (prefab == nullptr) return;

	prefab->tag = TAG_PREVIEW;
	for (auto& child : prefab->children)
	{
		child->layer = (int)RenderLayer::BuildPreview;
		child->AddComponent<Constant>()->v4 = { 0.0f,1.0f,0.0f,1.0f };
	}

	curPrefabType = type;
	curPrefabIndex = index;

	Scene::scene->spatialPartitioningManager.tagData.SetTagCollision(TAG_BUILDING, TAG_PREVIEW, true);
}

float BuildingBuilder::getBoundingBox(int type, int index)
{
	if (index < building[type].size())
	{
		auto data = building[type][index];

		XMFLOAT3 bound;

		if (data.prefab)
			bound = data.prefab->GetComponent<BoxCollider>()->boundingBox.Extents;
		else
			bound = data.mesh->Bounds.Extents;

		return bound.x > bound.y ? bound.x : bound.y;
	}
	return 0;
}

int BuildingBuilder::getBuildingCount(int type)
{
	return building[type].size();
}

wstring BuildingBuilder::getBuildingName(int type, int index)
{
	if (index < building[type].size())
		return building[type][index].buildingName;
	return L"X";
}

void BuildingBuilder::pickToDelete()
{
	Vector3 mousePosInWorld = getPosOnTerrain();

	int x = mousePosInWorld.x / gameObject->scene->spatialPartitioningManager.sectorWidth;
	int y = mousePosInWorld.z / gameObject->scene->spatialPartitioningManager.sectorHeight;


	for (auto& tagList : gameObject->scene->spatialPartitioningManager.sectorList[x][y].list)
	{
		for (auto& object : tagList.second)
		{
			BoxCollider* collider = object->GetComponent<BoxCollider>();
			if (collider)
			{
				BoundingOrientedBox boundingBox{};
				boundingBox.Center = object->transform->position.xmf3;
				boundingBox.Extents = collider->boundingBox.Extents;
				boundingBox.Orientation = object->transform->localToWorldMatrix.QuaternionRotationMatrix().xmf4;

				if (boundingBox.Contains(XMLoadFloat3(&XMFLOAT3(mousePosInWorld.x, mousePosInWorld.y, mousePosInWorld.z))))
				{
					if (Input::GetMouseButtonUp(0))
					{
						// 랜드마크인 경우 속해있는 모든 객체 다 삭제
						if (object->GetComponent<Building>()->type == BuildingType::Landmark)
						{
							for (auto& list : GameWorld::gameWorld->buildingList[object])
							{
								for (auto& go : list.second)
								{
									go->scene->PushDelete(go);
									updateTerrainNodeData(go, false);
									Building* building = go->GetComponent<Building>();
									GameWorld::gameWorld->deleteInGameWorld(building->landmark, go, building->type, building->index);
								}
							}
						}
						gameObject->scene->PushDelete(object);
						updateTerrainNodeData(object, false);
						Building* building = object->GetComponent<Building>();
						GameWorld::gameWorld->deleteInGameWorld(building->landmark, object, building->type, building->index);
					}
				}
			}
		}
	}
}

Vector3 BuildingBuilder::getPosOnTerrain()
{
	Vector3 screenPos{ Input::mousePosition.x, Input::mousePosition.y, 1.0f };

	Vector3 rayOrigin{ 0.0f, 0.0f, 0.0f };
	Vector3 rayDir = Camera::main->ScreenToWorldPoint(screenPos);

	Matrix4x4 invView = Camera::main->view.Inverse();
	Matrix4x4 invWorld = terrain->gameObject->transform->localToWorldMatrix.Inverse();
	Matrix4x4 toLocal = invView * invWorld;

	rayOrigin = rayOrigin.TransformCoord(toLocal);
	rayDir = rayDir.TransformNormal(invWorld).Normalize();

	if (IntersectPlane(rayOrigin, rayDir, Vector3{ 0,0,0 }, Vector3{ 1,0,0 }, Vector3{ 0,0,1 }))
	{
		Vector3 point;

		std::vector<XMFLOAT3> vertices;
		point = rayOrigin + rayDir * distance;

		auto terrainMesh = terrain->terrainData.heightmapTexture;
		for (float i = 1; i < distance + 1; i += 0.5)
		{
			Vector3 p = rayOrigin + rayDir * i;
			if ((int)point.x == (int)p.x && (int)point.z == (int)p.z)
				continue;

			point = p;
			float x, y, z;
			if (std::ceil(point.x) > terrain->terrainData.GetHeightMapWidth() - 1 || std::ceil(point.z) > terrain->terrainData.GetHeightMapLength() - 1)
				continue;

			x = std::floor(point.x);
			z = std::floor(point.z);
			vertices.push_back({ x, terrainMesh->OnGetHeight(x, z, &terrain->terrainData), z });
			vertices.push_back({ x + 1, terrainMesh->OnGetHeight(x + 1, z + 1, &terrain->terrainData), z + 1 });
			vertices.push_back({ x + 1, terrainMesh->OnGetHeight(x + 1, z, &terrain->terrainData), z });

			vertices.push_back({ x, terrainMesh->OnGetHeight(x, z, &terrain->terrainData), z });
			vertices.push_back({ x, terrainMesh->OnGetHeight(x, z + 1, &terrain->terrainData), z + 1 });
			vertices.push_back({ x + 1, terrainMesh->OnGetHeight(x + 1, z + 1, &terrain->terrainData), z + 1 });

		}
		IntersectVertices(rayOrigin.xmf3, rayDir.xmf3, vertices);

		point = rayOrigin + rayDir * distance;
		point = point.TransformCoord(terrain->gameObject->transform->localToWorldMatrix);

		return point;
	}
	return { 0 };
}

bool BuildingBuilder::IntersectPlane(Vector3 rayOrigin, Vector3 rayDirection, Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 edge1{ v1.x - v0.x,v1.y - v0.y,v1.z - v0.z };
	Vector3 edge2{ v2.x - v0.x,v2.y - v0.y, v2.z - v0.z };

	Vector3 pvec = Vector3::CrossProduct(rayDirection, edge2);

	float dot = Vector3::DotProduct(edge1, pvec);

	if (dot > 0.0001f)
		return false;

	Vector3 planeNormal = Vector3::CrossProduct(edge1, edge2);

	float dot1 = Vector3::DotProduct(planeNormal, v0);
	float dot2 = Vector3::DotProduct(planeNormal, rayOrigin);
	float dot3 = Vector3::DotProduct(planeNormal, rayDirection);

	distance = (dot1 - dot2) / dot3;

	return true;
}

void BuildingBuilder::IntersectVertices(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, std::vector<XMFLOAT3>& vertices)
{
	UINT triCount = vertices.size() / 3;

	float tmin = MathHelper::Infinity;
	for (UINT i = 0; i < triCount; ++i)
	{
		XMVECTOR v0 = XMLoadFloat3(&vertices[i * 3 + 0]);
		XMVECTOR v1 = XMLoadFloat3(&vertices[i * 3 + 1]);
		XMVECTOR v2 = XMLoadFloat3(&vertices[i * 3 + 2]);

		float x = (vertices[i * 3 + 0].x + vertices[i * 3 + 1].x + vertices[i * 3 + 2].x) / 3;
		float y = (vertices[i * 3 + 0].y + vertices[i * 3 + 1].y + vertices[i * 3 + 2].y) / 3;
		float z = (vertices[i * 3 + 0].z + vertices[i * 3 + 1].z + vertices[i * 3 + 2].z) / 3;
		XMMATRIX toLocal = XMMatrixTranslation(-x, -y, -z);
		XMMATRIX InvToLocal = XMMatrixInverse(&XMMatrixDeterminant(toLocal), toLocal);
		XMMATRIX s = XMMatrixScaling(1.2, 1.2, 1.2);

		v0 = XMVector3Transform(XMVector3Transform(XMVector3Transform(v0, toLocal), s), InvToLocal);
		v1 = XMVector3Transform(XMVector3Transform(XMVector3Transform(v1, toLocal), s), InvToLocal);
		v2 = XMVector3Transform(XMVector3Transform(XMVector3Transform(v2, toLocal), s), InvToLocal);

		float t = 0.0f;
		if (TriangleTests::Intersects(XMLoadFloat3(&rayOrigin), XMLoadFloat3(&rayDirection), v0, v1, v2, t))
		{
			if (t < tmin)
			{
				tmin = t;
				distance = tmin;
			}
		}
	}
}
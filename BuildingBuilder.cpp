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
			makePrefab(curPrefabType, curPrefabIndex);
			prefab->transform->localToWorldMatrix = prePrefab->GetMatrix();

			auto p = prefab->transform->position;
			int range = 0;
			if (curPrefabType == Landmark)
			{
				curLandmark = prefab;
				prefab->AddComponent<Village>()->OnAutoDevelopment();
				range = prefab->GetComponent<Village>()->radiusOfLand;
			}
			else {
				for (auto& landmark : HostGameWorld::gameWorld->buildingList) {
					float dist = sqrt(pow(p.x - landmark.first->transform->position.x, 2) + pow(p.z - landmark.first->transform->position.z, 2));
					if (landmark.first->GetComponent<Village>()->radiusOfLand >= dist) {
						curLandmark = landmark.first;
						break;
					}
				}
			}
			prefab->AddComponent<Building>()->setBuildingInform(curLandmark, curPrefabType, curPrefabIndex);
			prefab->tag = TAG_BUILDING;

			Vector3 building_forward = prefab->transform->forward;
			building_forward.y = 0;
			building_forward.Normalize();
			Vector3 forward = { 0,0,1 };
			double angle = Vector3::DotProduct(forward, building_forward);
			Vector3 dir = Vector3::CrossProduct(forward, building_forward);
			angle = XMConvertToDegrees(acos(angle));
			angle *= (dir.y > 0.0f) ? 1.0f : -1.0f;

			GameLoader::gameLoader->insertInFile(curPrefabType, curPrefabIndex, p.x, p.z, angle, range);
			if (HostNetwork::network->isConnect) {
				
				HostNetwork::network->send_construct_packet(curPrefabType, curPrefabIndex, p.x, p.z, angle, range);
			}
			updateTerrainNodeData(prefab, true);

			HostGameWorld::gameWorld->buildInGameWorld(curLandmark, prefab, curPrefabType, curPrefabIndex);

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

		// 미리보기 건물 이동
		else
			prefab->transform->position = getPosOnTerrain();
	}

	// 건물 삭제
	else if (Input::GetKey(KeyCode::X) && HostNetwork::network != nullptr) 
		pickToDelete();
	
}

void BuildingBuilder::serializeBuildings()
{
	/*AdvancedVillagePack*/
	{
		building[Landmark].push_back(makeBuilderDataAsPrefab(L"AV_Well", ASSET PREFAB("AV_Well")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_House01", ASSET MESH("AV_House_01"), ASSET MATERIAL("AV_house_01")));
		building[House].push_back(makeBuilderDataAsPrefab(L"AV_House02", ASSET PREFAB("AV_House_01")));
		//building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_01", ASSET MESH("AV_Tree_01"), { ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
		//building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_02", ASSET MESH("AV_Tree_02"), { ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
		//building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_03", ASSET MESH("AV_Tree_03"), { ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
		//building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_04", ASSET MESH("AV_Tree_04"), { ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
		//building[Landscape].push_back(makeBuilderDataAsMeshAndMaterials(L"Tree_05", ASSET MESH("AV_Tree_05"), { ASSET MATERIAL("TreeLeafs"), ASSET MATERIAL("TreeTrunks") }));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Mushroom_01", ASSET MESH("AV_Mushroom_01"), ASSET MATERIAL("AV_material_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Mushroom_02", ASSET MESH("AV_Mushroom_02"), ASSET MATERIAL("AV_material_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Mushroom_03", ASSET MESH("AV_Mushroom_03"), ASSET MATERIAL("AV_material_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Mushroom_04", ASSET MESH("AV_Mushroom_04"), ASSET MATERIAL("AV_material_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Mushroom_05", ASSET MESH("AV_Mushroom_05"), ASSET MATERIAL("AV_material_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Mushroom_06", ASSET MESH("AV_Mushroom_06"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Fence_01", ASSET MESH("AV_Fence_01"), ASSET MATERIAL("AV_material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Fence_02", ASSET MESH("AV_Fence_02"), ASSET MATERIAL("AV_material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Fence_03", ASSET MESH("AV_Fence_03"), ASSET MATERIAL("AV_material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Fence_04", ASSET MESH("AV_Fence_04"), ASSET MATERIAL("AV_material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Bucket", ASSET MESH("AV_Bucket"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Barrel", ASSET MESH("AV_Barrel"), ASSET MATERIAL("AV_material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Spike", ASSET MESH("AV_Spike"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Cart_01", ASSET MESH("AV_Cart_01"), ASSET MATERIAL("AV_material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Cart_02", ASSET MESH("AV_Cart_02"), ASSET MATERIAL("AV_material_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Cauldron", ASSET MESH("AV_Cauldron"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Pot_01", ASSET MESH("AV_Pot_01"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Pot_02", ASSET MESH("AV_Pot_02"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Pot_03", ASSET MESH("AV_Pot_03"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Pot_04", ASSET MESH("AV_Pot_04"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Pot_05", ASSET MESH("AV_Pot_05"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Pot_06", ASSET MESH("AV_Pot_06"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Apple", ASSET MESH("AV_Apple"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Potato", ASSET MESH("AV_Potato"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Tomato", ASSET MESH("AV_Tomato"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Watermellon", ASSET MESH("AV_Watermellon"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Sack_Apple", ASSET MESH("AV_Sack_Apple"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Sack_Flour", ASSET MESH("AV_Sack_Flour"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Sack_Potato", ASSET MESH("AV_Sack_Potato"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Sack_Tomato", ASSET MESH("AV_Sack_Tomato"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Sack_01", ASSET MESH("AV_Sack_01"), ASSET MATERIAL("AV_material_02")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"AV_Sack_02", ASSET MESH("AV_Sack_02"), ASSET MATERIAL("AV_material_02")));
	}
	


	/*HandPaintedEnviroment*/

	{	
		building[Landmark].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Well", ASSET MESH("HP_Well"), ASSET MATERIAL("HP_Well")));
		building[Landmark].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Town_Hall", ASSET MESH("HP_Town_Hall"), ASSET MATERIAL("HP_Town_Hall")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Baker_house", ASSET MESH("HP_Baker_house"), ASSET MATERIAL("HP_Baker_house")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Large_house", ASSET MESH("HP_Large_house"), ASSET MATERIAL("HP_Large_house")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Small_house", ASSET MESH("HP_Small_house"), ASSET MATERIAL("HP_Small_house")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Witch_house", ASSET MESH("HP_Witch_house"), ASSET MATERIAL("HP_Witch_house")));
		building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Mine", ASSET MESH("HP_Mine"), ASSET MATERIAL("HP_Mine")));
		building[Theme].push_back(makeBuilderDataAsPrefab(L"HP_Mill", ASSET PREFAB("HP_Mill")));
		building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Woodmill", ASSET MESH("HP_Woodmill"), ASSET MATERIAL("HP_Woodmill")));
		building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tavern", ASSET MESH("HP_Tavern"), ASSET MATERIAL("HP_Tavern")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tree_01", ASSET MESH("HP_Tree_01"), ASSET MATERIAL("HP_Tree_01")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tree_02", ASSET MESH("HP_Tree_02"), ASSET MATERIAL("HP_Tree_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tree_03", ASSET MESH("HP_Tree_03"), ASSET MATERIAL("HP_Tree_03")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tree_04", ASSET MESH("HP_Tree_04"), ASSET MATERIAL("HP_Tree_04")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Bush_1", ASSET MESH("HP_Bush_1"), ASSET MATERIAL("HP_Plants")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Bush_2", ASSET MESH("HP_Bush_2"), ASSET MATERIAL("HP_Plants")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Flower", ASSET MESH("HP_Flower"), ASSET MATERIAL("HP_Plants")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Flowers_1", ASSET MESH("HP_Flowers_1"), ASSET MATERIAL("HP_Plants")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Flowers_2", ASSET MESH("HP_Flowers_2"), ASSET MATERIAL("HP_Plants")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Mushroom_1", ASSET MESH("HP_Mushroom_1"), ASSET MATERIAL("HP_Log_stump_mushrooms")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Mushroom_2", ASSET MESH("HP_Mushroom_2"), ASSET MATERIAL("HP_Log_stump_mushrooms")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Mushroom_3", ASSET MESH("HP_Mushroom_3"), ASSET MATERIAL("HP_Log_stump_mushrooms")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Mushroom_4", ASSET MESH("HP_Mushroom_4"), ASSET MATERIAL("HP_Log_stump_mushrooms")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Pine_tree_01", ASSET MESH("HP_Pine_tree_01"), ASSET MATERIAL("HP_Pine_tree_01")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Pine_tree_02", ASSET MESH("HP_Pine_tree_02"), ASSET MATERIAL("HP_Pine_tree_02")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Plant", ASSET MESH("HP_Plant"), ASSET MATERIAL("HP_Plant")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Rock_1", ASSET MESH("HP_Rock_1"), ASSET MATERIAL("HP_Rocks_1_2")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Rock_2", ASSET MESH("HP_Rock_2"), ASSET MATERIAL("HP_Rocks_1_2")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Rock_3", ASSET MESH("HP_Rock_3"), ASSET MATERIAL("HP_Rocks_3_4")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Rock_4", ASSET MESH("HP_Rock_4"), ASSET MATERIAL("HP_Rocks_3_4")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Stump", ASSET MESH("HP_Stump"), ASSET MATERIAL("HP_Log_stump_mushrooms")));
		building[Lighting].push_back(makeBuilderDataAsPrefab(L"HP_Street_light", ASSET PREFAB("HP_Street_light")));
		building[Lighting].push_back(makeBuilderDataAsPrefab(L"HP_Lamppost", ASSET PREFAB("HP_Lamppost")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Altar", ASSET MESH("HP_Altar"), ASSET MATERIAL("HP_Lampposts")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Anvil", ASSET MESH("HP_Anvil"), ASSET MATERIAL("HP_Hammer_anvil")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Barrel", ASSET MESH("HP_Barrel"), ASSET MATERIAL("HP_Barrel")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Box_1", ASSET MESH("HP_Box_1"), ASSET MATERIAL("HP_Boxes")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Box_2", ASSET MESH("HP_Box_2"), ASSET MATERIAL("HP_Boxes")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Box_3", ASSET MESH("HP_Box_3"), ASSET MATERIAL("HP_Boxes")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Box_4", ASSET MESH("HP_Box_4"), ASSET MATERIAL("HP_Boxes")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Cauldron", ASSET MESH("HP_Cauldron"), ASSET MATERIAL("HP_Cauldron")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Crystal_1", ASSET MESH("HP_Crystal_1"), ASSET MATERIAL("HP_Crystals")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Crystal_2", ASSET MESH("HP_Crystal_2"), ASSET MATERIAL("HP_Crystals")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Crystal_3", ASSET MESH("HP_Crystal_3"), ASSET MATERIAL("HP_Crystals")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Crystal_4", ASSET MESH("HP_Crystal_4"), ASSET MATERIAL("HP_Crystals")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Crystal_5", ASSET MESH("HP_Crystal_5"), ASSET MATERIAL("HP_Crystals")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Fence", ASSET MESH("HP_Fence"), ASSET MATERIAL("HP_Fence")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Fence_1", ASSET MESH("HP_Fence_1"), ASSET MATERIAL("HP_Fence")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Fence_2", ASSET MESH("HP_Fence_2"), ASSET MATERIAL("HP_Fence")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Fern", ASSET MESH("HP_Fern"), ASSET MATERIAL("HP_Fern")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Forge", ASSET MESH("HP_Forge"), ASSET MATERIAL("HP_Forge")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Hammer", ASSET MESH("HP_Hammer"), ASSET MATERIAL("HP_Hammer_anvil")));	
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Log", ASSET MESH("HP_Log"), ASSET MATERIAL("HP_Log_stump_mushrooms")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Log_1", ASSET MESH("HP_Log_1"), ASSET MATERIAL("HP_Logs")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Log_2", ASSET MESH("HP_Log_2"), ASSET MATERIAL("HP_Logs")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Log_3", ASSET MESH("HP_Log_3"), ASSET MATERIAL("HP_Logs")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Pillar", ASSET MESH("HP_Pillar"), ASSET MATERIAL("HP_Pillars_arch")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Pillar_ruined_1", ASSET MESH("HP_Pillar_ruined_1"), ASSET MATERIAL("HP_Pillars_arch")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Stone_fence", ASSET MESH("HP_Stone_fence"), ASSET MATERIAL("HP_Stone_fence")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tile_1", ASSET MESH("HP_Tile_1"), ASSET MATERIAL("HP_Stone_tiles")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tile_2", ASSET MESH("HP_Tile_2"), ASSET MATERIAL("HP_Stone_tiles")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tile_3", ASSET MESH("HP_Tile_3"), ASSET MATERIAL("HP_Stone_tiles")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tile_4", ASSET MESH("HP_Tile_4"), ASSET MATERIAL("HP_Stone_tiles")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tile_5", ASSET MESH("HP_Tile_5"), ASSET MATERIAL("HP_Stone_tiles")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tile_6", ASSET MESH("HP_Tile_6"), ASSET MATERIAL("HP_Stone_tiles")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tile_7", ASSET MESH("HP_Tile_7"), ASSET MATERIAL("HP_Stone_tiles")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Tower", ASSET MESH("HP_Tower"), ASSET MATERIAL("HP_Ruined_tower")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Wall_1", ASSET MESH("HP_Wall_1"), ASSET MATERIAL("HP_Ruined_walls")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"HP_Wall_2", ASSET MESH("HP_Wall_2"), ASSET MATERIAL("HP_Ruined_walls")));
	}
	

	/*SpringEnviromnet*/
	{	
		building[Landmark].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_TownHall_SM", ASSET MESH("SE_TownHall_SM"), ASSET MATERIAL("SE_TownHall")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_01a_SM", ASSET MESH("SE_House_01a_SM"), ASSET MATERIAL("SE_House_01a")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_01b_SM", ASSET MESH("SE_House_01b_SM"), ASSET MATERIAL("SE_House_01b")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_01c_SM", ASSET MESH("SE_House_01c_SM"), ASSET MATERIAL("SE_House_01c")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_01d_SM", ASSET MESH("SE_House_01d_SM"), ASSET MATERIAL("SE_House_01d")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_02a_SM", ASSET MESH("SE_House_02a_SM"), ASSET MATERIAL("SE_House_02a")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_02b_SM", ASSET MESH("SE_House_02b_SM"), ASSET MATERIAL("SE_House_02b")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_02c_SM", ASSET MESH("SE_House_02c_SM"), ASSET MATERIAL("SE_House_02c")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_02d_SM", ASSET MESH("SE_House_02d_SM"), ASSET MATERIAL("SE_House_02d")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_03d_SM", ASSET MESH("SE_House_03d_SM"), ASSET MATERIAL("SE_House_03d")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_03a_SM", ASSET MESH("SE_House_03a_SM"), ASSET MATERIAL("SE_House_03a")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_03b_SM", ASSET MESH("SE_House_03b_SM"), ASSET MATERIAL("SE_House_03b")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_03c_SM", ASSET MESH("SE_House_03c_SM"), ASSET MATERIAL("SE_House_03c")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_04a_SM", ASSET MESH("SE_House_04a_SM"), ASSET MATERIAL("SE_House_04a")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_04b_SM", ASSET MESH("SE_House_04b_SM"), ASSET MATERIAL("SE_House_04b")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_05a_SM", ASSET MESH("SE_House_05a_SM"), ASSET MATERIAL("SE_House_05a")));
		building[House].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_House_05b_SM", ASSET MESH("SE_House_05b_SM"), ASSET MATERIAL("SE_House_05b")));
		building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_CandyHouse_01_SM", ASSET MESH("SE_CandyHouse_01_SM"), ASSET MATERIAL("SE_Candyhouse")));
		building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_FabricHouse_01_SM", ASSET MESH("SE_FabricHouse_01_SM"), ASSET MATERIAL("SE_FabricHouse")));
		building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Factory_01_SM", ASSET MESH("SE_Factory_01_SM"), ASSET MATERIAL("SE_Factory")));
		building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_HeartHouse_01_SM", ASSET MESH("SE_HeartHouse_01_SM"), ASSET MATERIAL("SE_HeartHouse")));
		building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Gazebo_01_SM", ASSET MESH("SE_Gazebo_01_SM"), ASSET MATERIAL("SE_Gazebo")));
		building[Theme].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Theater_01_SM", ASSET MESH("SE_Theater_01_SM"), ASSET MATERIAL("SE_Theater")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Blossom_01_SM", ASSET MESH("SE_Blossom_01_SM"), ASSET MATERIAL("SE_Bushese")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Blossom_02_SM", ASSET MESH("SE_Blossom_02_SM"), ASSET MATERIAL("SE_Bushese")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_CottonCandy_01_SM", ASSET MESH("SE_CottonCandy_01_SM"), ASSET MATERIAL("SE_CottonCandyTree")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_CottonCandyTree_01_SM", ASSET MESH("SE_CottonCandyTree_01_SM"), ASSET MATERIAL("SE_CottonCandyTree")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_CottonCandyTree_02_SM", ASSET MESH("SE_CottonCandyTree_02_SM"), ASSET MATERIAL("SE_CottonCandyTree")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_CottonCandyTree_03_SM", ASSET MESH("SE_CottonCandyTree_03_SM"), ASSET MATERIAL("SE_CottonCandyTree")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_01_SM", ASSET MESH("SE_Flower_01_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_03_SM", ASSET MESH("SE_Flower_03_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_04_SM", ASSET MESH("SE_Flower_04_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_06_SM", ASSET MESH("SE_Flower_06_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_07_SM", ASSET MESH("SE_Flower_07_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_08_SM", ASSET MESH("SE_Flower_08_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_10_SM", ASSET MESH("SE_Flower_10_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_11_SM", ASSET MESH("SE_Flower_11_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_13_SM", ASSET MESH("SE_Flower_13_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_14_SM", ASSET MESH("SE_Flower_14_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_16_SM", ASSET MESH("SE_Flower_16_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_40_SM", ASSET MESH("SE_Flower_40_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_41_SM", ASSET MESH("SE_Flower_41_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_42_SM", ASSET MESH("SE_Flower_42_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_43_SM", ASSET MESH("SE_Flower_43_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_44_SM", ASSET MESH("SE_Flower_44_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_45_SM", ASSET MESH("SE_Flower_45_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_46_SM", ASSET MESH("SE_Flower_46_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_47_SM", ASSET MESH("SE_Flower_47_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_48_SM", ASSET MESH("SE_Flower_48_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_49_SM", ASSET MESH("SE_Flower_49_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_50_SM", ASSET MESH("SE_Flower_50_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_51_SM", ASSET MESH("SE_Flower_51_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_52_SM", ASSET MESH("SE_Flower_52_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_71_SM", ASSET MESH("SE_Flower_71_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_72_SM", ASSET MESH("SE_Flower_72_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_73_SM", ASSET MESH("SE_Flower_73_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_74_SM", ASSET MESH("SE_Flower_74_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_75_SM", ASSET MESH("SE_Flower_75_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Flower_76_SM", ASSET MESH("SE_Flower_76_SM"), ASSET MATERIAL("SE_Flowers")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_01_SM", ASSET MESH("SE_Rose_01_SM"), ASSET MATERIAL("SE_Roses")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_02_SM", ASSET MESH("SE_Rose_02_SM"), ASSET MATERIAL("SE_Roses")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_03_SM", ASSET MESH("SE_Rose_03_SM"), ASSET MATERIAL("SE_Roses")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_04_SM", ASSET MESH("SE_Rose_04_SM"), ASSET MATERIAL("SE_Roses")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_05_SM", ASSET MESH("SE_Rose_05_SM"), ASSET MATERIAL("SE_Roses")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Rose_06_SM", ASSET MESH("SE_Rose_06_SM"), ASSET MATERIAL("SE_Roses")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_01_SM", ASSET MESH("SE_Stone_01_SM"), ASSET MATERIAL("SE_Stones")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_02_SM", ASSET MESH("SE_Stone_02_SM"), ASSET MATERIAL("SE_Stones")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_03_SM", ASSET MESH("SE_Stone_03_SM"), ASSET MATERIAL("SE_Stones")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_04_SM", ASSET MESH("SE_Stone_04_SM"), ASSET MATERIAL("SE_Stones")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_05_SM", ASSET MESH("SE_Stone_05_SM"), ASSET MATERIAL("SE_Stones")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Stone_06_SM", ASSET MESH("SE_Stone_06_SM"), ASSET MATERIAL("SE_Stones")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Willow_01a_SM", ASSET MESH("SE_Willow_01a_SM"), ASSET MATERIAL("SE_Willow")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Willow_01b_SM", ASSET MESH("SE_Willow_01b_SM"), ASSET MATERIAL("SE_Willow")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Baloon_01_SM", ASSET MESH("SE_Baloon_01_SM"), ASSET MATERIAL("SE_Baloons")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Baloon_02_SM", ASSET MESH("SE_Baloon_02_SM"), ASSET MATERIAL("SE_Baloons")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Baloon_03_SM", ASSET MESH("SE_Baloon_03_SM"), ASSET MATERIAL("SE_Baloons")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Baloon_04_SM", ASSET MESH("SE_Baloon_04_SM"), ASSET MATERIAL("SE_Baloons")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Baloons_01_SM", ASSET MESH("SE_Baloons_01_SM"), ASSET MATERIAL("SE_Baloons")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bench_01a_SM", ASSET MESH("SE_Bench_01a_SM"), ASSET MATERIAL("SE_Bench_01a")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bench_01b_SM", ASSET MESH("SE_Bench_01b_SM"), ASSET MATERIAL("SE_Bench_01b")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_01_SM", ASSET MESH("SE_Birch_01_SM"), ASSET MATERIAL("SE_Birch")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_01b_SM", ASSET MESH("SE_Birch_01b_SM"), ASSET MATERIAL("SE_Birch_01b")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_02_SM", ASSET MESH("SE_Birch_02_SM"), ASSET MATERIAL("SE_Birch")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_02b_SM", ASSET MESH("SE_Birch_02b_SM"), ASSET MATERIAL("SE_Birch_01b")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_03_SM", ASSET MESH("SE_Birch_03_SM"), ASSET MATERIAL("SE_Birch")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Birch_03b_SM", ASSET MESH("SE_Birch_03b_SM"), ASSET MATERIAL("SE_Birch_01b")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Blanket_01_SM", ASSET MESH("SE_Blanket_01_SM"), ASSET MATERIAL("SE_Blanket")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Blanket_02_SM", ASSET MESH("SE_Blanket_02_SM"), ASSET MATERIAL("SE_Blanket")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bonsai_01_SM", ASSET MESH("SE_Bonsai_01_SM"), ASSET MATERIAL("SE_Bonsai")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bonsai_02_SM", ASSET MESH("SE_Bonsai_02_SM"), ASSET MATERIAL("SE_Bonsai")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Booth_01_SM", ASSET MESH("SE_Booth_01_SM"), ASSET MATERIAL("SE_Booth")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Booth_02_SM", ASSET MESH("SE_Booth_02_SM"), ASSET MATERIAL("SE_Booth")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Branch_01_SM", ASSET MESH("SE_Branch_01_SM"), ASSET MATERIAL("SE_Bushese")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Branch_02_SM", ASSET MESH("SE_Branch_02_SM"), ASSET MATERIAL("SE_Bushese")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bush_06_SM", ASSET MESH("SE_Bush_06_SM"), ASSET MATERIAL("SE_Bonsai")));
		building[Landscape].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Bush_07_SM", ASSET MESH("SE_Bush_07_SM"), ASSET MATERIAL("SE_Bonsai")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Candy_01_SM", ASSET MESH("SE_Candy_01_SM"), ASSET MATERIAL("SE_Candies")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Candy_02_SM", ASSET MESH("SE_Candy_02_SM"), ASSET MATERIAL("SE_Candies")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Candy_03_SM", ASSET MESH("SE_Candy_03_SM"), ASSET MATERIAL("SE_Candies")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Candy_04_SM", ASSET MESH("SE_Candy_04_SM"), ASSET MATERIAL("SE_Candies")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Carousel_01_SM", ASSET MESH("SE_Carousel_01_SM"), ASSET MATERIAL("SE_Carousel")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Cart_01_SM", ASSET MESH("SE_Cart_01_SM"), ASSET MATERIAL("SE_Cart")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Chair_01a_SM", ASSET MESH("SE_Chair_01a_SM"), ASSET MATERIAL("SE_Bench_01a")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Climber_01_SM", ASSET MESH("SE_Climber_01_SM"), ASSET MATERIAL("SE_Climbers")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Climber_02_SM", ASSET MESH("SE_Climber_02_SM"), ASSET MATERIAL("SE_Climbers")));	
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Fence_07a_SM", ASSET MESH("SE_Fence_07a_SM"), ASSET MATERIAL("SE_Fence_01a")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Fence_07b_SM", ASSET MESH("SE_Fence_07b_SM"), ASSET MATERIAL("SE_Fence_01b")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Fence_08a_SM", ASSET MESH("SE_Fence_08a_SM"), ASSET MATERIAL("SE_Fence_01a")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Fence_08b_SM", ASSET MESH("SE_Fence_08b_SM"), ASSET MATERIAL("SE_Fence_01b")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Gate_01b_SM", ASSET MESH("SE_Gate_01b_SM"), ASSET MATERIAL("SE_Gate")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Grill_02_SM", ASSET MESH("SE_Grill_02_SM"), ASSET MATERIAL("SE_Grill")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_01_SM", ASSET MESH("SE_Hedge_01_SM"), ASSET MATERIAL("SE_Hedges")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_02_SM", ASSET MESH("SE_Hedge_02_SM"), ASSET MATERIAL("SE_Hedges")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_03_SM", ASSET MESH("SE_Hedge_03_SM"), ASSET MATERIAL("SE_Hedges")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_04_SM", ASSET MESH("SE_Hedge_04_SM"), ASSET MATERIAL("SE_Hedges")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_05_SM", ASSET MESH("SE_Hedge_05_SM"), ASSET MATERIAL("SE_Hedges")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_06_SM", ASSET MESH("SE_Hedge_06_SM"), ASSET MATERIAL("SE_Hedges")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_07_SM", ASSET MESH("SE_Hedge_07_SM"), ASSET MATERIAL("SE_Hedges")));
		building[Fence].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Hedge_08_SM", ASSET MESH("SE_Hedge_08_SM"), ASSET MATERIAL("SE_Hedges")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_HotAirBaloon_SM", ASSET MESH("SE_HotAirBaloon_SM"), ASSET MATERIAL("SE_HotAirBaloon")));
		building[Lighting].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Lamp_01_SM", ASSET MESH("SE_Lamp_01_SM"), ASSET MATERIAL("SE_Lamps")));
		building[Lighting].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Lamp_02_SM", ASSET MESH("SE_Lamp_02_SM"), ASSET MATERIAL("SE_Lamps")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Maypole_01_SM", ASSET MESH("SE_Maypole_01_SM"), ASSET MATERIAL("SE_Maypole_01")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_01_SM", ASSET MESH("SE_Present_01_SM"), ASSET MATERIAL("SE_Presents")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_02_SM", ASSET MESH("SE_Present_02_SM"), ASSET MATERIAL("SE_Presents")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_03_SM", ASSET MESH("SE_Present_03_SM"), ASSET MATERIAL("SE_Presents")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_04_SM", ASSET MESH("SE_Present_04_SM"), ASSET MATERIAL("SE_Presents")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_05_SM", ASSET MESH("SE_Present_05_SM"), ASSET MATERIAL("SE_Presents")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Present_06_SM", ASSET MESH("SE_Present_06_SM"), ASSET MATERIAL("SE_Presents")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Pump_01_SM", ASSET MESH("SE_Pump_01_SM"), ASSET MATERIAL("SE_Pump")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_SeeSaw_01_SM", ASSET MESH("SE_SeeSaw_01_SM"), ASSET MATERIAL("SE_Seesaw")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Sign_01_SM", ASSET MESH("SE_Sign_01_SM"), ASSET MATERIAL("SE_Sign")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Slide_01_SM", ASSET MESH("SE_Slide_01_SM"), ASSET MATERIAL("SE_Slide")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Swing_01_SM", ASSET MESH("SE_Swing_01_SM"), ASSET MATERIAL("SE_Swing")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Swing_02_SM", ASSET MESH("SE_Swing_02_SM"), ASSET MATERIAL("SE_Swing")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Table_01a_SM", ASSET MESH("SE_Table_01a_SM"), ASSET MATERIAL("SE_Tables")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Table_01b_SM", ASSET MESH("SE_Table_01b_SM"), ASSET MATERIAL("SE_Tables")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Table_02a_SM", ASSET MESH("SE_Table_02a_SM"), ASSET MATERIAL("SE_Tables")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Table_02b_SM", ASSET MESH("SE_Table_02b_SM"), ASSET MATERIAL("SE_Tables")));
		building[Prop].push_back(makeBuilderDataAsMeshAndMaterial(L"SE_Trampoline_01_SM", ASSET MESH("SE_Trampoline_01_SM"), ASSET MATERIAL("SE_Trampoline")));
	}

	ofstream out("colliders.txt");
	for (int i = 0; i < BuildingType::Count; ++i) {
		int j = 0;
		for (auto b : building[i]) {
			if (b.prefab) {
				auto box = b.prefab->GetComponent<BoxCollider>();
				out << i << " " << j << " " << box->center.x - box->extents.x << " " << box->center.y - box->extents.y << " " << box->center.x + box->extents.x << " " << box->center.y + box->extents.y << endl;
			}
			else {
				auto box = b.mesh->Bounds;
				out << i << " " << j << " " << box.Center.x - box.Extents.x << " " << box.Center.x - box.Extents.y << " " << box.Center.x + box.Extents.x << " " << box.Center.x + box.Extents.y << endl;
			}
			++j;
		}
	}
	out.close();
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
	if (curPrefabType == Landmark)
	{
		if (HostGameWorld::gameWorld->buildingList.empty())
		{
			curLandmark = prefab;
			return prefab;
		}
		for (auto& landmark : HostGameWorld::gameWorld->buildingList)
		{
			float distance = sqrt(pow(prefab->transform->position.x - landmark.first->transform->position.x, 2) + pow(prefab->transform->position.z - landmark.first->transform->position.z, 2));

			if (distance >= landmark.first->GetComponent<Village>()->radiusOfLand + LAND_SMALL)
			{
				curLandmark = prefab;
				return prefab;
			}
		}
	}
	else
	{
		for (auto& landmark : HostGameWorld::gameWorld->buildingList)
		{
			float distance = sqrt(pow(prefab->transform->position.x - landmark.first->transform->position.x, 2) + pow(prefab->transform->position.z - landmark.first->transform->position.z, 2));

			if (distance <= landmark.first->GetComponent<Village>()->radiusOfLand)
			{
				curLandmark = landmark.first;
				return landmark.first;
			}
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

void BuildingBuilder::build(Vector2 position, double angle, int type, int index, GameObject* landmark)
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
		obj->AddComponent<Building>()->setBuildingInform(landmark, type, index);
		obj->tag = TAG_BUILDING;

		GameLoader::gameLoader->insertInFile(type, index, pos.x, pos.z, angle, 0);
		updateTerrainNodeData(obj, true);
		HostGameWorld::gameWorld->buildInGameWorld(landmark, obj, type, index);

	}
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
			prefab->AddComponent<BoxCollider>()->boundingBox.Center = { data.mesh->Bounds.Center.x, data.mesh->Bounds.Center.z, data.mesh->Bounds.Center.y };
			prefab->GetComponent<BoxCollider>()->boundingBox.Extents = { data.mesh->Bounds.Extents.x, data.mesh->Bounds.Extents.z, data.mesh->Bounds.Extents.y };

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
		prefab->GetComponent<BoxCollider>()->obb = true;
	}
}

void BuildingBuilder::exitBuildMode()
{
	Scene::scene->PushDelete(prefab);
	Scene::scene->spatialPartitioningManager.tagData.SetTagCollision(TAG_BUILDING, TAG_PREVIEW, false);
	prefab = nullptr;
}

void BuildingBuilder::enterBuildMode(int type, int index)
{
	if (prefab != nullptr)
	{
		exitBuildMode();
		return;
	}

	makePrefab(type, index);

	if (prefab == nullptr) return;

	prefab->tag = TAG_PREVIEW;
	for (auto& child : prefab->children)
	{
		child->layer = (int)RenderLayer::BuildPreview;
		child->AddComponent<Constant>()->v4 = { 0.0f, 1.0f, 0.0f, 1.0f };
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

void BuildingBuilder::enterDeleteMode()
{

}

void BuildingBuilder::pickToDelete()
{
	Vector3 mousePosInWorld = getPosOnTerrain();

	int x = mousePosInWorld.x / gameObject->scene->spatialPartitioningManager.sectorWidth;
	int y = mousePosInWorld.z / gameObject->scene->spatialPartitioningManager.sectorHeight;
	
	if (0 > x || x >= gameObject->scene->spatialPartitioningManager.xSize || 0 > y || y >= gameObject->scene->spatialPartitioningManager.ySize) 
		return;

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
						Building* building = object->GetComponent<Building>();

						if (HostNetwork::network->isConnect) {
							Vector3 building_forward = object->transform->forward;
							building_forward.y = 0;
							building_forward.Normalize();
							Vector3 forward = { 0,0,1 };
							float angle = Vector3::DotProduct(forward, building_forward);
							Vector3 dir = Vector3::CrossProduct(forward, building_forward);
							angle = XMConvertToDegrees(acos(angle));
							angle *= (dir.y > 0.0f) ? 1.0f : -1.0f;
							HostNetwork::network->send_destruct_packet(building->type, building->index, object->transform->position.x, object->transform->position.z, angle);
						}
						HostGameWorld::gameWorld->deleteInGameWorld(building->landmark, object, building->type, building->index);
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

void BuildingBuilder::hostLoad(int type, int index, float x, float z, float angle, int range)
{
	if (type != Landmark) {
		for (auto landmark : HostGameWorld::gameWorld->buildingList) {
			Vector3 landmarkPos = landmark.first->transform->position;
			float dist = sqrt(pow(landmarkPos.x - x, 2) + pow(landmarkPos.z - z, 2));
			if (landmark.first->GetComponent<Village>()->radiusOfLand >= dist) {
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

					Vector3 pos{ x, terrain->terrainData.GetHeight(x,z), z };
					obj->transform->position = pos;
					obj->transform->Rotate(Vector3(0, 1, 0), angle);
					obj->AddComponent<Building>()->setBuildingInform(landmark.first, type, index);
					obj->tag = TAG_BUILDING;

					updateTerrainNodeData(obj, true);
					HostGameWorld::gameWorld->buildInGameWorld(landmark.first, obj, type, index);
					return;
				}
			}
		}
		return;
	}

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
	obj->transform->Rotate({ 0.0,1.0, 0.0 }, angle);
	obj->transform->position = { x, terrain->terrainData.GetHeight(x, z),z };
	obj->AddComponent<Building>()->setBuildingInform(obj, type, index);
	obj->tag = TAG_BUILDING;
	obj->AddComponent<Village>()->OnAutoDevelopment();
	obj->GetComponent<Village>()->radiusOfLand = range;

	updateTerrainNodeData(obj, true);
	HostGameWorld::gameWorld->buildInGameWorld(obj, obj, type, index);
}

void BuildingBuilder::guestBuild(int type, int index, float x, float z, float angle, int range)
{
	if (type != Landmark) {
		for (auto landmark : GuestGameWorld::gameWorld->buildingList) {
			Vector3 landmarkPos = landmark.first->transform->position;
			float dist = sqrt(pow(landmarkPos.x - x, 2) + pow(landmarkPos.z - z, 2));
			if (landmark.first->GetComponent<Village>()->radiusOfLand >= dist) {
				if (index < building[type].size())
				{
					GameObject* obj;

					auto data = building[type][index];
					if (data.prefab)
						obj = Scene::scene->Duplicate(data.prefab);
					else
					{
						obj = Scene::scene->CreateEmpty();

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

					Vector3 pos{ x, terrain->terrainData.GetHeight(x,z), z };
					obj->transform->position = pos;
					obj->transform->Rotate(Vector3(0, 1, 0), angle);
					obj->AddComponent<Building>()->setBuildingInform(landmark.first, type, index);

					GuestGameWorld::gameWorld->buildInGameWorld(landmark.first, obj, type, index);
					return;
				}
			}
		}
		return;
	}

	GameObject* obj;
	auto data = building[type][index];
	if (data.prefab)
		obj = Scene::scene->Duplicate(data.prefab);
	else
	{
		obj = Scene::scene->CreateEmpty();

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
	obj->transform->Rotate({ 0.0,1.0, 0.0 }, angle);
	obj->transform->position = { x, terrain->terrainData.GetHeight(x, z),z };
	obj->AddComponent<Building>()->setBuildingInform(obj, type, index);
	obj->AddComponent<Village>()->OffAutoDevelopment();
	obj->GetComponent<Village>()->radiusOfLand = range;

	GuestGameWorld::gameWorld->buildInGameWorld(obj, obj, type, index);
}
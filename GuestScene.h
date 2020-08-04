#pragma once
#include "Scene.h"

class GuestScene: public Scene
{
public:
	GuestScene() : Scene() {}
	~GuestScene() {}

	virtual void BuildObjects();

	void LoadTextureAsset()
	{
		ASSET AddTexture("ground", L"Textures\\grass.dds");
		ASSET AddTexture("grass", L"Texture\\grass.dds");

		ASSET AddTexture("polyArtTex", L"Textures\\PolyArtTex.dds");

		//ASSET AddTexture("fireTexD", L"Textures\\fire.dds");
		//ASSET AddTexture("smokeTexD", L"Textures\\smoke.dds");


		/*AdvancedVillagePack*/
		{
			ASSET AddTexture("AV_house_01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_04_D.dds");
			ASSET AddTexture("AV_house_02", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_09_D.dds");
			ASSET AddTexture("AV_material_01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_01_D.dds");
			ASSET AddTexture("AV_material_02", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_02_D.dds");
			ASSET AddTexture("AV_material_03", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_03_D.dds");
			//ASSET AddTexture("AV_TreeLeafs", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_TreeLeafs_D.dds");
			//ASSET AddTexture("AV_TreeTrunks", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_TreeTrunks_D.dds");
		}


		/*HandPaintedEnviroment*/
		{
			ASSET AddTexture("HP_Baker_house", L"Assets\\HandPaintedEnvironment\\Textures\\Baker_house_texture.dds");
			ASSET AddTexture("HP_Barrel", L"Assets\\HandPaintedEnvironment\\Textures\\Barrel_texture.dds");
			ASSET AddTexture("HP_Boxes", L"Assets\\HandPaintedEnvironment\\Textures\\Boxes_texture.dds");
			ASSET AddTexture("HP_Cauldron", L"Assets\\HandPaintedEnvironment\\Textures\\Cauldron_texture.dds");
			ASSET AddTexture("HP_Chest", L"Assets\\HandPaintedEnvironment\\Textures\\Chest_texture.dds");
			ASSET AddTexture("HP_Crystals", L"Assets\\HandPaintedEnvironment\\Textures\\Crystals_texture.dds");
			ASSET AddTexture("HP_Fence", L"Assets\\HandPaintedEnvironment\\Textures\\Fence_texture.dds");
			ASSET AddTexture("HP_Fern", L"Assets\\HandPaintedEnvironment\\Textures\\Fern_texture.dds");
			ASSET AddTexture("HP_Forge", L"Assets\\HandPaintedEnvironment\\Textures\\Forge_texture.dds");
			ASSET AddTexture("HP_Hammer_anvil", L"Assets\\HandPaintedEnvironment\\Textures\\Hammer_anvil_texture.dds");
			ASSET AddTexture("HP_Lampposts", L"Assets\\HandPaintedEnvironment\\Textures\\Lampposts_texture.dds");
			ASSET AddTexture("HP_Large_house", L"Assets\\HandPaintedEnvironment\\Textures\\Large_house_texture.dds");
			ASSET AddTexture("HP_Log_stump_mushrooms", L"Assets\\HandPaintedEnvironment\\Textures\\Log_stump_mushrooms_texture.dds");
			ASSET AddTexture("HP_Logs", L"Assets\\HandPaintedEnvironment\\Textures\\Logs_texture.dds");
			ASSET AddTexture("HP_Mill", L"Assets\\HandPaintedEnvironment\\Textures\\Mill_texture.dds");
			ASSET AddTexture("HP_Mine", L"Assets\\HandPaintedEnvironment\\Textures\\Mine_texture.dds");
			ASSET AddTexture("HP_Pillars_arch", L"Assets\\HandPaintedEnvironment\\Textures\\Pillars_arch_texture.dds");
			ASSET AddTexture("HP_Pine_tree", L"Assets\\HandPaintedEnvironment\\Textures\\Pine_tree_texture.dds");
			ASSET AddTexture("HP_Pine_tree_2", L"Assets\\HandPaintedEnvironment\\Textures\\Pine_tree_2_texture.dds");
			ASSET AddTexture("HP_Plant", L"Assets\\HandPaintedEnvironment\\Textures\\Plant_texture.dds");
			ASSET AddTexture("HP_Plants", L"Assets\\HandPaintedEnvironment\\Textures\\Plants_texture.dds");
			ASSET AddTexture("HP_Rocks_1_2", L"Assets\\HandPaintedEnvironment\\Textures\\Rocks_1_2_texture.dds");
			ASSET AddTexture("HP_Rocks_3_4", L"Assets\\HandPaintedEnvironment\\Textures\\Rocks_3_4_texture.dds");
			ASSET AddTexture("HP_Ruined_tower", L"Assets\\HandPaintedEnvironment\\Textures\\Ruined_tower_texture.dds");
			ASSET AddTexture("HP_Ruined_walls", L"Assets\\HandPaintedEnvironment\\Textures\\Ruined_walls_texture.dds");
			ASSET AddTexture("HP_Small_house", L"Assets\\HandPaintedEnvironment\\Textures\\Small_house_texture.dds");
			ASSET AddTexture("HP_Stone_fence", L"Assets\\HandPaintedEnvironment\\Textures\\Stone_fence_texture.dds");
			ASSET AddTexture("HP_Street_light", L"Assets\\HandPaintedEnvironment\\Textures\\Street_light_texture.dds");
			ASSET AddTexture("HP_Stone_tiles", L"Assets\\HandPaintedEnvironment\\Textures\\Stone_tiles_texture.dds");
			ASSET AddTexture("HP_Tavern", L"Assets\\HandPaintedEnvironment\\Textures\\Tavern_texture.dds");
			ASSET AddTexture("HP_Town_Hall", L"Assets\\HandPaintedEnvironment\\Textures\\Town_Hall_texture.dds");
			ASSET AddTexture("HP_Tree_1_1", L"Assets\\HandPaintedEnvironment\\Textures\\Tree_1_1_texture.dds");
			ASSET AddTexture("HP_Tree_1_2", L"Assets\\HandPaintedEnvironment\\Textures\\Tree_1_2_texture.dds");
			ASSET AddTexture("HP_Tree_2_1", L"Assets\\HandPaintedEnvironment\\Textures\\Tree_2_1_texture.dds");
			ASSET AddTexture("HP_Tree_2_2", L"Assets\\HandPaintedEnvironment\\Textures\\Tree_2_2_texture.dds");
			ASSET AddTexture("HP_Trolley", L"Assets\\HandPaintedEnvironment\\Textures\\Trolley_texture.dds");
			ASSET AddTexture("HP_Well", L"Assets\\HandPaintedEnvironment\\Textures\\Well_texture.dds");
			ASSET AddTexture("HP_Witch_house", L"Assets\\HandPaintedEnvironment\\Textures\\Witch_house_texture.dds");
			ASSET AddTexture("HP_Woodmill", L"Assets\\HandPaintedEnvironment\\Textures\\Woodmill.dds");

		}


		/*SpringEnvironment*/
		{
			ASSET AddTexture("SE_Baloons_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Baloons_Diffuse.dds");
			ASSET AddTexture("SE_Bench_01a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Bench_01a_Diffuse.dds");
			ASSET AddTexture("SE_Bench_01b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Bench_01b_Diffuse.dds");
			ASSET AddTexture("SE_Birch_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Birch_Diffuse.dds");
			ASSET AddTexture("SE_Birch_01b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Birch_01b_Diffuse.dds");
			ASSET AddTexture("SE_Blanket_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Blanket_Diffuse.dds");
			ASSET AddTexture("SE_Bushese_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Bushes_Diffuse.dds");
			ASSET AddTexture("SE_Bonsai_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Bonsai_Diffuse.dds");
			ASSET AddTexture("SE_Booth_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Booth_Diffuse.dds");
			ASSET AddTexture("SE_Candies_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Candies_Diffuse.dds");
			ASSET AddTexture("SE_Candyhouse_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Candyhouse_Diffuse.dds");
			ASSET AddTexture("SE_Carousel_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Carousel_Diffuse.dds");
			ASSET AddTexture("SE_Cart_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Cart_Diffuse.dds");
			ASSET AddTexture("SE_Climbers_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Climbers_Diffuse.dds");
			ASSET AddTexture("SE_CottonCandyTree_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_CottonCandyTree_Diffuse.dds");
			ASSET AddTexture("SE_FabricHouse_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_FabricHouse_Diffuse.dds");
			ASSET AddTexture("SE_Factory_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Factory_Diffuse.dds");
			ASSET AddTexture("SE_FabricHouse_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_FabricHouse_Diffuse.dds");
			ASSET AddTexture("SE_Fence_01a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Fence_01a_Diffuse.dds");
			ASSET AddTexture("SE_Fence_01b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Fence_01b_Diffuse.dds");
			ASSET AddTexture("SE_Flowers_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Flowers_Diffuse.dds");
			ASSET AddTexture("SE_Gate_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Gate_Diffuse.dds");
			ASSET AddTexture("SE_Gazebo_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Gazebo_Diffuse.dds");
			ASSET AddTexture("SE_Grill_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Grill_Diffuse.dds");
			ASSET AddTexture("SE_HeartHouse_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_HeartHouse_Diffuse.dds");
			ASSET AddTexture("SE_Hedges_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Hedges_Diffuse.dds");
			ASSET AddTexture("SE_HotAirBaloon_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_HotAirBaloon_Diffuse.dds");
			ASSET AddTexture("SE_House_01a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_01a_Diffuse.dds");
			ASSET AddTexture("SE_House_01b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_01b_Diffuse.dds");
			ASSET AddTexture("SE_House_01c_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_01c_Diffuse.dds");
			ASSET AddTexture("SE_House_01d_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_01d_Diffuse.dds");
			ASSET AddTexture("SE_House_02a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_02a_Diffuse.dds");
			ASSET AddTexture("SE_House_02b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_02b_Diffuse.dds");
			ASSET AddTexture("SE_House_02c_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_02c_Diffuse.dds");
			ASSET AddTexture("SE_House_02d_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_02d_Diffuse.dds");
			ASSET AddTexture("SE_House_03d_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_03d_Diffuse.dds");
			ASSET AddTexture("SE_House_03a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_03a_Diffuse.dds");
			ASSET AddTexture("SE_House_03b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_03b_Diffuse.dds");
			ASSET AddTexture("SE_House_03c_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_03c_Diffuse.dds");
			ASSET AddTexture("SE_House_04a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_04a_Diffuse.dds");
			ASSET AddTexture("SE_House_04b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_04b_Diffuse.dds");
			ASSET AddTexture("SE_House_05a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_05a_Diffuse.dds");
			ASSET AddTexture("SE_House_05b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_05b_Diffuse.dds");
			ASSET AddTexture("SE_Lamps_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Lamps_Diffuse.dds");
			ASSET AddTexture("SE_Maypole_01_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Maypole_01_Diffuse.dds");
			ASSET AddTexture("SE_Presents_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Presents_Diffuse.dds");
			ASSET AddTexture("SE_Pump_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Pump_Diffuse.dds");
			ASSET AddTexture("SE_Roses_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Roses_Diffuse.dds");
			ASSET AddTexture("SE_Seesaw_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Seesaw_Diffuse.dds");
			ASSET AddTexture("SE_Sign_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Sign_Diffuse.dds");
			ASSET AddTexture("SE_Slide_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Slide_Diffuse.dds");
			ASSET AddTexture("SE_Stones_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Stones_Diffuse.dds");
			ASSET AddTexture("SE_Swing_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Swing_Diffuse.dds");
			ASSET AddTexture("SE_Tables_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Tables_Diffuse.dds");
			ASSET AddTexture("SE_Theater_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Theater_Diffuse.dds");
			ASSET AddTexture("SE_TownHall_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_TownHall_Diffuse.dds");
			ASSET AddTexture("SE_Trampoline_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Trampoline_Diffuse.dds");
			ASSET AddTexture("SE_Willow_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Willow_Diffuse.dds");
		}
	}

	void LoadMaterialAsset()
	{
		ASSET AddMaterial("none", ASSET TEXTURE("none"));
		ASSET AddMaterial("gray", ASSET TEXTURE("none"), 0, 0, 0, { 0.5, 0.5, 0.5, 1.0 });
		ASSET AddMaterial("ground", ASSET TEXTURE("ground"), 0, 0, 0, { 0.48f, 0.64f, 0.2f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f, Matrix4x4::MatrixScaling(200, 200, 200));
		ASSET AddMaterial("grass", ASSET TEXTURE("grass"), 0, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);

		ASSET AddMaterial("PolyArt", ASSET TEXTURE("polyArtTex"), 0, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);

		//ASSET AddMaterial("fireMat", ASSET TEXTURE("fireTexD"));
		//ASSET AddMaterial("smokeMat", ASSET TEXTURE("smokeTexD"));


		/*AdvancedVillagePack*/
		{
			ASSET AddMaterial("AV_house_01", ASSET TEXTURE("AV_house_01"), 0, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
			ASSET AddMaterial("AV_house_02", ASSET TEXTURE("AV_house_02"), 0, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
			ASSET AddMaterial("AV_material_01", ASSET TEXTURE("AV_material_01"), 0, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
			ASSET AddMaterial("AV_material_02", ASSET TEXTURE("AV_material_02"), 0, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
			ASSET AddMaterial("AV_material_03", ASSET TEXTURE("AV_material_03"), 0, 0, 0, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
			//ASSET AddMaterial("TreeLeafs", ASSET TEXTURE("TreeLeafs"));
			//ASSET AddMaterial("TreeTrunks", ASSET TEXTURE("TreeTrunks"));
		}


		/*HandPaintedEnviroment*/
		{
			ASSET AddMaterial("HP_Baker_house", ASSET TEXTURE("HP_Baker_house"));
			ASSET AddMaterial("HP_Barrel", ASSET TEXTURE("HP_Barrel"));
			ASSET AddMaterial("HP_Boxes", ASSET TEXTURE("HP_Boxes"));
			ASSET AddMaterial("HP_Cauldron", ASSET TEXTURE("HP_Cauldron"));
			ASSET AddMaterial("HP_Chest", ASSET TEXTURE("HP_Chest"));
			ASSET AddMaterial("HP_Crystals", ASSET TEXTURE("HP_Crystals"));
			ASSET AddMaterial("HP_Fence", ASSET TEXTURE("HP_Fence"));
			ASSET AddMaterial("HP_Fern", ASSET TEXTURE("HP_Fern"));
			ASSET AddMaterial("HP_Forge", ASSET TEXTURE("HP_Forge"));
			ASSET AddMaterial("HP_Hammer_anvil", ASSET TEXTURE("HP_Hammer_anvil"));
			ASSET AddMaterial("HP_Lampposts", ASSET TEXTURE("HP_Lampposts"));
			ASSET AddMaterial("HP_Large_house", ASSET TEXTURE("HP_Large_house"));
			ASSET AddMaterial("HP_Log_stump_mushrooms", ASSET TEXTURE("HP_Log_stump_mushrooms"));
			ASSET AddMaterial("HP_Logs", ASSET TEXTURE("HP_Logs"));
			ASSET AddMaterial("HP_Mill", ASSET TEXTURE("HP_Mill"));
			ASSET AddMaterial("HP_Mine", ASSET TEXTURE("HP_Mine"));
			ASSET AddMaterial("HP_Pillars_arch", ASSET TEXTURE("HP_Pillars_arch"));
			ASSET AddMaterial("HP_Pine_tree_01", ASSET TEXTURE("HP_Pine_tree"));
			ASSET AddMaterial("HP_Pine_tree_02", ASSET TEXTURE("HP_Pine_tree_2"));
			ASSET AddMaterial("HP_Plant", ASSET TEXTURE("HP_Plant"));
			ASSET AddMaterial("HP_Plants", ASSET TEXTURE("HP_Plants"));
			ASSET AddMaterial("HP_Rocks_1_2", ASSET TEXTURE("HP_Rocks_1_2"));
			ASSET AddMaterial("HP_Rocks_3_4", ASSET TEXTURE("HP_Rocks_3_4"));
			ASSET AddMaterial("HP_Ruined_tower", ASSET TEXTURE("HP_Ruined_tower"));
			ASSET AddMaterial("HP_Ruined_walls", ASSET TEXTURE("HP_Ruined_walls"));
			ASSET AddMaterial("HP_Small_house", ASSET TEXTURE("HP_Small_house"));
			ASSET AddMaterial("HP_Stone_fence", ASSET TEXTURE("HP_Stone_fence"));
			ASSET AddMaterial("HP_Stone_tiles", ASSET TEXTURE("HP_Stone_tiles"));
			ASSET AddMaterial("HP_Street_light", ASSET TEXTURE("HP_Street_light"));
			ASSET AddMaterial("HP_Tavern", ASSET TEXTURE("HP_Tavern"));
			ASSET AddMaterial("HP_Town_Hall", ASSET TEXTURE("HP_Town_Hall"));
			ASSET AddMaterial("HP_Tree_01", ASSET TEXTURE("HP_Tree_1_1"));
			ASSET AddMaterial("HP_Tree_02", ASSET TEXTURE("HP_Tree_1_2"));
			ASSET AddMaterial("HP_Tree_03", ASSET TEXTURE("HP_Tree_2_1"));
			ASSET AddMaterial("HP_Tree_04", ASSET TEXTURE("HP_Tree_2_2"));
			ASSET AddMaterial("HP_Trolley", ASSET TEXTURE("HP_Trolley"));
			ASSET AddMaterial("HP_Well", ASSET TEXTURE("HP_Well"));
			ASSET AddMaterial("HP_Witch_house", ASSET TEXTURE("HP_Witch_house"));
			ASSET AddMaterial("HP_Woodmill", ASSET TEXTURE("HP_Woodmill"));
		}


		/*SpringEnvironment*/
		{
			ASSET AddMaterial("SE_Baloons", ASSET TEXTURE("SE_Baloons_Diffuse"));
			ASSET AddMaterial("SE_Bench_01a", ASSET TEXTURE("SE_Bench_01a_Diffuse"));
			ASSET AddMaterial("SE_Bench_01b", ASSET TEXTURE("SE_Bench_01b_Diffuse"));
			ASSET AddMaterial("SE_Birch", ASSET TEXTURE("SE_Birch_Diffuse"));
			ASSET AddMaterial("SE_Birch_01b", ASSET TEXTURE("SE_Birch_01b_Diffuse"));
			ASSET AddMaterial("SE_Blanket", ASSET TEXTURE("SE_Blanket_Diffuse"));
			ASSET AddMaterial("SE_Bushese", ASSET TEXTURE("SE_Bushese_Diffuse"));
			ASSET AddMaterial("SE_Bonsai", ASSET TEXTURE("SE_Bonsai_Diffuse"));
			ASSET AddMaterial("SE_Booth", ASSET TEXTURE("SE_Booth_Diffuse"));
			ASSET AddMaterial("SE_Candies", ASSET TEXTURE("SE_Candies_Diffuse"));
			ASSET AddMaterial("SE_Candyhouse", ASSET TEXTURE("SE_Candyhouse_Diffuse"));
			ASSET AddMaterial("SE_Carousel", ASSET TEXTURE("SE_Carousel_Diffuse"));
			ASSET AddMaterial("SE_Cart", ASSET TEXTURE("SE_Cart_Diffuse"));
			ASSET AddMaterial("SE_Climbers", ASSET TEXTURE("SE_Climbers_Diffuse"));
			ASSET AddMaterial("SE_CottonCandyTree", ASSET TEXTURE("SE_CottonCandyTree_Diffuse"));
			ASSET AddMaterial("SE_FabricHouse", ASSET TEXTURE("SE_FabricHouse_Diffuse"));
			ASSET AddMaterial("SE_Factory", ASSET TEXTURE("SE_Factory_Diffuse"));
			ASSET AddMaterial("SE_Fence_01a", ASSET TEXTURE("SE_Fence_01a_Diffuse"));
			ASSET AddMaterial("SE_Fence_01b", ASSET TEXTURE("SE_Fence_01b_Diffuse"));
			ASSET AddMaterial("SE_Flowers", ASSET TEXTURE("SE_Flowers_Diffuse"));
			ASSET AddMaterial("SE_Gate", ASSET TEXTURE("SE_Gate_Diffuse"));
			ASSET AddMaterial("SE_Gazebo", ASSET TEXTURE("SE_Gazebo_Diffuse"));
			ASSET AddMaterial("SE_Grill", ASSET TEXTURE("SE_Grill_Diffuse"));
			ASSET AddMaterial("SE_HeartHouse", ASSET TEXTURE("SE_HeartHouse_Diffuse"));
			ASSET AddMaterial("SE_Hedges", ASSET TEXTURE("SE_Hedges_Diffuse"));
			ASSET AddMaterial("SE_HotAirBaloon", ASSET TEXTURE("SE_HotAirBaloon_Diffuse"));
			ASSET AddMaterial("SE_House_01a", ASSET TEXTURE("SE_House_01a_Diffuse"));
			ASSET AddMaterial("SE_House_01b", ASSET TEXTURE("SE_House_01b_Diffuse"));
			ASSET AddMaterial("SE_House_01c", ASSET TEXTURE("SE_House_01c_Diffuse"));
			ASSET AddMaterial("SE_House_01d", ASSET TEXTURE("SE_House_01d_Diffuse"));
			ASSET AddMaterial("SE_House_02a", ASSET TEXTURE("SE_House_02a_Diffuse"));
			ASSET AddMaterial("SE_House_02b", ASSET TEXTURE("SE_House_02b_Diffuse"));
			ASSET AddMaterial("SE_House_02c", ASSET TEXTURE("SE_House_02c_Diffuse"));
			ASSET AddMaterial("SE_House_02d", ASSET TEXTURE("SE_House_02d_Diffuse"));
			ASSET AddMaterial("SE_House_03d", ASSET TEXTURE("SE_House_03d_Diffuse"));
			ASSET AddMaterial("SE_House_03a", ASSET TEXTURE("SE_House_03a_Diffuse"));
			ASSET AddMaterial("SE_House_03b", ASSET TEXTURE("SE_House_03b_Diffuse"));
			ASSET AddMaterial("SE_House_03c", ASSET TEXTURE("SE_House_03c_Diffuse"));
			ASSET AddMaterial("SE_House_04a", ASSET TEXTURE("SE_House_04a_Diffuse"));
			ASSET AddMaterial("SE_House_04b", ASSET TEXTURE("SE_House_04b_Diffuse"));
			ASSET AddMaterial("SE_House_05a", ASSET TEXTURE("SE_House_05a_Diffuse"));
			ASSET AddMaterial("SE_House_05b", ASSET TEXTURE("SE_House_05b_Diffuse"));
			ASSET AddMaterial("SE_Lamps", ASSET TEXTURE("SE_Lamps_Diffuse"));
			ASSET AddMaterial("SE_Maypole_01", ASSET TEXTURE("SE_Maypole_01_Diffuse"));
			ASSET AddMaterial("SE_Presents", ASSET TEXTURE("SE_Presents_Diffuse"));
			ASSET AddMaterial("SE_Pump", ASSET TEXTURE("SE_Pump_Diffuse"));
			ASSET AddMaterial("SE_Roses", ASSET TEXTURE("SE_Roses_Diffuse"));
			ASSET AddMaterial("SE_Seesaw", ASSET TEXTURE("SE_Seesaw_Diffuse"));
			ASSET AddMaterial("SE_Sign", ASSET TEXTURE("SE_Sign_Diffuse"));
			ASSET AddMaterial("SE_Slide", ASSET TEXTURE("SE_Slide_Diffuse"));
			ASSET AddMaterial("SE_Stones", ASSET TEXTURE("SE_Stones_Diffuse"));
			ASSET AddMaterial("SE_Swing", ASSET TEXTURE("SE_Swing_Diffuse"));
			ASSET AddMaterial("SE_Tables", ASSET TEXTURE("SE_Tables_Diffuse"));
			ASSET AddMaterial("SE_Theater", ASSET TEXTURE("SE_Theater_Diffuse"));
			ASSET AddMaterial("SE_TownHall", ASSET TEXTURE("SE_TownHall_Diffuse"));
			ASSET AddMaterial("SE_Trampoline", ASSET TEXTURE("SE_Trampoline_Diffuse"));
			ASSET AddMaterial("SE_Willow", ASSET TEXTURE("SE_Willow_Diffuse"));
		}
	}

	void LoadMeshAsset()
	{
		//ASSET AddMesh("Image", Mesh::CreateQuad());
		//ASSET AddMesh("Sphere", Mesh::CreateSphere());

		/*AdvancedVillagePack*/
		{
			ASSET AddFbxForMesh("AV_Well", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well.FBX");
			ASSET AddFbxForMesh("AV_Well_Extra02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well_Extra02.FBX");
			ASSET AddFbxForMesh("AV_Well_Extra03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well_Extra03.FBX");
			ASSET AddFbxForMesh("AV_House_01", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var01.FBX");
			ASSET AddFbxForMesh("AV_House_02", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02.FBX");
			ASSET AddFbxForMesh("AV_House_02_Extra", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02_Extra.FBX");
			//ASSET AddFbxForMesh("AV_Tree_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var01.FBX");
			//ASSET AddFbxForMesh("AV_Tree_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var02.FBX");
			//ASSET AddFbxForMesh("AV_Tree_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var03.FBX");
			//ASSET AddFbxForMesh("AV_Tree_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var04.FBX");
			//ASSET AddFbxForMesh("AV_Tree_Var05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var05.FBX");
			ASSET AddFbxForMesh("AV_Mushroom_01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var01.FBX");
			ASSET AddFbxForMesh("AV_Mushroom_02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var02.FBX");
			ASSET AddFbxForMesh("AV_Mushroom_03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var03.FBX");
			ASSET AddFbxForMesh("AV_Mushroom_04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var04.FBX");
			ASSET AddFbxForMesh("AV_Mushroom_05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var05.FBX");
			ASSET AddFbxForMesh("AV_Mushroom_06", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var06.FBX");
			ASSET AddFbxForMesh("AV_Cart_01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var01.FBX");
			ASSET AddFbxForMesh("AV_Cart_02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var02.FBX");
			ASSET AddFbxForMesh("AV_Barrel", "Assets\\AdvancedVillagePack\\Meshes\\SM_Barrel.FBX");
			ASSET AddFbxForMesh("AV_Bucket", "Assets\\AdvancedVillagePack\\Meshes\\SM_Bucket.FBX");
			ASSET AddFbxForMesh("AV_Fence_01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var01.FBX");
			ASSET AddFbxForMesh("AV_Fence_02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var02.FBX");
			ASSET AddFbxForMesh("AV_Fence_03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var03.FBX");
			ASSET AddFbxForMesh("AV_Fence_04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var04.FBX");
			ASSET AddFbxForMesh("AV_Cauldron", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cauldron.FBX");
			ASSET AddFbxForMesh("AV_Spike", "Assets\\AdvancedVillagePack\\Meshes\\SM_Spike.FBX");
			ASSET AddFbxForMesh("AV_Fish", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fish.FBX");
			ASSET AddFbxForMesh("AV_Pot_01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var01.FBX");
			ASSET AddFbxForMesh("AV_Pot_02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var02.FBX");
			ASSET AddFbxForMesh("AV_Pot_03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var03.FBX");
			ASSET AddFbxForMesh("AV_Pot_04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var04.FBX");
			ASSET AddFbxForMesh("AV_Pot_05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var05.FBX");
			ASSET AddFbxForMesh("AV_Pot_06", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var06.FBX");
			ASSET AddFbxForMesh("AV_Apple", "Assets\\AdvancedVillagePack\\Meshes\\SM_Apple.FBX");
			ASSET AddFbxForMesh("AV_Potato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Potato.FBX");
			ASSET AddFbxForMesh("AV_Tomato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tomato.FBX");
			ASSET AddFbxForMesh("AV_Watermellon", "Assets\\AdvancedVillagePack\\Meshes\\SM_Watermellon.FBX");
			ASSET AddFbxForMesh("AV_Sack_Apple", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Apple.FBX");
			ASSET AddFbxForMesh("AV_Sack_Flour", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Flour.FBX");
			ASSET AddFbxForMesh("AV_Sack_Potato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Potato.FBX");
			ASSET AddFbxForMesh("AV_Sack_Tomato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Tomato.FBX");
			ASSET AddFbxForMesh("AV_Sack_01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Var01.FBX");
			ASSET AddFbxForMesh("AV_Sack_02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Var02.FBX");
		}

		/*HandPaintedEnviroment*/
		{
			ASSET AddFbxForMesh("HP_Altar", "Assets\\HandPaintedEnvironment\\Models\\Altar.FBX");
			ASSET AddFbxForMesh("HP_Anvil", "Assets\\HandPaintedEnvironment\\Models\\Anvil.FBX");
			ASSET AddFbxForMesh("HP_Baker_house", "Assets\\HandPaintedEnvironment\\Models\\Baker_house.FBX");
			ASSET AddFbxForMesh("HP_Barrel", "Assets\\HandPaintedEnvironment\\Models\\Barrel.FBX");
			ASSET AddFbxForMesh("HP_Box_1", "Assets\\HandPaintedEnvironment\\Models\\Box_1.FBX");
			ASSET AddFbxForMesh("HP_Box_2", "Assets\\HandPaintedEnvironment\\Models\\Box_2.FBX");
			ASSET AddFbxForMesh("HP_Box_3", "Assets\\HandPaintedEnvironment\\Models\\Box_3.FBX");
			ASSET AddFbxForMesh("HP_Box_4", "Assets\\HandPaintedEnvironment\\Models\\Box_4.FBX");
			ASSET AddFbxForMesh("HP_Bush_1", "Assets\\HandPaintedEnvironment\\Models\\Bush_1.FBX");
			ASSET AddFbxForMesh("HP_Bush_2", "Assets\\HandPaintedEnvironment\\Models\\Bush_2.FBX");
			ASSET AddFbxForMesh("HP_Cauldron", "Assets\\HandPaintedEnvironment\\Models\\Cauldron.FBX");
			ASSET AddFbxForMesh("HP_Crystal_1", "Assets\\HandPaintedEnvironment\\Models\\Crystal_1.FBX");
			ASSET AddFbxForMesh("HP_Crystal_2", "Assets\\HandPaintedEnvironment\\Models\\Crystal_2.FBX");
			ASSET AddFbxForMesh("HP_Crystal_3", "Assets\\HandPaintedEnvironment\\Models\\Crystal_3.FBX");
			ASSET AddFbxForMesh("HP_Crystal_4", "Assets\\HandPaintedEnvironment\\Models\\Crystal_4.FBX");
			ASSET AddFbxForMesh("HP_Crystal_5", "Assets\\HandPaintedEnvironment\\Models\\Crystal_5.FBX");
			ASSET AddFbxForMesh("HP_Fence", "Assets\\HandPaintedEnvironment\\Models\\Fence.FBX");
			ASSET AddFbxForMesh("HP_Fence_1", "Assets\\HandPaintedEnvironment\\Models\\Fence_1.FBX");
			ASSET AddFbxForMesh("HP_Fence_2", "Assets\\HandPaintedEnvironment\\Models\\Fence_2.FBX");
			ASSET AddFbxForMesh("HP_Fern", "Assets\\HandPaintedEnvironment\\Models\\Fern.FBX");
			ASSET AddFbxForMesh("HP_Flower", "Assets\\HandPaintedEnvironment\\Models\\Flower.FBX");
			ASSET AddFbxForMesh("HP_Flowers_1", "Assets\\HandPaintedEnvironment\\Models\\Flowers_1.FBX");
			ASSET AddFbxForMesh("HP_Flowers_2", "Assets\\HandPaintedEnvironment\\Models\\Flowers_2.FBX");
			ASSET AddFbxForMesh("HP_Forge", "Assets\\HandPaintedEnvironment\\Models\\Forge.FBX");
			ASSET AddFbxForMesh("HP_Hammer", "Assets\\HandPaintedEnvironment\\Models\\Hammer.FBX");
			ASSET AddFbxForMesh("HP_Lamppost", "Assets\\HandPaintedEnvironment\\Models\\Lamppost.FBX");
			ASSET AddFbxForMesh("HP_Large_house", "Assets\\HandPaintedEnvironment\\Models\\Large_house.FBX");
			ASSET AddFbxForMesh("HP_Log", "Assets\\HandPaintedEnvironment\\Models\\Log.FBX");
			ASSET AddFbxForMesh("HP_Log_1", "Assets\\HandPaintedEnvironment\\Models\\Log_1.FBX");
			ASSET AddFbxForMesh("HP_Log_2", "Assets\\HandPaintedEnvironment\\Models\\Log_2.FBX");
			ASSET AddFbxForMesh("HP_Log_3", "Assets\\HandPaintedEnvironment\\Models\\Log_3.FBX");
			ASSET AddFbxForMesh("HP_Mill", "Assets\\HandPaintedEnvironment\\Models\\Mill.FBX");
			ASSET AddFbxForMesh("HP_Mill_wings", "Assets\\HandPaintedEnvironment\\Models\\Mill_wings.FBX");
			ASSET AddFbxForMesh("HP_Mine", "Assets\\HandPaintedEnvironment\\Models\\Mine.FBX");
			ASSET AddFbxForMesh("HP_Mushroom_1", "Assets\\HandPaintedEnvironment\\Models\\Mushroom_1.FBX");
			ASSET AddFbxForMesh("HP_Mushroom_2", "Assets\\HandPaintedEnvironment\\Models\\Mushroom_2.FBX");
			ASSET AddFbxForMesh("HP_Mushroom_3", "Assets\\HandPaintedEnvironment\\Models\\Mushroom_3.FBX");
			ASSET AddFbxForMesh("HP_Mushroom_4", "Assets\\HandPaintedEnvironment\\Models\\Mushroom_4.FBX");
			ASSET AddFbxForMesh("HP_Pillar", "Assets\\HandPaintedEnvironment\\Models\\Pillar.FBX");
			ASSET AddFbxForMesh("HP_Pillar_ruined_1", "Assets\\HandPaintedEnvironment\\Models\\Pillar_ruined_1.FBX");
			ASSET AddFbxForMesh("HP_Pine_tree_01", "Assets\\HandPaintedEnvironment\\Models\\Pine_tree.FBX");
			ASSET AddFbxForMesh("HP_Pine_tree_02", "Assets\\HandPaintedEnvironment\\Models\\Pine_tree_2.FBX");
			ASSET AddFbxForMesh("HP_Plant", "Assets\\HandPaintedEnvironment\\Models\\Plant.FBX");
			ASSET AddFbxForMesh("HP_Rock_1", "Assets\\HandPaintedEnvironment\\Models\\Rock_1.FBX");
			ASSET AddFbxForMesh("HP_Rock_2", "Assets\\HandPaintedEnvironment\\Models\\Rock_2.FBX");
			ASSET AddFbxForMesh("HP_Rock_3", "Assets\\HandPaintedEnvironment\\Models\\Rock_3.FBX");
			ASSET AddFbxForMesh("HP_Rock_4", "Assets\\HandPaintedEnvironment\\Models\\Rock_4.FBX");
			ASSET AddFbxForMesh("HP_Small_house", "Assets\\HandPaintedEnvironment\\Models\\Small_house.FBX");
			ASSET AddFbxForMesh("HP_Stone_fence", "Assets\\HandPaintedEnvironment\\Models\\Stone_fence.FBX");
			ASSET AddFbxForMesh("HP_street_light", "Assets\\HandPaintedEnvironment\\Models\\Street_light.FBX");
			ASSET AddFbxForMesh("HP_Stump", "Assets\\HandPaintedEnvironment\\Models\\Stump.FBX");
			ASSET AddFbxForMesh("HP_Tavern", "Assets\\HandPaintedEnvironment\\Models\\Tavern.FBX");
			ASSET AddFbxForMesh("HP_Tile_1", "Assets\\HandPaintedEnvironment\\Models\\Tile_1.FBX");
			ASSET AddFbxForMesh("HP_Tile_2", "Assets\\HandPaintedEnvironment\\Models\\Tile_2.FBX");
			ASSET AddFbxForMesh("HP_Tile_3", "Assets\\HandPaintedEnvironment\\Models\\Tile_3.FBX");
			ASSET AddFbxForMesh("HP_Tile_4", "Assets\\HandPaintedEnvironment\\Models\\Tile_4.FBX");
			ASSET AddFbxForMesh("HP_Tile_5", "Assets\\HandPaintedEnvironment\\Models\\Tile_5.FBX");
			ASSET AddFbxForMesh("HP_Tile_6", "Assets\\HandPaintedEnvironment\\Models\\Tile_6.FBX");
			ASSET AddFbxForMesh("HP_Tile_7", "Assets\\HandPaintedEnvironment\\Models\\Tile_7.FBX");
			ASSET AddFbxForMesh("HP_Tower", "Assets\\HandPaintedEnvironment\\Models\\Tower.FBX");
			ASSET AddFbxForMesh("HP_Town_Hall", "Assets\\HandPaintedEnvironment\\Models\\Town_Hall.FBX");
			ASSET AddFbxForMesh("HP_Tree_01", "Assets\\HandPaintedEnvironment\\Models\\Tree_1.FBX");
			ASSET AddFbxForMesh("HP_Tree_03", "Assets\\HandPaintedEnvironment\\Models\\Tree_2.FBX");
			ASSET AddFbxForMesh("HP_Tree_04", "Assets\\HandPaintedEnvironment\\Models\\Tree_2_1.FBX");
			ASSET AddFbxForMesh("HP_Tree_02", "Assets\\HandPaintedEnvironment\\Models\\Tree_4.FBX");
			ASSET AddFbxForMesh("HP_Wall_1", "Assets\\HandPaintedEnvironment\\Models\\Wall_1.FBX");
			ASSET AddFbxForMesh("HP_Wall_2", "Assets\\HandPaintedEnvironment\\Models\\Wall_2.FBX");
			ASSET AddFbxForMesh("HP_Well", "Assets\\HandPaintedEnvironment\\Models\\Well.FBX");
			ASSET AddFbxForMesh("HP_Witch_house", "Assets\\HandPaintedEnvironment\\Models\\Witch_house.FBX");
			ASSET AddFbxForMesh("HP_Woodmill", "Assets\\HandPaintedEnvironment\\Models\\Woodmill.FBX");
			ASSET AddFbxForMesh("HP_Woodmill_Saw", "Assets\\HandPaintedEnvironment\\Models\\Woodmill_Saw.FBX");
		}


		/*SpringEnvironment*/
		{
			ASSET AddFbxForMesh("SE_Baloon_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Baloon_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Baloon_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Baloon_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Baloon_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Baloon_03_SM.FBX");
			ASSET AddFbxForMesh("SE_Baloon_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Baloon_04_SM.FBX");
			ASSET AddFbxForMesh("SE_Baloons_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Baloons_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Bench_01a_SM", "Assets\\SpringEnvironment\\Models\\SE_Bench_01a_SM.FBX");
			ASSET AddFbxForMesh("SE_Bench_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_Bench_01b_SM.FBX");
			ASSET AddFbxForMesh("SE_Birch_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Birch_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_01b_SM.FBX");
			ASSET AddFbxForMesh("SE_Birch_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Birch_02b_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_02b_SM.FBX");
			ASSET AddFbxForMesh("SE_Birch_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_03_SM.FBX");
			ASSET AddFbxForMesh("SE_Birch_03b_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_03b_SM.FBX");
			ASSET AddFbxForMesh("SE_Blanket_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Blanket_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Blanket_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Blanket_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Blossom_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Blossom_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Blossom_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Blossom_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Bonsai_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Bonsai_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Bonsai_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Bonsai_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Booth_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Booth_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Booth_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Booth_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Branch_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Branch_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Branch_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Branch_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Blossom_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Blossom_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Blossom_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Blossom_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Bush_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Bush_06_SM.FBX");
			ASSET AddFbxForMesh("SE_Bush_07_SM", "Assets\\SpringEnvironment\\Models\\SE_Bush_07_SM.FBX");
			ASSET AddFbxForMesh("SE_Candy_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Candy_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Candy_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Candy_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Candy_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Candy_03_SM.FBX");
			ASSET AddFbxForMesh("SE_Candy_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Candy_04_SM.FBX");
			ASSET AddFbxForMesh("SE_CandyHouse_01_SM", "Assets\\SpringEnvironment\\Models\\SE_CandyHouse_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Carousel_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Carousel_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Cart_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Cart_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Chair_01a_SM", "Assets\\SpringEnvironment\\Models\\SE_Chair_01a_SM.FBX");
			ASSET AddFbxForMesh("SE_Climber_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Climber_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Climber_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Climber_02_SM.FBX");
			ASSET AddFbxForMesh("SE_CottonCandy_01_SM", "Assets\\SpringEnvironment\\Models\\SE_CottonCandy_01_SM.FBX");
			ASSET AddFbxForMesh("SE_CottonCandyTree_01_SM", "Assets\\SpringEnvironment\\Models\\SE_CottonCandyTree_01_SM.FBX");
			ASSET AddFbxForMesh("SE_CottonCandyTree_02_SM", "Assets\\SpringEnvironment\\Models\\SE_CottonCandyTree_02_SM.FBX");
			ASSET AddFbxForMesh("SE_CottonCandyTree_03_SM", "Assets\\SpringEnvironment\\Models\\SE_CottonCandyTree_03_SM.FBX");
			ASSET AddFbxForMesh("SE_FabricHouse_01_SM", "Assets\\SpringEnvironment\\Models\\SE_FabricHouse_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Factory_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Factory_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Fence_07a_SM", "Assets\\SpringEnvironment\\Models\\SE_Fence_07a_SM.FBX");
			ASSET AddFbxForMesh("SE_Fence_07b_SM", "Assets\\SpringEnvironment\\Models\\SE_Fence_07b_SM.FBX");
			ASSET AddFbxForMesh("SE_Fence_08a_SM", "Assets\\SpringEnvironment\\Models\\SE_Fence_08a_SM.FBX");
			ASSET AddFbxForMesh("SE_Fence_08b_SM", "Assets\\SpringEnvironment\\Models\\SE_Fence_08b_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_03_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_04_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_06_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_07_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_07_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_08_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_08_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_10_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_10_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_11_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_11_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_13_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_13_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_14_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_14_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_16_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_16_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_40_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_40_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_41_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_41_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_42_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_42_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_43_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_43_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_44_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_44_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_45_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_45_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_46_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_46_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_47_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_47_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_48_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_48_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_49_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_49_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_50_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_50_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_51_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_51_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_52_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_52_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_71_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_71_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_72_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_72_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_73_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_73_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_74_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_74_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_75_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_75_SM.FBX");
			ASSET AddFbxForMesh("SE_Flower_76_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_76_SM.FBX");
			ASSET AddFbxForMesh("SE_Gate_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_Gate_01b_SM.FBX");
			ASSET AddFbxForMesh("SE_Gazebo_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Gazebo_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Grill_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Grill_02_SM.FBX");
			ASSET AddFbxForMesh("SE_HeartHouse_01_SM", "Assets\\SpringEnvironment\\Models\\SE_HeartHouse_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Hedge_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Hedge_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Hedge_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_03_SM.FBX");
			ASSET AddFbxForMesh("SE_Hedge_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_04_SM.FBX");
			ASSET AddFbxForMesh("SE_Hedge_05_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_05_SM.FBX");
			ASSET AddFbxForMesh("SE_Hedge_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_06_SM.FBX");
			ASSET AddFbxForMesh("SE_Hedge_07_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_07_SM.FBX");
			ASSET AddFbxForMesh("SE_Hedge_08_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_08_SM.FBX");
			ASSET AddFbxForMesh("SE_HotAirBaloon_SM", "Assets\\SpringEnvironment\\Models\\SE_HotAirBaloon_SM.FBX");
			ASSET AddFbxForMesh("SE_House_01a_SM", "Assets\\SpringEnvironment\\Models\\SE_House_01a_SM.FBX");
			ASSET AddFbxForMesh("SE_House_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_House_01b_SM.FBX");
			ASSET AddFbxForMesh("SE_House_01c_SM", "Assets\\SpringEnvironment\\Models\\SE_House_01c_SM.FBX");
			ASSET AddFbxForMesh("SE_House_01d_SM", "Assets\\SpringEnvironment\\Models\\SE_House_01d_SM.FBX");
			ASSET AddFbxForMesh("SE_House_02a_SM", "Assets\\SpringEnvironment\\Models\\SE_House_02a_SM.FBX");
			ASSET AddFbxForMesh("SE_House_02b_SM", "Assets\\SpringEnvironment\\Models\\SE_House_02b_SM.FBX");
			ASSET AddFbxForMesh("SE_House_02c_SM", "Assets\\SpringEnvironment\\Models\\SE_House_02c_SM.FBX");
			ASSET AddFbxForMesh("SE_House_02d_SM", "Assets\\SpringEnvironment\\Models\\SE_House_02d_SM.FBX");
			ASSET AddFbxForMesh("SE_House_03d_SM", "Assets\\SpringEnvironment\\Models\\SE_House_03d_SM.FBX");
			ASSET AddFbxForMesh("SE_House_03a_SM", "Assets\\SpringEnvironment\\Models\\SE_House_03a_SM.FBX");
			ASSET AddFbxForMesh("SE_House_03b_SM", "Assets\\SpringEnvironment\\Models\\SE_House_03b_SM.FBX");
			ASSET AddFbxForMesh("SE_House_03c_SM", "Assets\\SpringEnvironment\\Models\\SE_House_03c_SM.FBX");
			ASSET AddFbxForMesh("SE_House_04a_SM", "Assets\\SpringEnvironment\\Models\\SE_House_04a_SM.FBX");
			ASSET AddFbxForMesh("SE_House_04b_SM", "Assets\\SpringEnvironment\\Models\\SE_House_04b_SM.FBX");
			ASSET AddFbxForMesh("SE_House_05a_SM", "Assets\\SpringEnvironment\\Models\\SE_House_05a_SM.FBX");
			ASSET AddFbxForMesh("SE_House_05b_SM", "Assets\\SpringEnvironment\\Models\\SE_House_05b_SM.FBX");
			ASSET AddFbxForMesh("SE_Lamp_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Lamp_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Lamp_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Lamp_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Maypole_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Maypole_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Present_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Present_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Present_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_03_SM.FBX");
			ASSET AddFbxForMesh("SE_Present_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_04_SM.FBX");
			ASSET AddFbxForMesh("SE_Present_05_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_05_SM.FBX");
			ASSET AddFbxForMesh("SE_Present_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_06_SM.FBX");
			ASSET AddFbxForMesh("SE_Pump_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Pump_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Rose_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Rose_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Rose_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_03_SM.FBX");
			ASSET AddFbxForMesh("SE_Rose_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_04_SM.FBX");
			ASSET AddFbxForMesh("SE_Rose_05_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_05_SM.FBX");
			ASSET AddFbxForMesh("SE_Rose_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_06_SM.FBX");
			ASSET AddFbxForMesh("SE_SeeSaw_01_SM", "Assets\\SpringEnvironment\\Models\\SE_SeeSaw_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Sign_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Sign_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Slide_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Slide_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Stone_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Stone_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Stone_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_03_SM.FBX");
			ASSET AddFbxForMesh("SE_Stone_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_04_SM.FBX");
			ASSET AddFbxForMesh("SE_Stone_05_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_05_SM.FBX");
			ASSET AddFbxForMesh("SE_Stone_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_06_SM.FBX");
			ASSET AddFbxForMesh("SE_Swing_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Swing_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Swing_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Swing_02_SM.FBX");
			ASSET AddFbxForMesh("SE_Table_01a_SM", "Assets\\SpringEnvironment\\Models\\SE_Table_01a_SM.FBX");
			ASSET AddFbxForMesh("SE_Table_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_Table_01b_SM.FBX");
			ASSET AddFbxForMesh("SE_Table_02a_SM", "Assets\\SpringEnvironment\\Models\\SE_Table_02a_SM.FBX");
			ASSET AddFbxForMesh("SE_Table_02b_SM", "Assets\\SpringEnvironment\\Models\\SE_Table_02b_SM.FBX");
			ASSET AddFbxForMesh("SE_Theater_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Theater_01_SM.FBX");
			ASSET AddFbxForMesh("SE_TownHall_SM", "Assets\\SpringEnvironment\\Models\\SE_TownHall_SM.FBX");
			ASSET AddFbxForMesh("SE_Trampoline_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Trampoline_01_SM.FBX");
			ASSET AddFbxForMesh("SE_Willow_01a_SM", "Assets\\SpringEnvironment\\Models\\SE_Willow_01a_SM.FBX");
			ASSET AddFbxForMesh("SE_Willow_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_Willow_01b_SM.FBX");


		}
		ASSET AddFbxForAnimation("ApprenticeSK", "Models\\modelTest.fbx");

		//*** Animation ***//
		ASSET AddFbxForAnimation("Walk_BowAnim", "Models\\BowStance\\Walk_BowAnim.fbx");
		ASSET AddFbxForAnimation("WalkBack_BowAnim", "Models\\BowStance\\WalkBack_BowAnim.fbx");
		ASSET AddFbxForAnimation("WalkRight_BowAnim", "Models\\BowStance\\WalkRight_BowAnim.fbx");
		ASSET AddFbxForAnimation("WalkLeft_BowAnim", "Models\\BowStance\\WalkLeft_BowAnim.fbx");
		ASSET AddFbxForAnimation("Idle_BowAnim", "Models\\BowStance\\Idle_BowAnim.fbx");
	}

	void LoadPrefab()
	{
		{
			auto prefab = CreateEmptyPrefab();
			auto collider = prefab->AddComponent<BoxCollider>();
			collider->obb = true;
			Vector3 boundMin{ FLT_MAX }, boundMax{ -FLT_MAX }, center, extents;
			BoundingBox bb0, bb1, bb2;
			{
				auto child = MakePrefab(ASSET MESH("AV_Well"), ASSET MATERIAL("AV_material_03"), prefab);
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				bb0 = ASSET MESH("AV_Well")->Bounds;
				center.xmf3 = bb0.Center; extents.xmf3 = bb0.Extents;
				boundMin = Vector3::Min(boundMin, center - extents);
				boundMax = Vector3::Max(boundMax, center + extents);
			}
			{
				auto child = MakePrefab(ASSET MESH("AV_Well_Extra02"), ASSET MATERIAL("AV_material_03"), prefab);
				child->transform->position = { 0.0f,1.5f,0.0f };
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				bb1 = ASSET MESH("AV_Well_Extra02")->Bounds;
				center.xmf3 = bb1.Center; extents.xmf3 = bb1.Extents;
				boundMin = Vector3::Min(boundMin, center - extents);
				boundMax = Vector3::Max(boundMax, center + extents);
			}
			{
				auto child = MakePrefab(ASSET MESH("AV_Well_Extra03"), ASSET MATERIAL("AV_material_03"), prefab);
				child->transform->position = { 0.0f,1.0f,0.0f };
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				bb2 = ASSET MESH("AV_Well_Extra03")->Bounds;
				center.xmf3 = bb2.Center; extents.xmf3 = bb2.Extents;
				boundMin = Vector3::Min(boundMin, center - extents);
				boundMax = Vector3::Max(boundMax, center + extents);
			}
			collider->center = (boundMax + boundMin) / 2;
			collider->extents = (boundMax - boundMin) / 2;

			ASSET AddPrefab("AV_Well", std::make_unique<GameObject>(prefab));
		}

		{
			auto prefab = CreateEmptyPrefab();
			auto collider = prefab->AddComponent<BoxCollider>();
			collider->extents = { 2.5f, 4.5f, 2.5f };
			collider->center.y += collider->extents.y * 0.5f;
			collider->obb = true;
			{
				auto child = MakePrefab(ASSET MESH("AV_House_02"), ASSET MATERIAL("AV_house_02"), prefab);
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			{
				auto child = MakePrefab(ASSET MESH("AV_House_02_Extra"), ASSET MATERIAL("AV_house_02"), prefab);
				child->AddComponent<RotatingBehavior>()->speedRotating = -10.0f;
				child->transform->position = { 0.25f,6.0f,2.5f };
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			ASSET AddPrefab("AV_House_01", std::make_unique<GameObject>(prefab));
		}

		{
			auto prefab = MakePrefab();
			auto collider = prefab->AddComponent<BoxCollider>()->obb;

			ASSET AddPrefab("MRC", std::make_unique<GameObject>(prefab));
		}


		{
			auto prefab = CreateEmptyPrefab();
			auto collider = prefab->AddComponent<BoxCollider>();
			BoundingBox bb = ASSET MESH("HP_Mill")->Bounds;
			collider->extents.xmf3 = bb.Extents;
			collider->center.xmf3 = bb.Center;
			collider->obb = true;
			{
				auto child = MakePrefab(ASSET MESH("HP_Mill"), ASSET MATERIAL("HP_Mill"), prefab);
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			{
				auto child = MakePrefab(ASSET MESH("HP_Mill_wings"), ASSET MATERIAL("HP_Mill"), prefab);
				child->AddComponent<RotatingBehavior>()->speedRotating = -10.0f;
				child->transform->position = { 0.0f,6.5f,6.5f };
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			ASSET AddPrefab("HP_Mill", std::make_unique<GameObject>(prefab));
		}


		{
			auto prefab = CreateEmptyPrefab();
			auto collider = prefab->AddComponent<BoxCollider>();
			BoundingBox bb = ASSET MESH("HP_street_light")->Bounds;
			collider->extents.xmf3 = bb.Extents;
			collider->center.xmf3 = bb.Center;
			collider->obb = true;
			{
				auto child = MakePrefab(ASSET MESH("HP_street_light"), ASSET MATERIAL("HP_Street_light"), prefab);
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			{
				auto spotLight = prefab->AddChild();
				spotLight->transform->position = { 0.0f, 2.5f, -1.6f };
				spotLight->AddComponent<Light>()->Strength = { 1, 1, 1 };
				spotLight->GetComponent<Light>()->type = Light::Type::Point;
				spotLight->GetComponent<Light>()->FalloffEnd = 5;
			}
			ASSET AddPrefab("HP_Street_light", std::make_unique<GameObject>(prefab));
		}

		{
			auto prefab = CreateEmptyPrefab();
			auto collider = prefab->AddComponent<BoxCollider>();
			BoundingBox bb = ASSET MESH("HP_Lamppost")->Bounds;
			collider->extents.xmf3 = bb.Extents;
			collider->center.xmf3 = bb.Center;
			collider->obb = true;
			{
				auto child = MakePrefab(ASSET MESH("HP_Lamppost"), ASSET MATERIAL("HP_Lampposts"), prefab);
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			{
				auto spotLight = prefab->AddChild();
				spotLight->transform->position = { 0.0f, 4.0f, 0.0f };
				spotLight->AddComponent<Light>()->Strength = { 1, 1, 1 };
				spotLight->GetComponent<Light>()->type = Light::Type::Point;
				spotLight->GetComponent<Light>()->FalloffEnd = 5;
			}
			ASSET AddPrefab("HP_Lamppost", std::make_unique<GameObject>(prefab));
		}


	}

	GameObject* MakePrefab(Mesh* mesh = nullptr, Material* material = nullptr, GameObject* parent = nullptr)
	{
		auto prefab = parent ? parent->AddChild() : CreateEmptyPrefab();
		prefab->AddComponent<MeshFilter>()->mesh = mesh;
		auto renderer = prefab->AddComponent<Renderer>();
		if (material)
			renderer->materials.push_back(material);
		return prefab;
	}
};
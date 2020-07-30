#pragma once
#include "Scene.h"

class HostScene : public Scene
{
public:
	HostScene() : Scene() {}
	~HostScene() {}

	virtual void BuildObjects();

	void LoadTextureAsset()
	{
		//ASSET AddTexture("none", L"Textures\\none.dds");
		ASSET AddTexture("ground", L"Textures\\grass.dds");
		ASSET AddTexture("grass", L"Texture\\grass.dds");

		ASSET AddTexture("polyArtTex", L"Textures\\PolyArtTex.dds");

		//ASSET AddTexture("fireTexD", L"Textures\\fire.dds");
		//ASSET AddTexture("smokeTexD", L"Textures\\smoke.dds");


		/*AdvancedVillagePack*/
		ASSET AddTexture("house01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_04_D.dds");
		ASSET AddTexture("house02", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_09_D.dds");
		ASSET AddTexture("material_01", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_01_D.dds");
		ASSET AddTexture("material_02", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_02_D.dds");
		ASSET AddTexture("material_03", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_03_D.dds");
		ASSET AddTexture("TreeLeafs", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_TreeLeafs_D.dds");
		ASSET AddTexture("TreeTrunks", L"Assets\\AdvancedVillagePack\\Textures\\T_Pack_TreeTrunks_D.dds");


		/*HandPaintedEnviroment*/
		//ASSET AddTexture("Baker_house", L"Assets\\HandPaintedEnvironment\\Textures\\Baker_house_texture.dds");
		//ASSET AddTexture("Barrel", L"Assets\\HandPaintedEnvironment\\Textures\\Barrel_texture.dds");
		//ASSET AddTexture("Boxes", L"Assets\\HandPaintedEnvironment\\Textures\\Boxes_texture.dds");
		//ASSET AddTexture("Cauldron", L"Assets\\HandPaintedEnvironment\\Textures\\Cauldron_texture.dds");
		//ASSET AddTexture("Chest", L"Assets\\HandPaintedEnvironment\\Textures\\Chest_texture.dds");
		//ASSET AddTexture("Crystals", L"Assets\\HandPaintedEnvironment\\Textures\\Crystals_texture.dds");
		//ASSET AddTexture("Fence", L"Assets\\HandPaintedEnvironment\\Textures\\Fence_texture.dds");
		//ASSET AddTexture("Fern", L"Assets\\HandPaintedEnvironment\\Textures\\Fern_texture.dds");
		//ASSET AddTexture("Forge", L"Assets\\HandPaintedEnvironment\\Textures\\Forge_texture.dds");
		//ASSET AddTexture("Hammer_anvil", L"Assets\\HandPaintedEnvironment\\Textures\\Hammer_anvil_texture.dds");
		//ASSET AddTexture("Lampposts", L"Assets\\HandPaintedEnvironment\\Textures\\Lampposts_texture.dds");
		//ASSET AddTexture("Large_house", L"Assets\\HandPaintedEnvironment\\Textures\\Large_house_texture.dds");
		//ASSET AddTexture("Log_stump_mushrooms", L"Assets\\HandPaintedEnvironment\\Textures\\Log_stump_mushrooms_texture.dds");
		//ASSET AddTexture("Logs", L"Assets\\HandPaintedEnvironment\\Textures\\Logs_texture.dds");
		//ASSET AddTexture("Mill", L"Assets\\HandPaintedEnvironment\\Textures\\Mill_texture.dds");
		//ASSET AddTexture("Mine", L"Assets\\HandPaintedEnvironment\\Textures\\Mine_texture.dds");
		//ASSET AddTexture("Pillars_arch", L"Assets\\HandPaintedEnvironment\\Textures\\Pillars_arch_texture.dds");
		//ASSET AddTexture("Pine_tree", L"Assets\\HandPaintedEnvironment\\Textures\\Pine_tree_texture.dds");
		//ASSET AddTexture("Pine_tree_2", L"Assets\\HandPaintedEnvironment\\Textures\\Pine_tree_2_texture.dds");
		//ASSET AddTexture("Plant", L"Assets\\HandPaintedEnvironment\\Textures\\Plant_texture.dds");
		//ASSET AddTexture("Plants", L"Assets\\HandPaintedEnvironment\\Textures\\Plants_texture.dds");
		//ASSET AddTexture("Rocks_1_2", L"Assets\\HandPaintedEnvironment\\Textures\\Rocks_1_2_texture.dds");
		//ASSET AddTexture("Rocks_3_4", L"Assets\\HandPaintedEnvironment\\Textures\\Rocks_3_4_texture.dds");
		//ASSET AddTexture("Ruined_tower", L"Assets\\HandPaintedEnvironment\\Textures\\Ruined_tower_texture.dds");
		//ASSET AddTexture("Ruined_walls", L"Assets\\HandPaintedEnvironment\\Textures\\Ruined_walls_texture.dds");
		//ASSET AddTexture("Small_house", L"Assets\\HandPaintedEnvironment\\Textures\\Small_house_texture.dds");
		//ASSET AddTexture("Stone_fence", L"Assets\\HandPaintedEnvironment\\Textures\\Stone_fence_texture.dds");
		//ASSET AddTexture("Street_light", L"Assets\\HandPaintedEnvironment\\Textures\\Street_light_texture.dds");
		//ASSET AddTexture("Stone_tiles", L"Assets\\HandPaintedEnvironment\\Textures\\Stone_tiles_texture.dds");
		//ASSET AddTexture("Tavern", L"Assets\\HandPaintedEnvironment\\Textures\\Tavern_texture.dds");
		//ASSET AddTexture("Town_Hall", L"Assets\\HandPaintedEnvironment\\Textures\\Town_Hall_texture.dds");
		//ASSET AddTexture("Tree_1_1", L"Assets\\HandPaintedEnvironment\\Textures\\Tree_1_1_texture.dds");
		//ASSET AddTexture("Tree_1_2", L"Assets\\HandPaintedEnvironment\\Textures\\Tree_1_2_texture.dds");
		//ASSET AddTexture("Tree_2_1", L"Assets\\HandPaintedEnvironment\\Textures\\Tree_2_1_texture.dds");
		//ASSET AddTexture("Tree_2_2", L"Assets\\HandPaintedEnvironment\\Textures\\Tree_2_2_texture.dds");
		//ASSET AddTexture("Trolley", L"Assets\\HandPaintedEnvironment\\Textures\\Trolley_texture.dds");
		//ASSET AddTexture("Well", L"Assets\\HandPaintedEnvironment\\Textures\\Well_texture.dds");
		//ASSET AddTexture("Witch_house", L"Assets\\HandPaintedEnvironment\\Textures\\Witch_house_texture.dds");
		//ASSET AddTexture("Woodmill", L"Assets\\HandPaintedEnvironment\\Textures\\Woodmill.dds");



		/*SpringEnvironment*/
		//ASSET AddTexture("SE_Baloons_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Baloons_Diffuse.dds");
		//ASSET AddTexture("SE_Bench_01a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Bench_01a_Diffuse.dds");
		//ASSET AddTexture("SE_Bench_01b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Bench_01b_Diffuse.dds");
		//ASSET AddTexture("SE_Birch_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Birch_Diffuse.dds");
		//ASSET AddTexture("SE_Birch_01b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Birch_01b_Diffuse.dds");
		//ASSET AddTexture("SE_Blanket_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Blanket_Diffuse.dds");
		//ASSET AddTexture("SE_Bushese_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Bushes_Diffuse.dds");
		//ASSET AddTexture("SE_Bonsai_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Bonsai_Diffuse.dds");
		//ASSET AddTexture("SE_Booth_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Booth_Diffuse.dds");
		//ASSET AddTexture("SE_Candies_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Candies_Diffuse.dds");
		//ASSET AddTexture("SE_Candyhouse_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Candyhouse_Diffuse.dds");
		//ASSET AddTexture("SE_Carousel_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Carousel_Diffuse.dds");
		//ASSET AddTexture("SE_Cart_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Cart_Diffuse.dds");
		//ASSET AddTexture("SE_Climbers_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Climbers_Diffuse.dds");
		//ASSET AddTexture("SE_CottonCandyTree_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_CottonCandyTree_Diffuse.dds");
		//ASSET AddTexture("SE_FabricHouse_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_FabricHouse_Diffuse.dds");
		//ASSET AddTexture("SE_Factory_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Factory_Diffuse.dds");
		//ASSET AddTexture("SE_FabricHouse_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_FabricHouse_Diffuse.dds");
		//ASSET AddTexture("SE_Fence_01a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Fence_01a_Diffuse.dds");
		//ASSET AddTexture("SE_Fence_01b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Fence_01b_Diffuse.dds");
		//ASSET AddTexture("SE_Flowers_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Flowers_Diffuse.dds");
		//ASSET AddTexture("SE_Gate_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Gate_Diffuse.dds");
		//ASSET AddTexture("SE_Gazebo_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Gazebo_Diffuse.dds");
		//ASSET AddTexture("SE_Grill_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Grill_Diffuse.dds");
		//ASSET AddTexture("SE_HeartHouse_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_HeartHouse_Diffuse.dds");
		//ASSET AddTexture("SE_Hedges_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Hedges_Diffuse.dds");
		//ASSET AddTexture("SE_HotAirBaloon_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_HotAirBaloon_Diffuse.dds");
		//ASSET AddTexture("SE_House_01a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_01a_Diffuse.dds");
		//ASSET AddTexture("SE_House_01b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_01b_Diffuse.dds");
		//ASSET AddTexture("SE_House_01c_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_01c_Diffuse.dds");
		//ASSET AddTexture("SE_House_01d_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_01d_Diffuse.dds");
		//ASSET AddTexture("SE_House_02a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_02a_Diffuse.dds");
		//ASSET AddTexture("SE_House_02b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_02b_Diffuse.dds");
		//ASSET AddTexture("SE_House_02c_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_02c_Diffuse.dds");
		//ASSET AddTexture("SE_House_02d_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_02d_Diffuse.dds");
		//ASSET AddTexture("SE_House_03d_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_03d_Diffuse.dds");
		//ASSET AddTexture("SE_House_03a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_03a_Diffuse.dds");
		//ASSET AddTexture("SE_House_03b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_03b_Diffuse.dds");
		//ASSET AddTexture("SE_House_03c_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_03c_Diffuse.dds");
		//ASSET AddTexture("SE_House_04a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_04a_Diffuse.dds");
		//ASSET AddTexture("SE_House_04b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_04b_Diffuse.dds");
		//ASSET AddTexture("SE_House_05a_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_05a_Diffuse.dds");
		//ASSET AddTexture("SE_House_05b_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_House_05b_Diffuse.dds");
		//ASSET AddTexture("SE_Lamps_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Lamps_Diffuse.dds");
		//ASSET AddTexture("SE_Maypole_01_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Maypole_01_Diffuse.dds");
		//ASSET AddTexture("SE_Presents_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Presents_Diffuse.dds");
		//ASSET AddTexture("SE_Pump_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Pump_Diffuse.dds");
		//ASSET AddTexture("SE_Roses_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Roses_Diffuse.dds");
		//ASSET AddTexture("SE_Seesaw_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Seesaw_Diffuse.dds");
		//ASSET AddTexture("SE_Sign_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Sign_Diffuse.dds");
		//ASSET AddTexture("SE_Slide_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Slide_Diffuse.dds");
		//ASSET AddTexture("SE_Stones_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Stones_Diffuse.dds");
		//ASSET AddTexture("SE_Swing_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Swing_Diffuse.dds");
		//ASSET AddTexture("SE_Tables_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Tables_Diffuse.dds");
		//ASSET AddTexture("SE_Theater_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Theater_Diffuse.dds");
		//ASSET AddTexture("SE_TownHall_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_TownHall_Diffuse.dds");
		//ASSET AddTexture("SE_Trampoline_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Trampoline_Diffuse.dds");
		//ASSET AddTexture("SE_Willow_Diffuse", L"Assets\\SpringEnvironment\\Textures\\SE_Willow_Diffuse.dds");
	}

	void LoadMaterialAsset()
	{
		ASSET AddMaterial("none", ASSET TEXTURE("none"));
		ASSET AddMaterial("gray", ASSET TEXTURE("none"), nullptr, { 0.5, 0.5, 0.5, 1.0 });
		ASSET AddMaterial("ground", ASSET TEXTURE("ground"), nullptr, { 0.48f, 0.64f, 0.2f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f, Matrix4x4::MatrixScaling(200, 200, 200));
		ASSET AddMaterial("grass", ASSET TEXTURE("grass"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.1f);

		ASSET AddMaterial("PolyArt", ASSET TEXTURE("polyArtTex"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);

		//ASSET AddMaterial("fireMat", ASSET TEXTURE("fireTexD"));
		//ASSET AddMaterial("smokeMat", ASSET TEXTURE("smokeTexD"));


		/*AdvancedVillagePack*/
		ASSET AddMaterial("house01", ASSET TEXTURE("house01"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("house02", ASSET TEXTURE("house02"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_01", ASSET TEXTURE("material_01"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_02", ASSET TEXTURE("material_02"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("material_03", ASSET TEXTURE("material_03"), nullptr, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.01f, 0.01f, 0.01f }, 0.9f);
		ASSET AddMaterial("TreeLeafs", ASSET TEXTURE("TreeLeafs"));
		ASSET AddMaterial("TreeTrunks", ASSET TEXTURE("TreeTrunks"));


		/*HandPaintedEnviroment*/
		//ASSET AddMaterial("Baker_house", ASSET TEXTURE("Baker_house"));
		//ASSET AddMaterial("Barrel", ASSET TEXTURE("Barrel"));
		//ASSET AddMaterial("Boxes", ASSET TEXTURE("Boxes"));
		//ASSET AddMaterial("Cauldron", ASSET TEXTURE("Cauldron"));
		//ASSET AddMaterial("Chest", ASSET TEXTURE("Chest"));
		//ASSET AddMaterial("Crystals", ASSET TEXTURE("Crystals"));
		//ASSET AddMaterial("Fence", ASSET TEXTURE("Fence"));
		//ASSET AddMaterial("Fern", ASSET TEXTURE("Fern"));
		//ASSET AddMaterial("Forge", ASSET TEXTURE("Forge"));
		//ASSET AddMaterial("Hammer_anvil", ASSET TEXTURE("Hammer_anvil"));
		//ASSET AddMaterial("Lampposts", ASSET TEXTURE("Lampposts"));
		//ASSET AddMaterial("Large_house", ASSET TEXTURE("Large_house"));
		//ASSET AddMaterial("Log_stump_mushrooms", ASSET TEXTURE("Log_stump_mushrooms"));
		//ASSET AddMaterial("Logs", ASSET TEXTURE("Logs"));
		//ASSET AddMaterial("Mill", ASSET TEXTURE("Mill"));
		//ASSET AddMaterial("Mine", ASSET TEXTURE("Mine"));
		//ASSET AddMaterial("Pillars_arch", ASSET TEXTURE("Pillars_arch"));
		//ASSET AddMaterial("Pine_tree", ASSET TEXTURE("Pine_tree"));
		//ASSET AddMaterial("Pine_tree_2", ASSET TEXTURE("Pine_tree_2"));
		//ASSET AddMaterial("Plant", ASSET TEXTURE("Plant"));
		//ASSET AddMaterial("Plants", ASSET TEXTURE("Plants"));
		//ASSET AddMaterial("Rocks_1_2", ASSET TEXTURE("Rocks_1_2"));
		//ASSET AddMaterial("Rocks_3_4", ASSET TEXTURE("Rocks_3_4"));
		//ASSET AddMaterial("Ruined_tower", ASSET TEXTURE("Ruined_tower"));
		//ASSET AddMaterial("Ruined_walls", ASSET TEXTURE("Ruined_walls"));
		//ASSET AddMaterial("Small_house", ASSET TEXTURE("Small_house"));
		//ASSET AddMaterial("Stone_fence", ASSET TEXTURE("Stone_fence"));
		//ASSET AddMaterial("Stone_tiles", ASSET TEXTURE("Stone_tiles"));
		//ASSET AddMaterial("Street_light", ASSET TEXTURE("Street_light"));
		//ASSET AddMaterial("Tavern", ASSET TEXTURE("Tavern"));
		//ASSET AddMaterial("Town_Hall", ASSET TEXTURE("Town_Hall"));
		//ASSET AddMaterial("Tree_1_1", ASSET TEXTURE("Tree_1_1"));
		//ASSET AddMaterial("Tree_1_2", ASSET TEXTURE("Tree_1_2"));
		//ASSET AddMaterial("Tree_2_1", ASSET TEXTURE("Tree_2_1"));
		//ASSET AddMaterial("Tree_2_2", ASSET TEXTURE("Tree_2_2"));
		//ASSET AddMaterial("Trolley", ASSET TEXTURE("Trolley"));
		//ASSET AddMaterial("Well", ASSET TEXTURE("Well"));
		//ASSET AddMaterial("Witch_house", ASSET TEXTURE("Witch_house"));
		//ASSET AddMaterial("Woodmill", ASSET TEXTURE("Woodmill"));


		/*SpringEnvironment*/
		//ASSET AddMaterial("SE_Baloons", ASSET TEXTURE("SE_Baloons_Diffuse"));
		//ASSET AddMaterial("SE_Bench_01a", ASSET TEXTURE("SE_Bench_01a_Diffuse"));
		//ASSET AddMaterial("SE_Bench_01b", ASSET TEXTURE("SE_Bench_01b_Diffuse"));
		//ASSET AddMaterial("SE_Birch", ASSET TEXTURE("SE_Birch_Diffuse"));
		//ASSET AddMaterial("SE_Birch_01b", ASSET TEXTURE("SE_Birch_01b_Diffuse"));
		//ASSET AddMaterial("SE_Blanket", ASSET TEXTURE("SE_Blanket_Diffuse"));
		//ASSET AddMaterial("SE_Bushese", ASSET TEXTURE("SE_Bushese_Diffuse"));
		//ASSET AddMaterial("SE_Bonsai", ASSET TEXTURE("SE_Bonsai_Diffuse"));
		//ASSET AddMaterial("SE_Booth", ASSET TEXTURE("SE_Booth_Diffuse"));
		//ASSET AddMaterial("SE_Candies", ASSET TEXTURE("SE_Candies_Diffuse"));
		//ASSET AddMaterial("SE_Candyhouse", ASSET TEXTURE("SE_Candyhouse_Diffuse"));
		//ASSET AddMaterial("SE_Carousel", ASSET TEXTURE("SE_Carousel_Diffuse"));
		//ASSET AddMaterial("SE_Cart", ASSET TEXTURE("SE_Cart_Diffuse"));
		//ASSET AddMaterial("SE_Climbers", ASSET TEXTURE("SE_Climbers_Diffuse"));
		//ASSET AddMaterial("SE_CottonCandyTree", ASSET TEXTURE("SE_CottonCandyTree_Diffuse"));
		//ASSET AddMaterial("SE_FabricHouse", ASSET TEXTURE("SE_FabricHouse_Diffuse"));
		//ASSET AddMaterial("SE_Factory", ASSET TEXTURE("SE_Factory_Diffuse"));
		//ASSET AddMaterial("SE_Fence_01a", ASSET TEXTURE("SE_Fence_01a_Diffuse"));
		//ASSET AddMaterial("SE_Fence_01b", ASSET TEXTURE("SE_Fence_01b_Diffuse"));
		//ASSET AddMaterial("SE_Flowers", ASSET TEXTURE("SE_Flowers_Diffuse"));
		//ASSET AddMaterial("SE_Gate", ASSET TEXTURE("SE_Gate_Diffuse"));
		//ASSET AddMaterial("SE_Gazebo", ASSET TEXTURE("SE_Gazebo_Diffuse"));
		//ASSET AddMaterial("SE_Grill", ASSET TEXTURE("SE_Grill_Diffuse"));
		//ASSET AddMaterial("SE_HeartHouse", ASSET TEXTURE("SE_HeartHouse_Diffuse"));
		//ASSET AddMaterial("SE_Hedges", ASSET TEXTURE("SE_Hedges_Diffuse"));
		//ASSET AddMaterial("SE_HotAirBaloon", ASSET TEXTURE("SE_HotAirBaloon_Diffuse"));
		//ASSET AddMaterial("SE_House_01a", ASSET TEXTURE("SE_House_01a_Diffuse"));
		//ASSET AddMaterial("SE_House_01b", ASSET TEXTURE("SE_House_01b_Diffuse"));
		//ASSET AddMaterial("SE_House_01c", ASSET TEXTURE("SE_House_01c_Diffuse"));
		//ASSET AddMaterial("SE_House_01d", ASSET TEXTURE("SE_House_01d_Diffuse"));
		//ASSET AddMaterial("SE_House_02a", ASSET TEXTURE("SE_House_02a_Diffuse"));
		//ASSET AddMaterial("SE_House_02b", ASSET TEXTURE("SE_House_02b_Diffuse"));
		//ASSET AddMaterial("SE_House_02c", ASSET TEXTURE("SE_House_02c_Diffuse"));
		//ASSET AddMaterial("SE_House_02d", ASSET TEXTURE("SE_House_02d_Diffuse"));
		//ASSET AddMaterial("SE_House_03d", ASSET TEXTURE("SE_House_03d_Diffuse"));
		//ASSET AddMaterial("SE_House_03a", ASSET TEXTURE("SE_House_03a_Diffuse"));
		//ASSET AddMaterial("SE_House_03b", ASSET TEXTURE("SE_House_03b_Diffuse"));
		//ASSET AddMaterial("SE_House_03c", ASSET TEXTURE("SE_House_03c_Diffuse"));
		//ASSET AddMaterial("SE_House_04a", ASSET TEXTURE("SE_House_04a_Diffuse"));
		//ASSET AddMaterial("SE_House_04b", ASSET TEXTURE("SE_House_04b_Diffuse"));
		//ASSET AddMaterial("SE_House_05a", ASSET TEXTURE("SE_House_05a_Diffuse"));
		//ASSET AddMaterial("SE_House_05b", ASSET TEXTURE("SE_House_05b_Diffuse"));
		//ASSET AddMaterial("SE_Lamps", ASSET TEXTURE("SE_Lamps_Diffuse"));
		//ASSET AddMaterial("SE_Maypole_01", ASSET TEXTURE("SE_Maypole_01_Diffuse"));
		//ASSET AddMaterial("SE_Presents", ASSET TEXTURE("SE_Presents_Diffuse"));
		//ASSET AddMaterial("SE_Pump", ASSET TEXTURE("SE_Pump_Diffuse"));
		//ASSET AddMaterial("SE_Roses", ASSET TEXTURE("SE_Roses_Diffuse"));
		//ASSET AddMaterial("SE_Seesaw", ASSET TEXTURE("SE_Seesaw_Diffuse"));
		//ASSET AddMaterial("SE_Sign", ASSET TEXTURE("SE_Sign_Diffuse"));
		//ASSET AddMaterial("SE_Slide", ASSET TEXTURE("SE_Slide_Diffuse"));
		//ASSET AddMaterial("SE_Stones", ASSET TEXTURE("SE_Stones_Diffuse"));
		//ASSET AddMaterial("SE_Swing", ASSET TEXTURE("SE_Swing_Diffuse"));
		//ASSET AddMaterial("SE_Tables", ASSET TEXTURE("SE_Tables_Diffuse"));
		//ASSET AddMaterial("SE_Theater", ASSET TEXTURE("SE_Theater_Diffuse"));
		//ASSET AddMaterial("SE_TownHall", ASSET TEXTURE("SE_TownHall_Diffuse"));
		//ASSET AddMaterial("SE_Trampoline", ASSET TEXTURE("SE_Trampoline_Diffuse"));
		//ASSET AddMaterial("SE_Willow", ASSET TEXTURE("SE_Willow_Diffuse"));
	}

	void LoadMeshAsset()
	{
		//ASSET AddMesh("Image", Mesh::CreateQuad());
		//ASSET AddMesh("Sphere", Mesh::CreateSphere());

		/*AdvancedVillagePack*/
		ASSET AddFbxForMesh("SM_Well", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well.FBX");
		ASSET AddFbxForMesh("SM_Well_Extra02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well_Extra02.FBX");
		ASSET AddFbxForMesh("SM_Well_Extra03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Well_Extra03.FBX");
		ASSET AddFbxForMesh("SM_House_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var01.FBX");
		ASSET AddFbxForMesh("SM_House_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02.FBX");
		ASSET AddFbxForMesh("SM_House_Var02_Extra", "Assets\\AdvancedVillagePack\\Meshes\\SM_House_Var02_Extra.FBX");
		ASSET AddFbxForMesh("SM_Tree_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var01.FBX");
		ASSET AddFbxForMesh("SM_Tree_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var02.FBX");
		ASSET AddFbxForMesh("SM_Tree_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var03.FBX");
		ASSET AddFbxForMesh("SM_Tree_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var04.FBX");
		ASSET AddFbxForMesh("SM_Tree_Var05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tree_Var05.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var01.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var02.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var03.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var04.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var05.FBX");
		ASSET AddFbxForMesh("SM_Mushroom_Var06", "Assets\\AdvancedVillagePack\\Meshes\\SM_Mushroom_Var06.FBX");
		ASSET AddFbxForMesh("SM_Cart_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var01.FBX");
		ASSET AddFbxForMesh("SM_Cart_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cart_Var02.FBX");
		ASSET AddFbxForMesh("SM_Barrel", "Assets\\AdvancedVillagePack\\Meshes\\SM_Barrel.FBX");
		ASSET AddFbxForMesh("SM_Bucket", "Assets\\AdvancedVillagePack\\Meshes\\SM_Bucket.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var01.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var02.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var03.FBX");
		ASSET AddFbxForMesh("SM_Fence_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fence_Var04.FBX");
		ASSET AddFbxForMesh("SM_Cauldron", "Assets\\AdvancedVillagePack\\Meshes\\SM_Cauldron.FBX");
		ASSET AddFbxForMesh("SM_Spike", "Assets\\AdvancedVillagePack\\Meshes\\SM_Spike.FBX");
		ASSET AddFbxForMesh("SM_Fish", "Assets\\AdvancedVillagePack\\Meshes\\SM_Fish.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var01.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var02.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var03", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var03.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var04", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var04.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var05", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var05.FBX");
		ASSET AddFbxForMesh("SM_Pot_Var06", "Assets\\AdvancedVillagePack\\Meshes\\SM_Pot_Var06.FBX");
		ASSET AddFbxForMesh("SM_Apple", "Assets\\AdvancedVillagePack\\Meshes\\SM_Apple.FBX");
		ASSET AddFbxForMesh("SM_Potato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Potato.FBX");
		ASSET AddFbxForMesh("SM_Tomato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Tomato.FBX");
		ASSET AddFbxForMesh("SM_Watermellon", "Assets\\AdvancedVillagePack\\Meshes\\SM_Watermellon.FBX");
		ASSET AddFbxForMesh("SM_Sack_Apple", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Apple.FBX");
		ASSET AddFbxForMesh("SM_Sack_Flour", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Flour.FBX");
		ASSET AddFbxForMesh("SM_Sack_Potato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Potato.FBX");
		ASSET AddFbxForMesh("SM_Sack_Tomato", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Tomato.FBX");
		ASSET AddFbxForMesh("SM_Sack_Var01", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Var01.FBX");
		ASSET AddFbxForMesh("SM_Sack_Var02", "Assets\\AdvancedVillagePack\\Meshes\\SM_Sack_Var02.FBX");

		/*HandPaintedEnviroment*/
		//ASSET AddFbxForMesh("Altar", "Assets\\HandPaintedEnvironment\\Models\\Altar.FBX");
		//ASSET AddFbxForMesh("Anvil", "Assets\\HandPaintedEnvironment\\Models\\Anvil.FBX");
		//ASSET AddFbxForMesh("Baker_house", "Assets\\HandPaintedEnvironment\\Models\\Baker_house.FBX");
		//ASSET AddFbxForMesh("Barrel", "Assets\\HandPaintedEnvironment\\Models\\Barrel.FBX");
		//ASSET AddFbxForMesh("Box_1", "Assets\\HandPaintedEnvironment\\Models\\Box_1.FBX");
		//ASSET AddFbxForMesh("Box_2", "Assets\\HandPaintedEnvironment\\Models\\Box_2.FBX");
		//ASSET AddFbxForMesh("Box_3", "Assets\\HandPaintedEnvironment\\Models\\Box_3.FBX");
		//ASSET AddFbxForMesh("Box_4", "Assets\\HandPaintedEnvironment\\Models\\Box_4.FBX");
		//ASSET AddFbxForMesh("Bush_1", "Assets\\HandPaintedEnvironment\\Models\\Bush_1.FBX");
		//ASSET AddFbxForMesh("Bush_2", "Assets\\HandPaintedEnvironment\\Models\\Bush_2.FBX");
		//ASSET AddFbxForMesh("Cauldron", "Assets\\HandPaintedEnvironment\\Models\\Cauldron.FBX");
		//ASSET AddFbxForMesh("Crystal_1", "Assets\\HandPaintedEnvironment\\Models\\Crystal_1.FBX");
		//ASSET AddFbxForMesh("Crystal_2", "Assets\\HandPaintedEnvironment\\Models\\Crystal_2.FBX");
		//ASSET AddFbxForMesh("Crystal_3", "Assets\\HandPaintedEnvironment\\Models\\Crystal_3.FBX");
		//ASSET AddFbxForMesh("Crystal_4", "Assets\\HandPaintedEnvironment\\Models\\Crystal_4.FBX");
		//ASSET AddFbxForMesh("Crystal_5", "Assets\\HandPaintedEnvironment\\Models\\Crystal_5.FBX");
		//ASSET AddFbxForMesh("Fence", "Assets\\HandPaintedEnvironment\\Models\\Fence.FBX");
		//ASSET AddFbxForMesh("Fence_1", "Assets\\HandPaintedEnvironment\\Models\\Fence_1.FBX");
		//ASSET AddFbxForMesh("Fence_2", "Assets\\HandPaintedEnvironment\\Models\\Fence_2.FBX");
		//ASSET AddFbxForMesh("Fern", "Assets\\HandPaintedEnvironment\\Models\\Fern.FBX");
		//ASSET AddFbxForMesh("Flower", "Assets\\HandPaintedEnvironment\\Models\\Flower.FBX");
		//ASSET AddFbxForMesh("Flowers_1", "Assets\\HandPaintedEnvironment\\Models\\Flowers_1.FBX");
		//ASSET AddFbxForMesh("Flowers_2", "Assets\\HandPaintedEnvironment\\Models\\Flowers_2.FBX");
		//ASSET AddFbxForMesh("Forge", "Assets\\HandPaintedEnvironment\\Models\\Forge.FBX");
		//ASSET AddFbxForMesh("Hammer", "Assets\\HandPaintedEnvironment\\Models\\Hammer.FBX");
		//ASSET AddFbxForMesh("Lamppost", "Assets\\HandPaintedEnvironment\\Models\\Lamppost.FBX");
		//ASSET AddFbxForMesh("Large_house", "Assets\\HandPaintedEnvironment\\Models\\Large_house.FBX");
		//ASSET AddFbxForMesh("Log", "Assets\\HandPaintedEnvironment\\Models\\Log.FBX");
		//ASSET AddFbxForMesh("Log_1", "Assets\\HandPaintedEnvironment\\Models\\Log_1.FBX");
		//ASSET AddFbxForMesh("Log_2", "Assets\\HandPaintedEnvironment\\Models\\Log_2.FBX");
		//ASSET AddFbxForMesh("Log_3", "Assets\\HandPaintedEnvironment\\Models\\Log_3.FBX");
		//ASSET AddFbxForMesh("Mill", "Assets\\HandPaintedEnvironment\\Models\\Mill.FBX");
		//ASSET AddFbxForMesh("Mill_wings", "Assets\\HandPaintedEnvironment\\Models\\Mill_wings.FBX");
		//ASSET AddFbxForMesh("Mine", "Assets\\HandPaintedEnvironment\\Models\\Mine.FBX");
		//ASSET AddFbxForMesh("Mushroom_1", "Assets\\HandPaintedEnvironment\\Models\\Mushroom_1.FBX");
		//ASSET AddFbxForMesh("Mushroom_2", "Assets\\HandPaintedEnvironment\\Models\\Mushroom_2.FBX");
		//ASSET AddFbxForMesh("Mushroom_3", "Assets\\HandPaintedEnvironment\\Models\\Mushroom_3.FBX");
		//ASSET AddFbxForMesh("Mushroom_4", "Assets\\HandPaintedEnvironment\\Models\\Mushroom_4.FBX");
		//ASSET AddFbxForMesh("Pillar", "Assets\\HandPaintedEnvironment\\Models\\Pillar.FBX");
		//ASSET AddFbxForMesh("Pillar_ruined_1", "Assets\\HandPaintedEnvironment\\Models\\Pillar_ruined_1.FBX");
		//ASSET AddFbxForMesh("Pine_tree", "Assets\\HandPaintedEnvironment\\Models\\Pine_tree.FBX");
		//ASSET AddFbxForMesh("Pine_tree_2", "Assets\\HandPaintedEnvironment\\Models\\Pine_tree_2.FBX");
		//ASSET AddFbxForMesh("Plant", "Assets\\HandPaintedEnvironment\\Models\\Plant.FBX");
		//ASSET AddFbxForMesh("Rock_1", "Assets\\HandPaintedEnvironment\\Models\\Rock_1.FBX");
		//ASSET AddFbxForMesh("Rock_2", "Assets\\HandPaintedEnvironment\\Models\\Rock_2.FBX");
		//ASSET AddFbxForMesh("Rock_3", "Assets\\HandPaintedEnvironment\\Models\\Rock_3.FBX");
		//ASSET AddFbxForMesh("Rock_4", "Assets\\HandPaintedEnvironment\\Models\\Rock_4.FBX");
		//ASSET AddFbxForMesh("Small_house", "Assets\\HandPaintedEnvironment\\Models\\Small_house.FBX");
		//ASSET AddFbxForMesh("Stone_fence", "Assets\\HandPaintedEnvironment\\Models\\Stone_fence.FBX");
		//ASSET AddFbxForMesh("street_light", "Assets\\HandPaintedEnvironment\\Models\\Street_light.FBX");
		//ASSET AddFbxForMesh("Stump", "Assets\\HandPaintedEnvironment\\Models\\Stump.FBX");
		//ASSET AddFbxForMesh("Tavern", "Assets\\HandPaintedEnvironment\\Models\\Tavern.FBX");
		//ASSET AddFbxForMesh("Tile_1", "Assets\\HandPaintedEnvironment\\Models\\Tile_1.FBX");
		//ASSET AddFbxForMesh("Tile_2", "Assets\\HandPaintedEnvironment\\Models\\Tile_2.FBX");
		//ASSET AddFbxForMesh("Tile_3", "Assets\\HandPaintedEnvironment\\Models\\Tile_3.FBX");
		//ASSET AddFbxForMesh("Tile_4", "Assets\\HandPaintedEnvironment\\Models\\Tile_4.FBX");
		//ASSET AddFbxForMesh("Tile_5", "Assets\\HandPaintedEnvironment\\Models\\Tile_5.FBX");
		//ASSET AddFbxForMesh("Tile_6", "Assets\\HandPaintedEnvironment\\Models\\Tile_6.FBX");
		//ASSET AddFbxForMesh("Tile_7", "Assets\\HandPaintedEnvironment\\Models\\Tile_7.FBX");
		//ASSET AddFbxForMesh("Tower", "Assets\\HandPaintedEnvironment\\Models\\Tower.FBX");
		//ASSET AddFbxForMesh("Town_Hall", "Assets\\HandPaintedEnvironment\\Models\\Town_Hall.FBX");
		//ASSET AddFbxForMesh("Tree_1", "Assets\\HandPaintedEnvironment\\Models\\Tree_1.FBX");
		//ASSET AddFbxForMesh("Tree_2", "Assets\\HandPaintedEnvironment\\Models\\Tree_2.FBX");
		//ASSET AddFbxForMesh("Tree_2_1", "Assets\\HandPaintedEnvironment\\Models\\Tree_2_1.FBX");
		//ASSET AddFbxForMesh("Tree_4", "Assets\\HandPaintedEnvironment\\Models\\Tree_4.FBX");
		//ASSET AddFbxForMesh("Wall_1", "Assets\\HandPaintedEnvironment\\Models\\Wall_1.FBX");
		//ASSET AddFbxForMesh("Wall_2", "Assets\\HandPaintedEnvironment\\Models\\Wall_2.FBX");
		//ASSET AddFbxForMesh("Well", "Assets\\HandPaintedEnvironment\\Models\\Well.FBX");
		//ASSET AddFbxForMesh("Witch_house", "Assets\\HandPaintedEnvironment\\Models\\Witch_house.FBX");
		//ASSET AddFbxForMesh("Woodmill", "Assets\\HandPaintedEnvironment\\Models\\Woodmill.FBX");
		//ASSET AddFbxForMesh("Woodmill_Saw", "Assets\\HandPaintedEnvironment\\Models\\Woodmill_Saw.FBX");


		/*SpringEnvironment*/
		//ASSET AddFbxForMesh("SE_Baloon_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Baloon_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Baloon_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Baloon_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Baloon_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Baloon_03_SM.FBX");
		//ASSET AddFbxForMesh("SE_Baloon_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Baloon_04_SM.FBX");
		//ASSET AddFbxForMesh("SE_Baloons_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Baloons_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Bench_01a_SM", "Assets\\SpringEnvironment\\Models\\SE_Bench_01a_SM.FBX");
		//ASSET AddFbxForMesh("SE_Bench_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_Bench_01b_SM.FBX");
		//ASSET AddFbxForMesh("SE_Birch_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Birch_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_01b_SM.FBX");
		//ASSET AddFbxForMesh("SE_Birch_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Birch_02b_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_02b_SM.FBX");
		//ASSET AddFbxForMesh("SE_Birch_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_03_SM.FBX");
		//ASSET AddFbxForMesh("SE_Birch_03b_SM", "Assets\\SpringEnvironment\\Models\\SE_Birch_03b_SM.FBX");
		//ASSET AddFbxForMesh("SE_Blanket_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Blanket_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Blanket_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Blanket_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Blossom_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Blossom_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Blossom_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Blossom_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Bonsai_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Bonsai_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Bonsai_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Bonsai_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Booth_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Booth_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Booth_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Booth_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Branch_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Branch_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Branch_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Branch_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Blossom_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Blossom_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Blossom_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Blossom_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Bush_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Bush_06_SM.FBX");
		//ASSET AddFbxForMesh("SE_Bush_07_SM", "Assets\\SpringEnvironment\\Models\\SE_Bush_07_SM.FBX");
		//ASSET AddFbxForMesh("SE_Candy_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Candy_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Candy_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Candy_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Candy_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Candy_03_SM.FBX");
		//ASSET AddFbxForMesh("SE_Candy_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Candy_04_SM.FBX");
		//ASSET AddFbxForMesh("SE_CandyHouse_01_SM", "Assets\\SpringEnvironment\\Models\\SE_CandyHouse_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Carousel_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Carousel_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Cart_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Cart_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Chair_01a_SM", "Assets\\SpringEnvironment\\Models\\SE_Chair_01a_SM.FBX");
		//ASSET AddFbxForMesh("SE_Climber_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Climber_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Climber_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Climber_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_CottonCandy_01_SM", "Assets\\SpringEnvironment\\Models\\SE_CottonCandy_01_SM.FBX");
		//ASSET AddFbxForMesh("E_CottonCandyTree_01_SM", "Assets\\SpringEnvironment\\Models\\SE_CottonCandyTree_01_SM.FBX");
		//ASSET AddFbxForMesh("E_CottonCandyTree_02_SM", "Assets\\SpringEnvironment\\Models\\SE_CottonCandyTree_02_SM.FBX");
		//ASSET AddFbxForMesh("E_CottonCandyTree_03_SM", "Assets\\SpringEnvironment\\Models\\SE_CottonCandyTree_03_SM.FBX");
		//ASSET AddFbxForMesh("SE_FabricHouse_01_SM", "Assets\\SpringEnvironment\\Models\\SE_FabricHouse_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Factory_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Factory_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Fence_07a_SM", "Assets\\SpringEnvironment\\Models\\SE_Fence_07a_SM.FBX");
		//ASSET AddFbxForMesh("SE_Fence_07b_SM", "Assets\\SpringEnvironment\\Models\\SE_Fence_07b_SM.FBX");
		//ASSET AddFbxForMesh("SE_Fence_08a_SM", "Assets\\SpringEnvironment\\Models\\SE_Fence_08a_SM.FBX");
		//ASSET AddFbxForMesh("SE_Fence_08b_SM", "Assets\\SpringEnvironment\\Models\\SE_Fence_08b_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_03_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_04_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_06_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_07_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_07_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_08_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_08_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_10_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_10_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_11_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_11_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_13_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_13_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_14_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_14_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_16_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_16_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_40_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_40_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_41_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_41_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_42_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_42_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_43_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_43_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_44_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_44_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_45_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_45_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_46_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_46_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_47_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_47_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_48_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_48_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_49_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_49_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_50_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_50_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_51_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_51_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_52_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_52_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_71_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_71_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_72_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_72_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_73_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_73_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_74_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_74_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_75_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_75_SM.FBX");
		//ASSET AddFbxForMesh("SE_Flower_76_SM", "Assets\\SpringEnvironment\\Models\\SE_Flower_76_SM.FBX");
		//ASSET AddFbxForMesh("SE_Gate_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_Gate_01b_SM.FBX");
		//ASSET AddFbxForMesh("SE_Gazebo_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Gazebo_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Grill_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Grill_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_HeartHouse_01_SM", "Assets\\SpringEnvironment\\Models\\SE_HeartHouse_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Hedge_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Hedge_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Hedge_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_03_SM.FBX");
		//ASSET AddFbxForMesh("SE_Hedge_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_04_SM.FBX");
		//ASSET AddFbxForMesh("SE_Hedge_05_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_05_SM.FBX");
		//ASSET AddFbxForMesh("SE_Hedge_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_06_SM.FBX");
		//ASSET AddFbxForMesh("SE_Hedge_07_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_07_SM.FBX");
		//ASSET AddFbxForMesh("SE_Hedge_08_SM", "Assets\\SpringEnvironment\\Models\\SE_Hedge_08_SM.FBX");
		//ASSET AddFbxForMesh("SE_HotAirBaloon_SM", "Assets\\SpringEnvironment\\Models\\SE_HotAirBaloon_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_01a_SM", "Assets\\SpringEnvironment\\Models\\SE_House_01a_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_House_01b_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_01c_SM", "Assets\\SpringEnvironment\\Models\\SE_House_01c_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_01d_SM", "Assets\\SpringEnvironment\\Models\\SE_House_01d_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_02a_SM", "Assets\\SpringEnvironment\\Models\\SE_House_02a_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_02b_SM", "Assets\\SpringEnvironment\\Models\\SE_House_02b_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_02c_SM", "Assets\\SpringEnvironment\\Models\\SE_House_02c_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_02d_SM", "Assets\\SpringEnvironment\\Models\\SE_House_02d_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_03d_SM", "Assets\\SpringEnvironment\\Models\\SE_House_03d_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_03a_SM", "Assets\\SpringEnvironment\\Models\\SE_House_03a_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_03b_SM", "Assets\\SpringEnvironment\\Models\\SE_House_03b_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_03c_SM", "Assets\\SpringEnvironment\\Models\\SE_House_03c_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_04a_SM", "Assets\\SpringEnvironment\\Models\\SE_House_04a_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_04b_SM", "Assets\\SpringEnvironment\\Models\\SE_House_04b_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_05a_SM", "Assets\\SpringEnvironment\\Models\\SE_House_05a_SM.FBX");
		//ASSET AddFbxForMesh("SE_House_05b_SM", "Assets\\SpringEnvironment\\Models\\SE_House_05b_SM.FBX");
		//ASSET AddFbxForMesh("SE_Lamp_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Lamp_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Lamp_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Lamp_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Maypole_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Maypole_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Present_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Present_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Present_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_03_SM.FBX");
		//ASSET AddFbxForMesh("SE_Present_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_04_SM.FBX");
		//ASSET AddFbxForMesh("SE_Present_05_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_05_SM.FBX");
		//ASSET AddFbxForMesh("SE_Present_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Present_06_SM.FBX");
		//ASSET AddFbxForMesh("SE_Pump_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Pump_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Rose_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Rose_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Rose_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_03_SM.FBX");
		//ASSET AddFbxForMesh("SE_Rose_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_04_SM.FBX");
		//ASSET AddFbxForMesh("SE_Rose_05_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_05_SM.FBX");
		//ASSET AddFbxForMesh("SE_Rose_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Rose_06_SM.FBX");
		//ASSET AddFbxForMesh("SE_SeeSaw_01_SM", "Assets\\SpringEnvironment\\Models\\SE_SeeSaw_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Sign_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Sign_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Slide_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Slide_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Stone_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Stone_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Stone_03_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_03_SM.FBX");
		//ASSET AddFbxForMesh("SE_Stone_04_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_04_SM.FBX");
		//ASSET AddFbxForMesh("SE_Stone_05_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_05_SM.FBX");
		//ASSET AddFbxForMesh("SE_Stone_06_SM", "Assets\\SpringEnvironment\\Models\\SE_Stone_06_SM.FBX");
		//ASSET AddFbxForMesh("SE_Swing_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Swing_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Swing_02_SM", "Assets\\SpringEnvironment\\Models\\SE_Swing_02_SM.FBX");
		//ASSET AddFbxForMesh("SE_Table_01a_SM", "Assets\\SpringEnvironment\\Models\\SE_Table_01a_SM.FBX");
		//ASSET AddFbxForMesh("SE_Table_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_Table_01b_SM.FBX");
		//ASSET AddFbxForMesh("SE_Table_02a_SM", "Assets\\SpringEnvironment\\Models\\SE_Table_02a_SM.FBX");
		//ASSET AddFbxForMesh("SE_Table_02b_SM", "Assets\\SpringEnvironment\\Models\\SE_Table_02b_SM.FBX");
		//ASSET AddFbxForMesh("SE_Theater_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Theater_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_TownHall_SM", "Assets\\SpringEnvironment\\Models\\SE_TownHall_SM.FBX");
		//ASSET AddFbxForMesh("SE_Trampoline_01_SM", "Assets\\SpringEnvironment\\Models\\SE_Trampoline_01_SM.FBX");
		//ASSET AddFbxForMesh("SE_Willow_01a_SM", "Assets\\SpringEnvironment\\Models\\SE_Willow_01a_SM.FBX");
		//ASSET AddFbxForMesh("SE_Willow_01b_SM", "Assets\\SpringEnvironment\\Models\\SE_Willow_01b_SM.FBX");


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
				auto child = MakePrefab(ASSET MESH("SM_Well"), ASSET MATERIAL("material_03"), prefab);
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				bb0 = ASSET MESH("SM_Well")->Bounds;
				center.xmf3 = bb0.Center; extents.xmf3 = bb0.Extents;
				boundMin = Vector3::Min(boundMin, center - extents);
				boundMax = Vector3::Max(boundMax, center + extents);
			}
			{
				auto child = MakePrefab(ASSET MESH("SM_Well_Extra02"), ASSET MATERIAL("material_03"), prefab);
				child->transform->position = { 0.0f,1.5f,0.0f };
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				bb1 = ASSET MESH("SM_Well_Extra02")->Bounds;
				center.xmf3 = bb1.Center; extents.xmf3 = bb1.Extents;
				boundMin = Vector3::Min(boundMin, center - extents);
				boundMax = Vector3::Max(boundMax, center + extents);
			}
			{
				auto child = MakePrefab(ASSET MESH("SM_Well_Extra03"), ASSET MATERIAL("material_03"), prefab);
				child->transform->position = { 0.0f,1.0f,0.0f };
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
				bb2 = ASSET MESH("SM_Well_Extra03")->Bounds;
				center.xmf3 = bb2.Center; extents.xmf3 = bb2.Extents;
				boundMin = Vector3::Min(boundMin, center - extents);
				boundMax = Vector3::Max(boundMax, center + extents);
			}
			collider->center = (boundMax + boundMin) / 2;
			collider->extents = (boundMax - boundMin) / 2;

			ASSET AddPrefab("Well", std::make_unique<GameObject>(prefab));
		}

		{
			auto prefab = CreateEmptyPrefab();
			auto collider = prefab->AddComponent<BoxCollider>();
			collider->extents = { 2.5f, 4.5f, 2.5f };
			collider->center.y += collider->extents.y * 0.5f;
			collider->obb = true;
			{
				auto child = MakePrefab(ASSET MESH("SM_House_Var02"), ASSET MATERIAL("house02"), prefab);
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			{
				auto child = MakePrefab(ASSET MESH("SM_House_Var02_Extra"), ASSET MATERIAL("house02"), prefab);
				child->AddComponent<RotatingBehavior>()->speedRotating = -10.0f;
				child->transform->position = { 0.25f,6.0f,2.5f };
				child->transform->Rotate({ 1.0,0.0,0.0 }, -90.0f);
			}
			ASSET AddPrefab("House01", std::make_unique<GameObject>(prefab));
		}

		{
			auto prefab = MakePrefab();
			auto collider = prefab->AddComponent<BoxCollider>()->obb;

			ASSET AddPrefab("MRC", std::make_unique<GameObject>(prefab));
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
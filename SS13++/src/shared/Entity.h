#pragma once



// Examples:
// Human:
//		Entity -> DynamicEntity -> LiveEntity -> PlayableEntity
//		Parts:
//			- Organs: Part -> OrganicPart -> VitalOrgan -> [Wathever]
//			- Limbs: Part -> OrganicPart -> Limb -> [LimbWhatever]
//			- Cover: Part -> OrganicPart -> Cover -> [Skin]
//			- Clothes: Part -> InventoryPart -> Clothes
//			- Controller: Part -> NetPart -> PlayerNetPart

// Airlock:
//		Entity -> DynamicEntity -> MachineEntity
//		Parts:
//			- CircuitBoard: Part -> CircuitPart
//				ElectricPart -> DoorController
//				ElectricPart -> Timer
//				ElectricPart -> ElectricInput
//			- Movable: Part -> DoorPart
//			- Button: Part -> Interactable -> Button

// Entity
// |
// |---- DynamicEntity (Interactable, Movable)
//     | - LiveEntity
//     | - MachineEntity
//     | - StorageEntity
// |---- ItemEntity (Can be stored in inventories)
// |---- GlobalEntity
// etc...

// Part
// |
// |---- CircuitPart
// |---- DoorPart
// |---- OrganicPart
//     | - OrganPart
//     | - LimbPart
// etc...


// Base Part. Does absolutely nothing.
class Part
{
public:

	Part() {}
	~Part() {}
};

// Base Entity. Does absolutely nothing.
class Entity
{
public:

	Entity() {}
	~Entity() {}
};


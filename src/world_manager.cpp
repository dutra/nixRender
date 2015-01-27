#include <iostream>
#include "world_manager.h"

WorldManager::WorldManager() {

}
WorldManager::~WorldManager() {

}
uuid WorldManager::createEntity() {
    // uuid id = _entities.size();
    // Entity entity;
    // entity.id = id;
    // _entities.add(entity);
    // return id;
    return 0;
}
void WorldManager::addComponent(uuid id, Component component) {
    std::cout << "Adding Component " << static_cast<uint32>(component) << " to id " << id;


}

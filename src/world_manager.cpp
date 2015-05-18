#include <iostream>
#include "world_manager.h"
#include "world_types.h"

WorldManager::WorldManager() {

}
WorldManager::~WorldManager() {

}
uuid WorldManager::createEntity() {
    //Entity entity;
    uuid entity_id = 0;

    /*auto update_id = [&] (Entity& element, std::size_t id) -> void { element.id = id; entity_id = id; };
     _entities.add(entity, update_id);
*/
     //StaticComponent sc;
     //_static_components.insert()

    return entity_id;
}
void WorldManager::addComponent(uuid id, Component component) {
    std::cout << "Adding Component " << static_cast<uint32>(component) << " to id " << id;

    switch (component) {
        case Component::STATIC_COMPONENT:
            //_static_components[id] =
            break;
        case Component::DRAW_COMPONENT:
            break;
    }

}

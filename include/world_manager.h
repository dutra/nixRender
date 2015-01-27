#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <vector>
#include <glm/glm.hpp>
#include "types.h"
#include "world_types.h"
#include "optional_vector.h"

///  Manages World Entities
/** (e.g.: Entities, Components)
 *
 */
class WorldManager {

public:
    WorldManager();
    ~WorldManager();

    /** Creates entity
      * @return the newly created entity UUID
      */
    uuid createEntity();

    /** Adds component to entity with UUID id
    * @param id The id of the entity
    * @param component The component to be added to entity
    */
    void addComponent(uuid id, Component component);

private:
    OptionalVector<Entity> _entities;
    OptionalVector<bool> _static_components;
    //OptionalVector<bool> _draw_components;
};

#endif /* WORLD_MANAGER_H */

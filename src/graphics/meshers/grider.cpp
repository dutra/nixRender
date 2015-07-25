
#include "grider.h"


void Grider::init(std::function<float(glm::vec3)> density) {

    m_density = density;

    m_grid = new Gridcell**[GRID_TOTAL_SIZE_X];
    for (int i = 0; i < GRID_TOTAL_SIZE_X; i++) {
        m_grid[i] = new Gridcell*[GRID_TOTAL_SIZE_Y];
        for (int j = 0; j < GRID_TOTAL_SIZE_Y; j++) {
            m_grid[i][j] = new Gridcell[GRID_TOTAL_SIZE_Z];
        }
    }
    fillGrid();
}

void Grider::fillGrid() {


    for (int i = 0; i < GRID_NEAR_SIZE_X; i++) {
        for (int j = 0; j < GRID_NEAR_SIZE_Y; j++) {
            for (int k = 0; k < GRID_NEAR_SIZE_Z; k++) {
                Gridcell& cell = m_grid[i][j][k];
                cell.p[0] = glm::vec3(0, 0, 0);
                cell.p[1] = glm::vec3(1, 0, 0);
                cell.p[2] = glm::vec3(1, 0, 1);
                cell.p[3] = glm::vec3(0, 0, 1);
                cell.p[4] = glm::vec3(0, 1, 0);
                cell.p[5] = glm::vec3(1, 1, 0);
                cell.p[6] = glm::vec3(1, 1, 1);
                cell.p[7] = glm::vec3(0, 1, 1);

                for (int u = 0; u < 8; u++) {
                    cell.p[u] += glm::vec3(i, j, k);
                    cell.val[u] = m_density(cell.p[u]);
                    //                    std::cout << "(" << cell.p[u].x << "," << cell.p[u].y << "," << cell.p[u].z << "): " << cell.val[u] << " ";
                }
            }
        }
    }


    for (int i = 0; i < GRID_MEDIUM_SIZE_X*GRID_MEDIUM_FACTOR_X; i += GRID_MEDIUM_FACTOR_X) {
        for (int j = 0; j < GRID_MEDIUM_SIZE_Y*GRID_MEDIUM_FACTOR_Y; j += GRID_MEDIUM_FACTOR_Y) {
            for (int k = 0; k < GRID_MEDIUM_SIZE_Z*GRID_MEDIUM_FACTOR_Z; k += GRID_MEDIUM_FACTOR_Z) {
                if (i < GRID_NEAR_SIZE_X && j < GRID_NEAR_SIZE_Y && k < GRID_NEAR_SIZE_Z)
                    continue;
                Gridcell& cell = m_grid[GRID_NEAR_SIZE_X + i / GRID_MEDIUM_FACTOR_X][GRID_NEAR_SIZE_Y + j / GRID_MEDIUM_FACTOR_Y][GRID_NEAR_SIZE_Z + k / GRID_MEDIUM_FACTOR_Z];
                cell.p[0] = glm::vec3(0, 0, 0);
                cell.p[1] = glm::vec3(1, 0, 0);
                cell.p[2] = glm::vec3(1, 0, 1);
                cell.p[3] = glm::vec3(0, 0, 1);
                cell.p[4] = glm::vec3(0, 1, 0);
                cell.p[5] = glm::vec3(1, 1, 0);
                cell.p[6] = glm::vec3(1, 1, 1);
                cell.p[7] = glm::vec3(0, 1, 1);

                for (int u = 0; u < 8; u++) {
                    cell.p[u] *= 4.0;
                    cell.p[u] += glm::vec3(i, j, k);
                    cell.val[u] = m_density(cell.p[u]);
                    //                    std::cout << "(" << cell.p[u].x << "," << cell.p[u].y << "," << cell.p[u].z << "): " << cell.val[u] << " ";
                }
            }
        }
    }

}

Gridcell*** Grider::getGrid() {
    return m_grid;
}

Grider::~Grider() {
    for (int i = 0; i < GRID_TOTAL_SIZE_X; i++) {
        for (int j = 0; j < GRID_TOTAL_SIZE_Y; j++) {
            delete m_grid[i][j];
        }
        delete m_grid[i];
    }
    delete m_grid;
}
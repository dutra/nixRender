
#include "grider.h"


void Grider::init(std::function<float(glm::vec3)> density) {

    m_density = density;

    m_grid = new Gridcell**[32];
    for (int i = 0; i < 32; i++) {
        m_grid[i] = new Gridcell*[32];
        for (int j = 0; j < 32; j++) {
            m_grid[i][j] = new Gridcell[32];
        }
    }
    fillGrid();
}

void Grider::fillGrid() {


    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            for (int k = 0; k < 32; k++) {
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
}

Gridcell*** Grider::getGrid() {
    return m_grid;
}

Grider::~Grider() {
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            delete m_grid[i][j];
        }
        delete m_grid[i];
    }
    delete m_grid;
}
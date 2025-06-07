#include "tipe.h"

void definir_station(igraph_t *g, Station* stations) {
    for(int i = 0; i < g->n; i++) {
        igraph_cattribute_VAN_set(g, "station", i, (int)stations[i]);
    }
}

Station get_station_status(igraph_t *g, int i) {
    assert(i >= 0 && i < g->n);
    int id = igraph_cattribute_VAN(g, "station", i);
    return (Station)id;
}

void attribuer_stations(igraph_t *g) {
    int *centres = calloc(K, sizeof(int));
    kmedian(g, K, centres);
}

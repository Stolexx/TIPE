#include "tipe.h"

int main(void) {
    //Graph graphe = get_colorado_graph();
    Graph graphe = get_random_graph(10);
    attribuer_stations(&graphe);
    afficher_graphe(&graphe, "graphe.dot");
    simulation(graphe, 50);
    return EXIT_SUCCESS;
}

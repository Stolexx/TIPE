
#include "tipe.h"
#include <float.h>

double distance(Graph *g, int i, int j) {
    double total = 0.0;
    if(i == j) return 0.0;

    igraph_vector_t poids;
    igraph_vector_int_t sommets;
    igraph_es_t aretes;

    igraph_vector_init(&poids, 0);
    igraph_es_all(&aretes, IGRAPH_EDGEORDER_ID);
    igraph_cattribute_EANV(g, ATTR_WEIGHT, aretes, &poids);
    igraph_vector_int_init(&sommets, 0);

    if(igraph_get_shortest_path_dijkstra(g, &sommets, NULL, i, j, &poids, IGRAPH_OUT) != IGRAPH_SUCCESS) {
        fprintf(stderr, "Erreur : pas de chemin entre %d et %d\n", i, j);
        igraph_vector_destroy(&poids);
        return +DBL_MAX;
    }

    for(int s = 0; s < igraph_vector_int_size(&sommets) - 1; s++) {
        int sommet = (int)VECTOR(sommets)[s];
        total += get_edge_attribute(g, get_edge_id(g, sommet, (int)VECTOR(sommets)[s+1]), ATTR_WEIGHT);
    }

    igraph_vector_destroy(&poids);
    igraph_vector_int_destroy(&sommets);
    igraph_es_destroy(&aretes);

    return total;
}

double cost(Graph *g, int *centres, int k) {
    if (k == 0) return +DBL_MAX;

    double total = 0.0;
    for(int s = 0; s < vertices_count(g); s++) {
        double dist_min = +DBL_MAX;
        for(int c = 0; c < k; c++) {
            double d = distance(g, s, centres[c]);
            if(d < dist_min) {
                dist_min = d;
            }
        }
        if(dist_min == +DBL_MAX) {
            fprintf(stderr, "Erreur : sommet %d non connecté à un centre.\n", s);
            return -1;
        }
        total += dist_min;
    }
    return total;
}

double gain(Graph *g, int candidate, int *centres, int old_k) {
    double cost_avant = cost(g, centres, old_k);
    double cost_apres = 0.0;

    int old_val = centres[old_k];
    centres[old_k] = candidate;
    cost_apres = cost(g, centres, old_k+1);
    centres[old_k] = old_val;

    return cost_avant - cost_apres; /* Si le gain est négatif y'a un problème */
}

void local_search(igraph_t *g, int k, int *centres) {
    int *centres_final = malloc(k * sizeof(int));
    for(int i = 0; i < k; i++) {
        centres_final[i] = centres[i];
    }

    bool continuer = true;
    while(continuer) {
        continuer = false;
        for(int c = 0; c < k; c++) {
            for(int s = 0; s < vertices_count(g); s++) {
                // On vérifie que le sommet n'est pas déjà un centre
                for(int i = 0; i < k; i++) {
                    if(centres_final[i] == s) continue;
                }

                int *nouveaux_centres = malloc(k * sizeof(int));
                for(int i = 0; i < k; i++) {
                    nouveaux_centres[i] = centres_final[i];
                }
                nouveaux_centres[c] = s;

                double avant = cost(g, centres_final, k);
                double apres = cost(g, nouveaux_centres, k);
                if(apres < avant) {
                    printf("Amélioration trouvée : %f -> %f\n", avant, apres);
                    set_vertix_attribute(g, centres_final[c], ATTR_STATION, NORMAL);
                    set_vertix_attribute(g, s, ATTR_STATION, CHARGEUR);
                    centres_final[c] = s;
                    continuer = true;
                    break;
                }
                free(nouveaux_centres);
            }
        }
    }

    for(int i = 0; i < k; i++) {
        centres[i] = centres_final[i];
    }
    free(centres_final);
}

void kmedian_greedy(Graph *g, int k, int *centres) {
    int nb_centres = 0;
    while(nb_centres < k) {
        int best_node = -1;
        double best_gain = -DBL_MAX;
        for(int s = 0; s < vertices_count(g); s++) {
            if(get_station_status(g, s) == NORMAL) {
                double gnv = gain(g, s, centres, nb_centres);
                if(gnv > best_gain) {
                    best_gain = gnv;
                    best_node = s;
                }
            }
        }
        printf("Sommet sélectionné : %d (gain : %f)\n", best_node, best_gain);
        centres[nb_centres] = best_node;
        set_vertix_attribute(g, best_node, ATTR_STATION, CHARGEUR);
        nb_centres++;
    }
}

void kmedian(igraph_t *g, int k, int *centres) {
    printf("\nDÉBUT GLOUTON\n\n");
    kmedian_greedy(g, k, centres);
    for(int i = 0; i < k; i++) {
        printf("Centre %d : %d\n", i, centres[i]);
    }
    printf("Coût après glouton : %f\n", cost(g, centres, k));
    printf("\nDÉBUT RECHERCHE LOCALE\n\n");
    local_search(g, k, centres);
    for(int i = 0; i < k; i++) {
        printf("Centre %d : %d\n", i, centres[i]);
    }
    printf("Coût après glouton + recherche locale : %f\n", cost(g, centres, k));
}

#include "tipe.h"

// ---- OUTILS ----

Graph init(int nb_sommets) {
    igraph_t g;
    igraph_empty(&g, nb_sommets, IGRAPH_UNDIRECTED);
    return g;
}

Graph random_init(int nb_sommets) {
    igraph_t g;
    bool connexe = false;
    int nb_arcs = nb_sommets*1.25;

    igraph_rng_seed(igraph_rng_default(), (unsigned long)time(NULL));
    igraph_set_attribute_table(&igraph_cattribute_table);

    while (!connexe) {
        if (igraph_erdos_renyi_game(&g, IGRAPH_ERDOS_RENYI_GNM, nb_sommets, nb_arcs, IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS) != IGRAPH_SUCCESS) {
            fprintf(stderr, "Erreur : impossible de créer le graphe aléatoire.\n");
            exit(EXIT_FAILURE);
        }

        if (igraph_is_connected(&g, &connexe, IGRAPH_WEAK) != IGRAPH_SUCCESS) {
            fprintf(stderr, "Erreur : impossible de vérifier la connexité du graphe.\n");
            igraph_destroy(&g);
            exit(EXIT_FAILURE);
        }

        if (!connexe) {
            igraph_destroy(&g);
        }
    }
    return g;
}

int vertices_count(Graph *g) {
    return g->n;
}

int edges_count(Graph *g) {
    return igraph_ecount(g);
}

int get_edge_id(Graph *g, int a, int b) {
    igraph_integer_t eid;
    if (igraph_get_eid(g, &eid, a, b, IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS) != IGRAPH_SUCCESS) {
        fprintf(stderr, "Erreur : impossible de trouver l'arête entre %d et %d.\n", a, b);
        return -1;
    }
    return (int)eid;
}

void add_edge(Graph *g, int a, int b, double weight) {
    if (get_edge_id(g, a, b) != -1) {
        fprintf(stderr, "Erreur : l'arête entre %d et %d existe déjà.\n", a, b);
        return;
    }
    if (igraph_add_edge(g, a, b) != IGRAPH_SUCCESS) {
        fprintf(stderr, "Erreur : impossible d'ajouter l'arête entre %d et %d.\n", a, b);
        return;
    }
    igraph_integer_t eid = get_edge_id(g, a, b);
    if (eid == -1) {
        fprintf(stderr, "Erreur : impossible de trouver l'identifiant de l'arête entre %d et %d.\n", a, b);
        return;
    }
    set_edge_attribute(g, eid, ATTR_WEIGHT, weight);
}

void get_edge_vertices(Graph *g, int edge_id, int *from, int *to) {
    igraph_integer_t from_id, to_id;
    if (igraph_edge(g, edge_id, &from_id, &to_id) != IGRAPH_SUCCESS) {
        fprintf(stderr, "Erreur : impossible de trouver les sommets de l'arête %d.\n", edge_id);
        return;
    }
    *from = (int)from_id;
    *to = (int)to_id;
}

void set_vertix_attribute(Graph *graph, int vertex_id, char *attr_name, double value) {
    if (igraph_cattribute_VAN_set(graph, attr_name, vertex_id, value) != IGRAPH_SUCCESS) {
        fprintf(stderr, "Erreur : impossible de définir l'attribut %s pour le sommet %d.\n", attr_name, vertex_id);
    }
}

void set_vertix_attributes(Graph *g, char* attr_name, Vector *values) {
    if (igraph_cattribute_VAN_setv(g, attr_name, values) != IGRAPH_SUCCESS) {
        fprintf(stderr, "Erreur : impossible de définir l'attribut %s pour les sommets.\n", attr_name);
    }
}

double get_vertix_attribute(igraph_t *graph, int vertex_id, char *attr_name) {
    double value = igraph_cattribute_VAN(graph, attr_name, vertex_id);
    if (!igraph_cattribute_has_attr(graph, IGRAPH_ATTRIBUTE_VERTEX, attr_name)) {
        fprintf(stderr, "Erreur : l'attribut %s n'existe pas pour le sommet %d.\n", attr_name, vertex_id);
        return -1;
    }
    return value;
}

bool has_vertix_attribute(igraph_t *graph, char *attr_name) {
    return igraph_cattribute_has_attr(graph, IGRAPH_ATTRIBUTE_VERTEX, attr_name);
}

void set_edge_attribute(igraph_t *g, int edge_id, char *attr_name, double value) {
    if (igraph_cattribute_EAN_set(g, attr_name, edge_id, value) != IGRAPH_SUCCESS) {
        fprintf(stderr, "Erreur : impossible de définir l'attribut %s pour l'arête %d.\n", attr_name, edge_id);
    }
}

void set_edge_attributes(Graph *g, char* attr_name, Vector *values) {
    if (igraph_cattribute_EAN_setv(g, attr_name, values) != IGRAPH_SUCCESS) {
        fprintf(stderr, "Erreur : impossible de définir l'attribut %s pour les arêtes.\n", attr_name);
    }
}

double get_edge_attribute(igraph_t *g, int edge_id, char *attr_name) {
    double value = igraph_cattribute_EAN(g, attr_name, edge_id);
    if (!igraph_cattribute_has_attr(g, IGRAPH_ATTRIBUTE_EDGE, attr_name)) {
        fprintf(stderr, "Erreur : l'attribut %s n'existe pas pour l'arête.\n", attr_name);
        return -1;
    }
    return value;
}

bool has_edge_attribute(igraph_t *graph, char *attr_name) {
    return igraph_cattribute_has_attr(graph, IGRAPH_ATTRIBUTE_EDGE, attr_name);
}

void init_vector(Vector *v, int size) {
    if (igraph_vector_init(v, size) != IGRAPH_SUCCESS) {
        fprintf(stderr, "Erreur : impossible d'initialiser le vecteur.\n");
        exit(EXIT_FAILURE);
    }
}

// ----- VISUALISATION ------

void afficher_graphe(Graph *g, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    fprintf(file, "graph {\n");

    // Ajouter les sommets avec leurs positions et leurs couleurs
    for (int i = 0; i < vertices_count(g); i++) {
        Station type_station = get_vertix_attribute(g, i, ATTR_STATION);
        char *color = (type_station == CHARGEUR) ? "red" : "lightgray";

        if (has_vertix_attribute(g, ATTR_COORD_X) && has_vertix_attribute(g, ATTR_COORD_Y)) {
            int magnifiying_factor = 15;
            double x = get_vertix_attribute(g, i, ATTR_COORD_X);
            double y = get_vertix_attribute(g, i, ATTR_COORD_Y);
            fprintf(file, "  %d [pos=\"%.2f,%.2f!\", style=filled, color=%s];\n", i, (x*magnifiying_factor+104), (y*magnifiying_factor-39), color);
        } else {
            fprintf(file, "  %d [style=filled, color=%s];\n", i, color);
        }
    }

    // Ajouter les arêtes avec les poids comme labels
    for (int i = 0; i < edges_count(g); i++) {
        int from, to;
        double poids = get_edge_attribute(g, i, ATTR_WEIGHT);
        get_edge_vertices(g, i, &from, &to);
        fprintf(file, "  %d -- %d [label=\"%.2fkm\"];\n", (int)from, (int)to, poids);
    }

    fprintf(file, "}\n");
    fclose(file);

    // Générer l'image avec Graphviz
    char command[256];
    snprintf(command, sizeof(command), "neato -Tpng %s -o output.png && open output.png", filename);
    int ret = system(command);
    if (ret != 0) {
        fprintf(stderr, "Erreur lors de l'exécution de Graphviz\n");
    }
}

// ----- GRAPHES -----

Graph get_random_graph(int nb_sommets) {
    Graph g = random_init(nb_sommets);

    for(int i = 0; i < nb_sommets; i++) {
        set_vertix_attribute(&g, i, ATTR_STATION, NORMAL);
    }

    int nb_arcs = edges_count(&g);

    Vector poids, population;
    init_vector(&poids, nb_arcs);
    init_vector(&population, nb_sommets);

    // Attribuer des poids aléatoires
    for (int i = 0; i < nb_arcs; i++) {
        VECTOR(poids)[i] = rand() % (POIDS_MAX - POIDS_MIN + 1) + POIDS_MIN;
    }
    for (int i = 0; i < nb_sommets; i++) {
        VECTOR(population)[i] = rand() % RAND_POPULATION_MAX;
    }

    set_edge_attributes(&g, ATTR_WEIGHT, &poids);
    set_vertix_attributes(&g, ATTR_POP, &population);

    igraph_vector_destroy(&poids);
    igraph_vector_destroy(&population);

    return g;
}

igraph_t get_colorado_graph() {
    return graph_from_csv("colo_vertices.csv", "colo_edges_3_max.csv");
}

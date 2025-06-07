#ifndef TIPE_H
#define TIPE_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <float.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "igraph.h"

#define K 3 // Le k du k-médian
#define MAX_SOMMETS 100 // Nombre maximum de sommets par graphe
#define CAPACITE_BATTERIE 50 // Capacité en kWh de la batterie
#define CONSOMMATION 0.2f // Quantité de kWh (batterie) consommée pour 1km parcouru
#define CO2_EMIS 110 // Masse de CO2 émise pour 1km parcouru
#define POIDS_MAX 200 // Poids maximum pour une arête (pour génération aléatoire)
#define POIDS_MIN 10 // Poids minimum pour une arête (pour une génération aléatoire)
#define RAND_POPULATION_MAX 100 // Population maximale pour un sommet (pour une génération aléatoire)
#define MAX_LINE_LENGTH 1024 // Pour fichiers CSV
#define MAX_FIELDS 100 // Pour fichiers CSV
#define ATTR_WEIGHT "poids" // Attribut pour le poids des arêtes
#define ATTR_POP "population" // Attribut pour la population
#define ATTR_STATION "station" // Attribut pour le type de station
#define ATTR_COORD_X "x" // Attribut pour la coordonnée x
#define ATTR_COORD_Y "y" // Attribut pour la coordonnée y
#define ATTR_LONG ATTR_COORD_X // Attribut pour la longitude
#define ATTR_LAT ATTR_COORD_Y // Attribut pour la latitude
#define CSV_SKIP_LINE '#' // Caractère pour ignorer une ligne dans le CSV

typedef igraph_t Graph;
typedef igraph_vector_t Vector;
typedef enum {
    EN_MARCHE, ARRIVE, EN_CHARGE, EN_PANNE, AUTRE
} Statut;
typedef enum {
    NORMAL, CHARGEUR
} Station;
struct Vehicule_s {
    int id;
    int depart, destination, position;
    igraph_vector_int_t chemin; // Itinéraire que va suivre le véhicule
    float batterie;
    Statut statut;
    float distance;
};
typedef struct Vehicule_s Vehicule;

// k-médian
void kmedian(Graph *g, int k, int *centres);
double cout_theorique(Graph *g, int *centres);
double cout_reel(Graph *g, int *centres);
// Stations
void definir_station(Graph *graph, Station* stations);
Station get_station_status(Graph *g, int i);
void attribuer_stations(Graph *graph);
// Simulation
void simulation(Graph reseau, int nb_vehicules);
// Graphes
// - Outils de base
Graph init(int nb_sommets);
Graph random_init(int nb_sommets);
int vertices_count(Graph *g);
int edges_count(Graph *g);
int get_edge_id(Graph *g, int a, int b);
void add_edge(Graph *g, int a, int b, double weight);
void get_edge_vertices(Graph *g, int edge_id, int *from, int *to);
void set_vertix_attribute(Graph *graph, int id, char *attr_name, double value);
void set_vertix_attributes(Graph *g, char *name, Vector *values);
double get_vertix_attribute(Graph *g, int id, char *name);
bool has_vertix_attribute(Graph *g, char *name);
void set_edge_attribute(Graph *g, int id, char *name, double value);
void set_edge_attributes(Graph *g, char *name, Vector *values);
double get_edge_attribute(Graph *g, int id, char *name);
bool has_edge_attribute(Graph *g, char *name);
void init_vector(Vector *v, int size);
// - Autre
void afficher_graphe(Graph *graph, char *filename);
Graph get_random_graph(int nb_sommets);
Graph get_colorado_graph();
// CSV
Graph graph_from_csv(char *vertices_file, char *edges_file);

#endif
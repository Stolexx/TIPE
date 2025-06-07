#include "tipe.h"

Graph *graph;
Vehicule *vehicules;
int nb_vehicules;

// Fonctions

void free_trafic() {
    if(vehicules != NULL) free(vehicules);
}

/**
 * Détermine le parcours complet d'un véhicule en tenant compte de son autonomie.
 * - Si l’autonomie restante suffit pour atteindre la destination, on renvoie
 *   directement le plus court chemin (Dijkstra).
 * - Sinon, on choisit récursivement la station de recharge la plus éloignée
 *   encore atteignable, on “recharge” (batterie remise à CAPACITE_BATTERIE)
 *   puis on poursuit jusqu’à destination.
 *
 * Le résultat est un igraph_vector_int_t contenant la suite des sommets.
 */
igraph_vector_int_t get_chemin(Graph *graph, Vehicule v) {

    igraph_vector_int_t chemin;
    igraph_vector_int_init(&chemin, 0);

    /* Récupération des poids de toutes les arêtes */
    igraph_vector_t poids;
    igraph_vector_init(&poids, 0);
    igraph_es_t aretes;
    igraph_es_all(&aretes, IGRAPH_EDGEORDER_ID);
    igraph_cattribute_EANV(graph, ATTR_WEIGHT, aretes, &poids);

    igraph_integer_t courant = v.depart;
    double batterie = v.batterie;

    while (courant != v.destination) {
        /* Plus court chemin courant → destination */
        igraph_vector_int_t segment;
        igraph_vector_int_init(&segment, 0);
        if (igraph_get_shortest_path_dijkstra(graph, &segment, NULL, courant, v.destination, &poids, IGRAPH_OUT) != IGRAPH_SUCCESS) {
            /* Pas de chemin : on renvoie ce qu’on a déjà */
            igraph_vector_int_destroy(&segment);
            break;
        }

        /* Distance (en km) et consommation (kWh) du segment */
        double dist_seg = 0.0;
        for (long k = 0; k < igraph_vector_int_size(&segment) - 1; k++) {
            igraph_integer_t u = VECTOR(segment)[k];
            igraph_integer_t v = VECTOR(segment)[k + 1];
            igraph_integer_t eid;
            igraph_get_eid(graph, &eid, u, v, IGRAPH_UNDIRECTED, 0);
            dist_seg += VECTOR(poids)[eid];
        }
        double conso_seg = dist_seg * CONSOMMATION;

        if (conso_seg <= batterie) {
            /* Segment faisable : on l’ajoute et on termine */
            if (igraph_vector_int_size(&chemin) == 0)
                igraph_vector_int_append(&chemin, &segment);
            else
                for (long k = 1; k < igraph_vector_int_size(&segment); k++)
                    igraph_vector_int_push_back(&chemin, VECTOR(segment)[k]);

            igraph_vector_int_destroy(&segment);
            break;
        }

        /* Sinon, chercher la station atteignable la plus éloignée */
        igraph_integer_t best_station = -1;
        double best_dist = -1.0;
        igraph_vector_int_t best_seg;
        igraph_vector_int_init(&best_seg, 0);

        for (igraph_integer_t v = 0; v < vertices_count(graph); v++) {
            if (!get_vertix_attribute(graph, v, ATTR_STATION))
                continue;

            igraph_vector_int_t tmp;
            igraph_vector_int_init(&tmp, 0);
            if (igraph_get_shortest_path_dijkstra(graph, &tmp, NULL, courant, v, &poids, IGRAPH_OUT) != IGRAPH_SUCCESS) {
                igraph_vector_int_destroy(&tmp);
                continue;
            }

            double dist_tmp = 0.0;
            for (long k = 0; k < igraph_vector_int_size(&tmp) - 1; k++) {
                igraph_integer_t a = VECTOR(tmp)[k];
                igraph_integer_t b = VECTOR(tmp)[k + 1];
                igraph_integer_t eid;
                igraph_get_eid(graph, &eid, a, b, IGRAPH_UNDIRECTED, 0);
                dist_tmp += VECTOR(poids)[eid];
            }

            if (dist_tmp * CONSOMMATION <= batterie && dist_tmp > best_dist) {
                best_dist = dist_tmp;
                best_station = v;
                igraph_vector_int_destroy(&best_seg);
                best_seg = tmp;
            } else {
                igraph_vector_int_destroy(&tmp);
            }
        }

        if (best_station == -1) {
            /* Erreur : Aucune station atteignable */
            igraph_vector_int_destroy(&segment);
            break;
        }

        /* Ajout du segment jusqu’à la station */
        if (igraph_vector_int_size(&chemin) == 0){
            igraph_vector_int_append(&chemin, &best_seg);
        } else{
            for (long k = 1; k < igraph_vector_int_size(&best_seg); k++) {
                igraph_vector_int_push_back(&chemin, VECTOR(best_seg)[k]);
            }
        }

        courant = best_station;     /* On se trouve maintenant à la station */
        batterie = v.batterie;      /* Recharge complète */
        igraph_vector_int_destroy(&segment);
        igraph_vector_int_destroy(&best_seg);
    }

    igraph_vector_destroy(&poids);
    igraph_es_destroy(&aretes);
    return chemin;
}

void generer_trafic(int n) {
    srand(time(NULL));
    nb_vehicules = n;
    free_trafic();
    Vehicule* res = malloc(n*sizeof(Vehicule));

    double total_pop = 0.0;
    for (igraph_integer_t v = 0; v < vertices_count(graph); v++) {
        total_pop += get_vertix_attribute(graph, v, ATTR_POP);
    }

    for(int i = 0; i < n; i++) {
        res[i].id = i+1;
        double r = ((double) rand() / RAND_MAX) * total_pop;
        double cumulative = 0.0;
        igraph_integer_t depart = 0;
        for (igraph_integer_t vtx = 0; vtx < vertices_count(graph); vtx++) {
            cumulative += get_vertix_attribute(graph, vtx, ATTR_POP);
            if (r <= cumulative) {
                depart = vtx;
                break;
            }
        }
        res[i].depart = depart;
        res[i].position = depart;
        do {
            res[i].destination = rand() % vertices_count(graph); // Aléatoire qui prend en compte la population
        } while (res[i].destination == res[i].depart);
        res[i].batterie = CAPACITE_BATTERIE;
        res[i].statut = EN_MARCHE;
        res[i].distance = 0.0f;
        printf("Véhicule %i généré (%i -> %i)\n", res[i].id, res[i].depart, res[i].destination);
        /* Calcul du chemin complet en tenant compte de l'autonomie */
        res[i].chemin = get_chemin(graph, res[i]);
    }

    vehicules = res;
}

bool est_finie() { // Vérifie si la simulation est terminée (i.e. tous les véhicules ne sont plus "EN_MARCHE" ou "EN_CHARGE")
    for(int i = 0; i < nb_vehicules; i++) {
        Vehicule v = vehicules[i];
        Statut statut = v.statut;
        if(statut == EN_MARCHE || statut == EN_CHARGE) {
            return false;
        }
    }
    return true;
}

void simuler_tour() {
    printf("----- Début Tour -----\n");

    /* Préparer le vecteur des poids d’arêtes (longueur en km) */
    igraph_vector_t poids;
    igraph_vector_init(&poids, 0);
    igraph_es_t aretes;
    igraph_es_all(&aretes, IGRAPH_EDGEORDER_ID);
    igraph_cattribute_EANV(graph, ATTR_WEIGHT, aretes, &poids);

    for (int i = 0; i < nb_vehicules; i++) {
        Vehicule *v = &vehicules[i];

        /* On ne traite que les véhicules encore en marche */
        if (v->statut != EN_MARCHE)
            continue;

        igraph_vector_int_t *chemin = &v->chemin;

        /* Chercher la position courante dans le chemin pré‑calculé */
        long idx = -1;
        for (long k = 0; k < igraph_vector_int_size(chemin); k++) {
            if ((int)VECTOR(*chemin)[k] == v->position) {
                idx = k;
                break;
            }
        }

        /* Sécurité : la position devrait toujours être trouvée */
        if (idx == -1 || idx + 1 >= igraph_vector_int_size(chemin)) {
            printf("Véhicule %d : position %d introuvable dans son chemin - abandon\n", v->id, v->position);
            v->statut = AUTRE;
            continue;
        }

        int prochain_sommet = (int)VECTOR(*chemin)[idx + 1];

        /* Consommation et distance pour l’arête courante */
        igraph_integer_t eid;
        if (igraph_get_eid(graph, &eid, v->position, prochain_sommet, IGRAPH_UNDIRECTED, 0) != IGRAPH_SUCCESS || eid == -1) {
            printf("Erreur : aucune arête entre %d et %d\n", v->position, prochain_sommet);
            v->statut = AUTRE;
            continue;
        }

        double distance_arc = VECTOR(poids)[eid];
        double conso = distance_arc * CONSOMMATION;

        if (conso > v->batterie + 1e-9) {
            printf("Véhicule %d : panne sèche anticipée (%.2f kWh requis, %.2f kWh restants)\n",
                   v->id, conso, v->batterie);
            v->statut = EN_PANNE;
            continue;
        }

        v->batterie -= conso;
        v->distance += distance_arc;
        printf("Véhicule %d : %d → %d (%.2f km | -%.2f kWh, reste %.2f kWh)\n",
               v->id, v->position, prochain_sommet,
               distance_arc, conso, v->batterie);

        v->position = prochain_sommet;

        /* Recharge instantanée si on est sur une station (et pas à destination) */
        if (get_vertix_attribute(graph, prochain_sommet, ATTR_STATION) && prochain_sommet != v->destination) {
            v->batterie = CAPACITE_BATTERIE;
            printf("Véhicule %d : recharge complète à la station %d (%.2f kWh)\n",
                   v->id, prochain_sommet, v->batterie);
        }

        /* Arrivée ? */
        if (prochain_sommet == v->destination) {
            v->statut = ARRIVE;
            printf("Véhicule %d : arrivé à destination 🎉\n", v->id);
        }
    }

    igraph_vector_destroy(&poids);
    igraph_es_destroy(&aretes);
    printf("----- Fin Tour -----\n");
}

void afficher_statistiques() {
    float total_distance = 0.0f;
    int arrive = 0, panne = 0, autre = 0;
    for(int i = 0; i < nb_vehicules; i++) {
        Vehicule v = vehicules[i];
        total_distance += v.distance;
        printf("Véhicule %i : %.2fkm parcourus / %.2fkWh consommés / %.2fkWh restants\n", v.id, v.distance, v.distance*CONSOMMATION, v.batterie);
        switch (v.statut) {
        case ARRIVE:
            arrive++;
            break;

        case EN_PANNE:
            panne++;
            break;
        
        default:
            autre++;
            break;
        }
    }
    printf("Total distance : %.2f km\n", total_distance);
    printf("Total consommé : %.2f kWh\n", total_distance*CONSOMMATION);
    printf("Total non-émis : %.2f kgCO2\n", (total_distance*CO2_EMIS)/1000.0);
    printf("Total statut : %i arrivés / %i en panne / %i autre\n", arrive, panne, autre);
}

// Simulation

void simulation(Graph reseau, int nb_trafic) {
    printf("Simulation\n");
    graph = malloc(sizeof(Graph));
    *graph = reseau;
    generer_trafic(nb_trafic);
    while(!est_finie()) {
        simuler_tour();
    }
    printf("Fini!\n");
    afficher_statistiques();
}


#include "tipe.h"

// Structures
typedef struct {
    char **fields;
    int field_count;
} CSVRow;

typedef struct {
    CSVRow *rows;
    int row_count;
} CSVFile;

CSVRow read_csv_row(FILE *file) {
    char line[MAX_LINE_LENGTH];
    CSVRow row;
    row.fields = malloc(MAX_FIELDS * sizeof(char *));
    row.field_count = 0;

    if (fgets(line, MAX_LINE_LENGTH, file)) {
        if (line[0] == CSV_SKIP_LINE) {
            free(row.fields);
            row.field_count = 0;
            return row;
        }
        char *token = strtok(line, ",\n");
        while (token != NULL) {
            row.fields[row.field_count] = strdup(token);
            row.field_count++;
            token = strtok(NULL, ",\n");
        }
    }

    return row;
}

CSVFile read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier CSV");
        exit(EXIT_FAILURE);
    }

    CSVFile csv;
    csv.rows = malloc(MAX_LINE_LENGTH * sizeof(CSVRow));
    csv.row_count = 0;

    while (!feof(file)) {
        CSVRow row = read_csv_row(file);
        if (row.field_count > 0) {
            csv.rows[csv.row_count] = row;
            csv.row_count++;
        }
    }

    fclose(file);
    return csv;
}

void write_csv_row(FILE *file, CSVRow row) {
    for (int i = 0; i < row.field_count; i++) {
        fprintf(file, "%s", row.fields[i]);
        if (i < row.field_count - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");
}

void free_csv_row(CSVRow row) {
    for (int i = 0; i < row.field_count; i++) {
        free(row.fields[i]);
    }
    free(row.fields);
}

void free_csv_file(CSVFile csv) {
    for (int i = 0; i < csv.row_count; i++) {
        free_csv_row(csv.rows[i]);
    }
    free(csv.rows);
}

/* Recherche l'identifiant d'un sommet par label "long,lat". Renvoie -1 si absent. */
int find_vertex_id(Graph g, double lon_ref, double lat_ref) {
    for (int i = 0; i < vertices_count(&g); i++) {
        double lon = get_vertix_attribute(&g, i, ATTR_LONG);
        double lat = get_vertix_attribute(&g, i, ATTR_LAT);
        if (lon == lon_ref && lat == lat_ref) return i;
        // if (strcmp(labels[i], label) == 0) return i;
    }
    return -1;
}

/* ------------------------------------------------------------------ *
 *  Construit le graphe à partir de deux CSV :
 *   - vertices_file  : long, lat, population
 *   - edges_file     : long1, lat1, long2, lat2, distance
 *  Renvoie un Graph initialisé.
 * ------------------------------------------------------------------ */
Graph graph_from_csv(char *vertices_file, char *edges_file) {
    static int attr_table_set = 0;
    if (!attr_table_set) {
        igraph_set_attribute_table(&igraph_cattribute_table);
        attr_table_set = 1;
    }

    /* Lecture du CSV des sommets ------------------------------------ */
    CSVFile vcsv = read_csv(vertices_file);
    int nb_lignes = vcsv.row_count;

    Graph g = init(nb_lignes);

    //char **labels = malloc(nb_lignes * sizeof(char *));

    for (int i = 0; i < nb_lignes; i++) {
        CSVRow row = vcsv.rows[i];
        if (row.field_count < 3) continue;

        double lon = atof(row.fields[0]);
        double lat = atof(row.fields[1]);
        double pop = atof(row.fields[2]);

        set_vertix_attribute(&g, i, ATTR_LONG, lon);
        set_vertix_attribute(&g, i, ATTR_LAT, lat);
        set_vertix_attribute(&g, i, ATTR_POP, pop);
        set_vertix_attribute(&g, i, ATTR_STATION, NORMAL);

        /*char lbl[64];
        snprintf(lbl, sizeof(lbl), "%s,%s", row.fields[0], row.fields[1]);
        labels[i] = strdup(lbl);
        SETVAS(&g, "label", i, lbl);*/
    }

    /* Lecture du CSV des arêtes ------------------------------------- */
    CSVFile ecsv = read_csv(edges_file);

    /*Vector edges_vec, w_vec;
    init_vector(&edges_vec, ecsv.row_count * ecsv.row_count);
    init_vector(&w_vec, ecsv.row_count);*/

    //int epos = 0;
    for (int r = 0; r < ecsv.row_count; r++) {
        CSVRow row = ecsv.rows[r];
        if (row.field_count < 5) continue;

        /*char lbl1[64], lbl2[64];
        snprintf(lbl1, sizeof(lbl1), "%s,%s", row.fields[0], row.fields[1]);
        snprintf(lbl2, sizeof(lbl2), "%s,%s", row.fields[2], row.fields[3]);

        int v1 = find_vertex_id(labels, nb_lignes, lbl1);
        int v2 = find_vertex_id(labels, nb_lignes, lbl2);
        if (v1 < 0 || v2 < 0) continue;*/

        double lon1 = atof(row.fields[0]);
        double lat1 = atof(row.fields[1]);
        double lon2 = atof(row.fields[2]);
        double lat2 = atof(row.fields[3]);
        double dist = atof(row.fields[4]);

        int v1 = find_vertex_id(g, lon1, lat1);
        int v2 = find_vertex_id(g, lon2, lat2);
        if (v1 < 0 || v2 < 0) continue;

        add_edge(&g, v1, v2, dist);

        /*VECTOR(edges_vec)[epos++] = v1;
        VECTOR(edges_vec)[epos++] = v2;
        VECTOR(w_vec)[(epos / 2) - 1] = atof(row.fields[4]);*/
    }

    //igraph_add_edges(&g, &edges_vec, NULL);

    //set_edge_attributes(&g, ATTR_WEIGHT, &w_vec);

    //igraph_vector_destroy(&edges_vec);
    //igraph_vector_destroy(&w_vec);

    /* Libérations ---------------------------------------------------- */
    /*for (int i = 0; i < nb_lignes; i++) {
        free(labels[i]);
    }
    free(labels);*/

    free_csv_file(vcsv);
    free_csv_file(ecsv);

    return g;
}

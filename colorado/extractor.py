# coding: utf-8
"""
extractor.py
-------------
Lit un fichier CSV contenant des coordonnées (longitude, latitude) et génère
la matrice du produit cartésien de cette liste avec elle-même.

Usage :
    python extractor.py data.csv            # CSV avec en-tête
    python extractor.py data.csv --no-header  # CSV sans ligne d’en-tête
"""

import csv
import sys
from typing import List, Tuple


def read_coords(csv_path: str, has_header: bool = True, delimiter: str = ",") -> List[Tuple[float, float]]:
    """Renvoie une liste de couples (longitude, latitude) extraits des deux premières colonnes du CSV."""
    coords: List[Tuple[float, float]] = []

    with open(csv_path, newline="", encoding="utf-8") as f:
        reader = csv.reader(f, delimiter=delimiter)

        if has_header:
            next(reader, None)  # Ignore la ligne d’en‑tête

        for row in reader:
            if len(row) < 2:
                continue  # Ligne incomplète

            try:
                lon = float(row[0].strip())
                lat = float(row[1].strip())
                coords.append((lon, lat))
            except ValueError:
                # Ignore les lignes où la conversion en float échoue
                continue

    return coords


def cartesian_matrix(
    coords: List[Tuple[float, float]]
) -> List[List[Tuple[Tuple[float, float], Tuple[float, float]]]]:
    """Retourne une matrice (liste de listes) contenant les couples (coord_i, coord_j) uniques, sans couples identiques ni doublons symétriques (i < j)."""
    n = len(coords)
    # i < j garantit qu'on ne garde qu'une orientation de chaque couple
    return [[(coords[i], coords[j]) for j in range(i + 1, n)] for i in range(n)]


def main() -> None:
    if len(sys.argv) < 2:
        print(f"Usage : python {sys.argv[0]} <csv_path> [--no-header]")
        sys.exit(1)

    csv_path = sys.argv[1]
    has_header = True
    if len(sys.argv) > 2 and sys.argv[2] == "--no-header":
        has_header = False

    coords = read_coords(csv_path, has_header)
    if not coords:
        print("Aucune coordonnée valide trouvée dans le fichier.")
        sys.exit(1)

    matrix = cartesian_matrix(coords)

    # Résumé
    # Liste plate de couples uniques (i < j) pour l’écriture
    unique_pairs = [[coords[i], coords[j]] for i in range(len(coords)) for j in range(i + 1, len(coords))]
    total_pairs = len(unique_pairs)
    print(f"{len(coords)} coordonnées chargées.")
    print(f"{total_pairs} couples uniques générés (sans couples identiques ni doublons symétriques).")

    # Sauvegarde dans un format directement copiable dans un autre script Python
    with open("matrix.txt", "w", encoding="utf-8") as f:
        f.write("coord_pairs = [\n")
        for pair in unique_pairs:
            f.write(f"    [[{pair[0][0]}, {pair[0][1]}], [{pair[1][0]}, {pair[1][1]}]],\n")
        f.write("]\n")
    print("Matrice enregistrée dans matrix.txt (format coord_pairs)")

    # Exemple d’accès : matrix[i][j]  donne  (coords[i], coords[j])
    # Pour éviter un affichage massif, on ne montre pas toute la matrice.
    # Décommentez ci‑dessous pour la visualiser :
    # for row in matrix:
    #     print(row)


if __name__ == "__main__":
    main()
`10455 | HAMEL Thomas`
# Disposition des stations de recharge sur un réseau routier

&nbsp;&nbsp;&nbsp;&nbsp;Le sujet répond à l’envie de profiter du thème pour traiter une problématique actuelle, liée à la transition écologique : optimiser la disposition des stations de recharge pour véhicules électriques. Mon travail m'a permis de travailler sur la préservation de l'environnement tout en développant des compétences en informatique et simulation.

&nbsp;&nbsp;&nbsp;&nbsp;Ce TIPE aborde le thème de la transition énergétique vers des énergies renouvelables. J'étudie la transformation de réseaux routiers pour intégrer des stations de recharge, favorisant ainsi la conversion du parc actuel vers une mobilité plus propre.

## Positionnement thématique

INFORMATIQUE (Informatique Théorique)<br>
INFORMATIQUE (Informatique pratique)

## Mots clés

|En français            |En anglais           |
|-----------------------|---------------------|
|Réseau routier         |Road network         |
|Théorie des grapes     |Graph theory         |
|Optimisation           |Optimization         |
|Simulation informatique|Computer simulation  |
|Transition écologique  |Ecological transition|
## Bibliographie documentée

&nbsp;&nbsp;&nbsp;&nbsp;Dans le cadre de ce TIPE, l’étude de l’optimisation du placement des stations de recharge pour des véhicules électriques dans un réseau routier, modélisé par un graphe, a nécessité plusieurs approfondissements dans divers domaines. Pour cela, plusieurs ressources clés ont été consultées pour apporter une meilleure compréhension des savoirs et techniques en jeu.

&nbsp;&nbsp;&nbsp;&nbsp;L’ouvrage Introduction to Algorithms de Cormen et al. [1](https://mitpress.mit.edu/9780262533058/introduction-to-algorithms/) a été une ressource fondamentale pour comprendre les bases des algorithmes, notamment ceux liés à la théorie des graphes. Ce livre propose une couverture complète des algorithmes de plus courts chemins, tels que Dijkstra et Floyd-Warshall, mais également d’algorithmes plus poussés et hors programme tels que l'algorithme du k-médian.

&nbsp;&nbsp;&nbsp;&nbsp;Pour pouvoir utiliser dans mon code des problèmes algorithmiques NP-durs tels que le problème du k-médian, l’article Approximation Algorithms for NP-hard Problems édité par Hochbaum [2](https://www.cs.ucr.edu/~neal/publication/Klein99Approximation.pdf) a été d’une grande utilité. Ce recueil explore divers algorithmes d’approximation, qui permettent de trouver des solutions proches de la solution optimale dans des délais raisonnables, ce qui est un aspect crucial lorsque l’on traite de grands graphes dans lesquels la solution exacte serait trop coûteuse à calculer.

&nbsp;&nbsp;&nbsp;&nbsp;La mise en œuvre pratique de ces concepts a été facilitée par l’utilisation de la bibliothèque C igraph [4](https://igraph.org/c/), qui offre une interface puissante pour la manipulation des graphes. La documentation et les exemples fournis par igraph [4](https://igraph.org/c/) ont permis de traduire les théories algorithmiques en code fonctionnel, facilitant ainsi la simulation et la validation des modèles théoriques développés dans ce projet. Cette bibliothèque a été essentielle pour tester les différentes stratégies de placement des stations de recharge et pour analyser leurs performances sur des graphes complexes.

&nbsp;&nbsp;&nbsp;&nbsp;Enfin, l’article de Zhou et al. Location optimization of electric vehicle charging stations: Based on cost model and genetic algorithm [5](https://www.sciencedirect.com/science/article/pii/S0360544222003401) a joué un rôle primordial dans l’orientation de mes réflexions. Cet article, en explorant des études de cas et des méthodes spécifiques pour le placement optimal des stations de recharge, m’a permis de comprendre les enjeux liés à l’optimisation des infrastructures pour les véhicules électriques. Il a ainsi guidé mes choix et m’a aidé à définir les axes de recherche pertinents pour ce TIPE.

&nbsp;&nbsp;&nbsp;&nbsp;Pour confronter la solution trouvée par mon algorithme à une situation concrète (ici l’État du Colorado, aux États-Unis), il a été indispensable de s’appuyer sur des données officielles fiables. À cette fin, des ressources disponibles sur le site du gouvernement du Colorado ont été exploitées, notamment une carte détaillée des axes routiers principaux ainsi qu’une répartition géographique des stations de recharge existantes [3](https://energyoffice.colorado.gov/transportation/grants-incentives/ev-fast-charging-corridors). Ces informations ont permis de modéliser le réseau routier et d’analyser la disposition actuelle des infrastructures de recharge, servant ainsi de base à l’évaluation et à la validation des solutions optimisées proposées par mon travail.

## Problématique retenue

Comment concilier les contraintes économiques et écologiques dans la disposition optimale des stations de recharge pour véhicules électriques au sein d’un réseau routier donné ?

## Objectifs du TIPE du candidat

1. Modélisation : Modéliser le réseau routier en graphe et simuler un trafic routier de véhicules électriques à batterie limitée
2. Stations de recharge : Déterminer des critères pour disposer les stations de recharge en se basant sur des contraintes concrètes et réalistes (coût, etc)
3. Optimisation : Implémenter un algorithme d’optimisation pour résoudre le problème de la disposition des stations de recharge sur le graphe
4. Simulation : Simuler et comparer les résultats sur un cas concret (réseau routier du Colorado)
5. Transition écologique : Montrer l’impact écologique que pourrait avoir une telle optimisation

## Références bibliographiques

[1] T. H. CORMEN, C. E. LEISERSON, R. L. RIVEST, AND C. STEIN : Introduction to Algorithms (3rd ed.) : _https://mitpress.mit.edu/9780262533058/introduction-to-algorithms/_ <br>
[2] P. N. KLEIN AND N. E. YOUNG : Approximation algorithms for NP-hard optimization problems : _https://www.cs.ucr.edu/~neal/publication/Klein99Approximation.pdf_ <br>
[3] COLORADO ENERGY OFFICE : EV Fast Charging Corridors : _https://energyoffice.colorado.gov/transportation/grants-incentives/ev-fast-charging-corridors_ <br>
[4] IGRAPH TEAM : igraph C library : _https://igraph.org/c/_ <br>
[5] GUANGYOU ZHOU, ZHIWEI ZHU, SUMEI LUO : Location optimization of electric vehicle charging stations: Based on cost model and genetic algorithm : _https://www.sciencedirect.com/science/article/pii/S0360544222003401_

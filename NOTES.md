GOMES
LIN
LISE

# Commentaire général
Code clair et efficace. Toutes les fonctionnalités sont là avec en plus un éditeur de niveau, bravo à vous. (Voir les détails dans NOTES.md)

## Programmation orientée objet
### Héritage $$4
Bien implémenté autour de la classe Entity.

Attention au nommage, une classe se nomme au singulier (Enemy et pas Enemies).
### Encapsulation $$4
Private et protected bien utilisés
### Fonctions virtuelles $$4
Bonne utilisation des mots clefs virtual et override
### Utilisation de références et pas de copies inutiles $$4
OK

## Clarté du code
### Niveau d'abstraction cohérent (updates en cascade) $$4
La répartition de l'état du jeu dans différente scène est très propre.
### Clarté des nommages (fonctions/variables) $$4
Aucun soucis de ce côté là.
### Découpage des fichiers cohérent $$4
Fonctions `ButtonS` et `ButtonAsset` dans setup.hpp ?

Sinon, rien de trop anormal à signaler.

## Fonctionnalités
### Qualité du rendu $$3.25
Vous avez rempli le contrat mais le résultat est encore un peu rigide,
### Absence de warning à la compilation $$5
OK
### Memory leaks $$3.75
Pas de leak au runtime, par contre j'ai eu un crash avec le flag -fsanitize=address, qui ne m'a pas permis de bien tester le shutdown.
### Crash $$5
Pas de crash rencontrés

## Présentation
### Commentaires $$4.5
De nombreux commentaires ! Attention, parfois ils sont inutiles.

Ex:
```c++
//Generate the map
void Map::MapGeneration(...
```

### Readme $$4.5
Très bon readme illustré et intéressant.
### Norme de code $$4
Très bien respectée.

## Bonus $$4
### Sons $$4
### Format de donnée (niveau, score) $$4
### Editeur de niveau $$3.75

## Git
### Qualité des messages $$4
Bons messages, parfois un peu trop concis.
### Régularité des commits $$4
Commits réguliers tout au long du projet.

## Gestion de projet
### Utilisation des tâches GitLab $$3.5
Bien, mais toutes les tâches n'ont pas été recensées.
### Bonne répartition des tâches $$3.75
D'après les commits, tout le monde à participé de façon plutôt équilibrée.

## Code review (divers)
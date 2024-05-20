# Projet C - Interface Graphique

#### Contributeurs : EUZEBY Raphaël, HAVET Léonard, LEGRAND Hugo

LIEN : [http://brouet.imag.fr/fberard/ProjetCHL/ProjetC](http://brouet.imag.fr/fberard/ProjetCHL/ProjetC)

## Initialisation du projet

Après deux jours de compréhension des exigences et du code fourni, nous avons commencé par afficher une fenêtre noire, puis un carré bleu. Pour ce faire, nous avons dû coder les fonctions pour les frames et comprendre que root serait une frame sans parent. Une fois cette étape réalisée, nous étions convaincus que nous pouvions rendre les frames totalement fonctionnelles.

## Problèmes rencontrés

Beaucoup de débogage a été nécessaire en raison de notre précipitation, qui nous a fait oublier plusieurs aspects. En particulier, les `malloc` pour la configuration des frames (spécialement les attributs x, y, width, et height) ont été sources de nombreux problèmes. Après avoir compris l'utilisation correcte de `malloc`, nous avons pu progresser vers les boutons et les textes.

Les boutons sont en fait composés de trois frames : une claire, une foncée, et celle au-dessus. En y ajoutant du texte, nous avons pu afficher des frames et des boutons avec du texte. L'implémentation des événements pour la première fois a marqué la fin des plus grandes difficultés.

## Géométrie

La mise en place des éléments fut compliquée à cause des nombreux `malloc`, ainsi que de la compréhension de ce qu'il fallait mettre dans width et height. Au départ, nous mettions tout dans `screen_location` sans utiliser `content_rect`, ce qui posait des problèmes pour les fenêtres top-level.

## Toplevel et jeux

À ce stade, nous avions les outils nécessaires pour déboguer efficacement notre code et identifier les zones les moins claires. En débloquant les fenêtres top-level, nous avons pu tester les jeux 2048 (que nous n'avons pas réussi à gagner), Puzzle (terminé), et Minesweeper, en commentant les entrées (`entry`).

## Entrées (Entry)

Pour rendre Minesweeper totalement fonctionnel, il nous a fallu implémenter les entrées (`entry`). Cela comprenait la gestion des sélections pour les entrées ainsi que les touches spéciales.

## Ancien fichier Readme

Répertoire "racine" du projet.

Il contient le fichier CMakeLists.txt utilisé par cmake pour construire le projet.

Ne lancez pas cmake directement depuis ce répertoire car cmake génère beaucoup de fichiers. Lancez cmake depuis le répertoire "cmake", ou mieux, construisez votre projet avec CLion (qui s'occupe d'appeler cmake): lisez clion/README.
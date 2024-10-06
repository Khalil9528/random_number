# Rapport de Laboratoire #2 : Génération de Variates Aléatoires

## Introduction

Ce projet présente la génération de variates aléatoires en utilisant le générateur Mersenne Twister. Le rapport explore différentes méthodes de génération de nombres aléatoires et visualise les résultats obtenus à l'aide de graphiques.

## Structure du Projet

Le projet se compose des fichiers et répertoires suivants :

- **1.png** : Histogramme montrant la distribution uniforme des nombres générés.
- **2.png** : Histogramme représentant la distribution exponentielle négative.
- **3.png** : Graphique à barres illustrant la répartition des classes A, B et C dans la simulation discrète.
- **4.png** : Histogramme de la distribution normale générée par la méthode de Box-Muller.
- **5.png** : Graphique illustrant les résultats de la méthode de rejet pour générer des nombres suivant une loi normale.
- **My_file.tex** : Fichier source LaTeX contenant le rapport complet.
- **My_file.pdf** : Rapport compilé au format PDF.
- **README.md** : Ce fichier, contenant des informations sur le projet.
- **mt19937ar-cok.c** : Code source de l'implémentation du générateur Mersenne Twister et des méthodes de génération.
- **visualisation_tests.py** : Script Python pour visualiser les distributions générées par le code C.
- **autres fichiers (.aux, .log, .toc, .fdb_latexmk, .fls)** : Fichiers générés lors de la compilation du rapport LaTeX.

## Instructions d'Installation

Assurez-vous d'avoir les outils suivants installés :

- Un compilateur C (comme GCC).
- `pdflatex` pour compiler le document LaTeX.
- Python avec `matplotlib` et `numpy` pour la visualisation des données.

### Compilation du Rapport

Pour compiler le rapport, utilisez la commande suivante :

```bash
pdflatex My_file.tex

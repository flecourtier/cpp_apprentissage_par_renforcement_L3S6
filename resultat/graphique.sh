#!/bin/sh

../build/run.e > sortie
python resultat.py

echo "Graphique créé"
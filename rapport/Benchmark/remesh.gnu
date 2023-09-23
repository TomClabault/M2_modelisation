# Définition du format de sortie
set terminal pngcairo

# Définition du nom du fichier de sortie
set output 'remesh.png'

# Définition des titres de la figure
set title "Temps de calcul / # différences de sphère dans l'arbre de construction" # titre
set xlabel '# différences de sphère'     # nom de l'axe des abscisses
set ylabel 'Temps de calcul (ms)'            # nom de l'axe des ordonnées

plot 'remesh.dat' axis x1y1 with lines title "Temps de calcul"

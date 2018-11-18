#Il faut rajouter le dossier "hall" avec toutes les images à la racine

#Créer l'exécutable bench pour le code scalaire
#L'exécutable sera dans le directory exe/
make bench
cd exe/
./bench
#Va générer les images transformées dans output/hallScalar/

#Créer l'exécutable bench_SSE2 pour le code en SIMD
#L'exécutable sera dans le directory exe/
make bench_SSE2
cd exe/
./bench_SSE2
#Va générer les images transformées dans output/hallSSE/

#Créer l'exécutable bench_OMP pour le code en scalaire avec OpenMP
#L'exécutable sera dans le directory exe/
make bench_OMP
cd exe/
./bench_OMP
#Va générer les images transformées dans output/hallOMP/

#Créer l'exécutable bench_OMPxSSE2 pour le code en SIMD avec OpenMP
#L'exécutable sera dans le directory exe/
make bench_OMPxSSE2
cd exe/
./bench_OMPxSSE2
#Va générer les images transformées dans output/hallOMPxSSE2/

#Créer l'exécutable bench_OMP pour le code en scalaire avec OpenMP
#Les exécutables sont dans le directory test/
make test

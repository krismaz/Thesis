find ./ -name \*.tex -delete
find ./ -name \*.pdf -delete
find ./ -name \*.gp -execdir gnuplot {} \;
find ./ -name \*.tex -execdir pdflatex -interaction=batchmode  -file-line-error -halt-on-error {} \;
find ./ -name \*converted-to.pdf -delete
find . -name \*.pdf -exec cp --parents {} ../Tex/graphs \;
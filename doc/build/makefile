all:
	[ -d "./../bin/appendices" ] || mkdir "./../bin/appendices"
	cd ../ && pdflatex -output-dir "bin/" -shell-escape main.tex && biber --output-directory "bin" main

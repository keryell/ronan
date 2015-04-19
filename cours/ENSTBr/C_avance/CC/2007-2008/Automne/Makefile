INSTALL_DIR = $(HOME)/public_html/cours/ENSTBr/C_avance/CC/2007-2008/Automne
SUJET = sujet_janvier

DOCS = $(SUJET).ps $(SUJET).pdf $(SUJET)-avec_reponses.ps $(SUJET)-avec_reponses.pdf

#SUBDIR = $(shell basename `pwd`)
LATEXMODE=-interaction nonstopmode

all: $(DOCS)

clean::
	-rm $(DOCS) *-expose.tex *-eleves.tex *.aux *.bbl *.blg *.dvi *.idx *.ilg *.ind *.log *.los *.out *.rel *.tmp *.toc comment.cut

install:
	-mkdir -p $(INSTALL_DIR)/$(SUBDIR)
	cp -a $(DOCS) $(INSTALL_DIR)/$(SUBDIR)

%.pdf: %.ps
	ps2pdf $<

%.ps: %.dvi
	dvips $< -o

%.dvi: %.tex
	latex $<

%-avec_reponses.dvi : %.tex $(SOURCES)
	-latex $(LATEXMODE) -jobname=$*-avec_reponses '\def\AvecReponses{} \input{'$*'}'

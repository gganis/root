MDFILES=titlepage.md Introduction.md Basics.md Terminology.md Tutorials.md  \
	concludingRemarks.md references.md

OPTIONS=-s -S -f markdown --toc --chapters --number-sections \
        --bibliography=bibliography.bib
OPTIONSHTML=-H css/github.css --mathjax
OPTIONSEPUB=--mathml --epub-cover-image=figures/cover.png
OPTIONSPDFA4=--variable=geometry:a4paper -V geometry:margin=1.5cm
OPTIONSPDFLETTER=--variable=geometry:letterpaper -V geometry:margin=1.5cm

.PHONY: pdfa4 pdfletter html tex epub

all: pdfa4 pdfletter html tex epub

pdfa4: $(MDFILES) bibliography.bib
	pandoc $(OPTIONS) $(OPTIONSPDFA4) $(MDFILES) -o PROOFPrimer.pdf

pdfletter: $(MDFILES) bibliography.bib
	pandoc $(OPTIONS) $(OPTIONSPDFLETTER) $(MDFILES) -o PROOFPrimerLetter.pdf

html: $(MDFILES) bibliography.bib
	pandoc $(OPTIONS) $(OPTIONSHTML) -t html $(MDFILES) -o PROOFPrimer.html

tex: $(MDFILES) bibliography.bib
	pandoc $(OPTIONS) -t latex $(MDFILES) -o PROOFPrimer.tex

epub: $(MDFILES) metadata.xml title.txt bibliography.bib
	pandoc $(OPTIONS) $(OPTIONSEPUB) --epub-metadata=metadata.xml \
	   title.txt $(MDFILES) -o PROOFPrimer.epub

clean:
	rm -f PROOFPrimer.pdf PROOFPrimerLetter.pdf PROOFPrimer.html \
	   PROOFPrimer.tex PROOFPrimer.epub

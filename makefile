LATEX := xelatex
BUILDDIR := pdf
TEXDIR := tex

# ASSIGNMENT_TEXS := $(addprefix $(TEXDIR)/,assignment_introduction.tex assignment_body.tex assignment_body_literature_review.tex assignment_body_mockingjay.tex assignment_body_implementation.tex code/mockingjay.cpp assignment_evaluation.tex assignment_conclusions.tex references.bib assignment_appendix_generating_payloads.tex)
TEX_FILES := $(addprefix $(TEXDIR)/,assignment_*.tex references.bib)

PDF_FILES := $(BUILDDIR)/assignment_mockingjay.pdf $(BUILDDIR)/literature_review.pdf


LATEX_INTERMEDIATE := $(BUILDDIR)/

.PHONY: clean clean-bcf

$(BUILDDIR)/%.bcf: $(TEXDIR)/%.tex
	TEXINPUTS=".//tex//:" ; $(LATEX) -synctex=1 -interaction=nonstopmode --shell-escape -output-directory=$(BUILDDIR) $<

$(BUILDDIR)/%.bbl: $(BUILDDIR)/%.bcf $(TEXDIR)/references.bib
	biber --input-directory $(TEXDIR) $<

$(BUILDDIR)/%.toc: $(TEXDIR)/%.tex
	TEXINPUTS=".//tex//:" ; $(LATEX) -synctex=0 -interaction=nonstopmode --shell-escape -output-directory=$(BUILDDIR) $<

$(BUILDDIR)/%.pdf: $(TEXDIR)/%.tex $(BUILDDIR)/%.bbl $(BUILDDIR)/%.toc $(TEX_FILES)
	TEXINPUTS=".//tex//:" ; $(LATEX) -synctex=1 -interaction=nonstopmode --shell-escape -output-directory=$(BUILDDIR) $<

$(BUILDDIR):
	mkdir $(BUILDDIR)

all: $(PDF_FILES) $(BUILDDIR)


clean-bcf:
	-rm $(BUILDDIR)/*.bcf $(BUILDDIR)/*.blg

clean:
	-rm $(BUILDDIR)/*.aux $(BUILDDIR)/*.log $(BUILDDIR)/*.out $(BUILDDIR)/*.toc

info:
	@ls -l $(ASSIGNMENT_TEXS) pdf/ass*.b* pdf/ass*.toc pdf/ass*.pdf

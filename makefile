LATEX := xelatex
BUILDDIR := pdf
TEXDIR := tex

ASSIGNMENT_TEXS := $(addprefix $(TEXDIR)/,assignment_introduction.tex assignment_body.tex assignment_body_literature_review.tex assignment_body_mockingjay.tex assignment_body_implementation.tex code/mockingjay.cpp assignment_evaluation.tex assignment_conclusions.tex references.bib assignment_appendix_generating_payloads.tex)

LATEX_INTERMEDIATE := $(BUILDDIR)/

.PHONY: clean

$(BUILDDIR)/%.bcf: $(TEXDIR)/%.tex $(TEXDIR)/references.bib
	TEXINPUTS=";.//tex//" ; $(LATEX) -synctex=1 -interaction=nonstopmode --shell-escape -output-directory=$(BUILDDIR) $<

$(BUILDDIR)/%.bbl: $(BUILDDIR)/%.bcf
	biber --input-directory $(TEXDIR) $<

$(BUILDDIR)/%.pdf: $(TEXDIR)/%.tex $(BUILDDIR)/%.bbl $(BUILDDIR)/%.toc $(ASSIGNMENT_TEXS)
	TEXINPUTS=";.//tex//" ; $(LATEX) -synctex=1 -interaction=nonstopmode --shell-escape -output-directory=$(BUILDDIR) $<

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean:
	-rm $(BUILDDIR)/*.aux $(BUILDDIR)/*.log $(BUILDDIR)/*.out $(BUILDDIR)/*.toc

info:
	@ls -l $(ASSIGNMENT_TEXS) pdf/ass*.b* pdf/ass*.toc pdf/ass*.pdf

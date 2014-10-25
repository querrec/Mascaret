Comment editer et generer cette documentation.

--- Editer ---
Le fichier source est le .texinfo
C'est du fichier texte qui ressemble à du latex (en fait qui utilise latex).
La documentation est sur le site:
  http://www.gnu.org/software/texinfo/manual/texinfo/

--- Generer ---
Sous ubuntu, avec les divers packages texinfo installés:

- vers html:
    makeinfo --html documentationUtilisateur.texinfo

- vers pdf:
    texi2pdf documentationUtilisateur.texinfo

README fait le 7 avril 2010

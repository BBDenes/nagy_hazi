#ifndef MENTES_H
#define MENTES_H

void asztalokMentes(Asztal *asztalok, Rendeles **rendelesek, const char *fajlNev);
Asztal *asztalokBetoltes(Rendeles **rendelesek, const char *fajlNev);
int rendelesMax(char *fajlnev);

#endif
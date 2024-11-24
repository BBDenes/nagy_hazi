#ifndef MENTES_H
#define MENTES_H

void asztalokMentes(Asztal *asztalok, Rendeles **rendelesek, const char *fajlNev);
Rendeles **rendelesBetoltes(Asztal *asztalok, const char *fajlNev);
int rendelesMax(char *fajlnev);

#endif
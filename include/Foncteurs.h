/*
 * Titre : Foncteurs.h - Travail Pratique #5 - Programmation Orientée Objet
 * Date : 16 Avril 2020
 * Auteurs : Bouh Abdillahi (1940646) & Junior Lionel Metogo (1935652)
 */

#ifndef FONCTEUR_H
#define FONCTEUR_H

#include "LigneLog.h"

class EstDansIntervalleDatesFilm {
public:

	EstDansIntervalleDatesFilm(int borneInf, int borneSup)
		: borneInf_(borneInf)
		,borneSup_(borneSup)
		{};
	bool operator()(const std::unique_ptr<Film>& film) {
		return (film->annee >= borneInf_ && film->annee <= borneSup_);
	};
private:
	int borneInf_;
	int borneSup_;
};

class ComparateurLog {
public:
	bool operator()(const LigneLog& ligneLog_1, const LigneLog& ligneLog_2) {
		return (ligneLog_1.timestamp < ligneLog_2.timestamp);
	};
};

template<typename T1, typename T2>
class ComparateurSecondElementPaire {
public:
	bool operator()(const std::pair<T1, T2>& paire_1, const std::pair<T1, T2>& paire_2) {
		return (paire_1.second < paire_2.second);
	};
};


#endif // FONCTEUR_H
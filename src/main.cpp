/// Fonction main.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#include <iostream>
#include "AnalyseurLogs.h"
#include "GestionnaireFilms.h"
#include "GestionnaireUtilisateurs.h"
#include "Tests.h"
#include "WindowsUnicodeConsole.h"

int main()
{
    // Change le code page de la console Windows en UTF-8 si l'OS est Windows
    initializeConsole();

    Tests::testAll();

    // Écrivez le code pour le bonus ici
	GestionnaireUtilisateurs gestionnaireUtilisateurs;
	gestionnaireUtilisateurs.chargerDepuisFichier("utilisateurs.txt");
	GestionnaireFilms gestionnaireFilms;
	gestionnaireFilms.chargerDepuisFichier("films.txt");
	AnalyseurLogs analyseurLogs;
	analyseurLogs.chargerDepuisFichier("logs.txt", gestionnaireUtilisateurs, gestionnaireFilms);
	std::cout << std::endl << gestionnaireUtilisateurs << std::endl;
	std::cout << "Films d'aventure:" << std::endl;
	for (const auto& element : gestionnaireFilms.getFilmsParGenre(Film::Genre::Aventure))
	{
		std::cout << '\t' << *element << '\n';
	}
	std::cout << std::endl << "Films produits de 1960 à 1961: " << std::endl;
	for (const auto& element : gestionnaireFilms.getFilmsEntreAnnees(1960, 1961))
	{
		std::cout << '\t' << *(element) << std::endl;
	}
	const Film* filmPopulaire = analyseurLogs.getFilmPlusPopulaire();
	std::cout << std::endl << "Film le plus populaire(" << analyseurLogs.getNombreVuesFilm(filmPopulaire) << " vues) : "
		<< *filmPopulaire << std::endl;
	std::cout << std::endl << "5 films les plus populaires: " << std::endl;
	for (const auto& element : analyseurLogs.getNFilmsPlusPopulaires(5))
	{
		std::cout << '\t' << *(element.first) << " (" << element.second << " vues)" << '\n';
	}
	const std::string id = "karasik@msn.com";
	const Utilisateur* utilisateur = gestionnaireUtilisateurs.getUtilisateurParId(id);
	std::cout << std::endl << "Nombre de films vus par l'utilisateur " << id << ": " << analyseurLogs.getNombreVuesPourUtilisateur(utilisateur) << std::endl;
}

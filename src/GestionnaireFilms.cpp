/// Gestionnaire de films.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#include "GestionnaireFilms.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Foncteurs.h"
#include "RawPointerBackInserter.h"

/// Constructeur par copie.
/// \param other    Le gestionnaire de films à partir duquel copier la classe.
GestionnaireFilms::GestionnaireFilms(const GestionnaireFilms& other)
{
    films_.reserve(other.films_.size());
    filtreNomFilms_.reserve(other.filtreNomFilms_.size());
    filtreGenreFilms_.reserve(other.filtreGenreFilms_.size());
    filtrePaysFilms_.reserve(other.filtrePaysFilms_.size());

    for (const auto& film : other.films_)
    {
        // TODO: Uncomment une fois que la fonction ajouterFilm est écrite
         ajouterFilm(*film);
    }
}

/// Opérateur d'assignation par copie utilisant le copy-and-swap idiom.
/// \param other    Le gestionnaire de films à partir duquel copier la classe.
/// \return         Référence à l'objet actuel.
GestionnaireFilms& GestionnaireFilms::operator=(GestionnaireFilms other)
{
    std::swap(films_, other.films_);
    std::swap(filtreNomFilms_, other.filtreNomFilms_);
    std::swap(filtreGenreFilms_, other.filtreGenreFilms_);
    std::swap(filtrePaysFilms_, other.filtrePaysFilms_);
    return *this;
}

/// Affiche les informations des films gérés par le gestionnaire de films à la sortie du stream donné.
/// \param outputStream         Le stream auquel écrire les informations des films.
/// \param gestionnaireFilms    Le gestionnaire de films à afficher au stream.
/// \return                     Une référence au stream.
std::ostream& operator<<(std::ostream& outputStream, const GestionnaireFilms& gestionnaireFilms)
{
    // TODO: Uncomment une fois que la fonction getNombreFilms est écrite
    outputStream << "Le gestionnaire de films contient " << gestionnaireFilms.getNombreFilms() << " films.\n"
                 << "Affichage par catégories:\n";

    // TODO: Réécrire l'implémentation avec des range-based for et structured bindings (voir énoncé du TP)
	for (const auto& [key, value] : gestionnaireFilms.filtreGenreFilms_)
	{
		Film::Genre genre = key;
		std::vector<const Film*> listeFilms = value;
		outputStream << "Genre: " << getGenreString(genre) << " (" << listeFilms.size() << " films):\n";
		for (const auto& element : listeFilms)
		{
			outputStream << '\t' << *element << '\n';
		}
	}
    return outputStream;
}

/// Ajoute les films à partir d'un fichier de description des films.
/// \param nomFichier   Le fichier à partir duquel lire les informations des films.
/// \return             True si tout le chargement s'est effectué avec succès, false sinon.
bool GestionnaireFilms::chargerDepuisFichier(const std::string& nomFichier)
{
    std::ifstream fichier(nomFichier);
    if (fichier)
    {
        films_.clear();
        filtreNomFilms_.clear();
        filtreGenreFilms_.clear();
        filtrePaysFilms_.clear();

        bool succesParsing = true;

        std::string ligne;
        while (std::getline(fichier, ligne))
        {
            std::istringstream stream(ligne);

            std::string nom;
            int genre;
            int pays;
            std::string realisateur;
            int annee;

            if (stream >> std::quoted(nom) >> genre >> pays >> std::quoted(realisateur) >> annee)
            {
                // TODO: Uncomment une fois que la fonction ajouterFilm est écrite
                 ajouterFilm(Film{nom, static_cast<Film::Genre>(genre), static_cast<Pays>(pays), realisateur, annee});
            }
            else
            {
                std::cerr << "Erreur GestionnaireFilms: la ligne " << ligne
                          << " n'a pas pu être interprétée correctement\n";
                succesParsing = false;
            }
        }
        return succesParsing;
    }
    std::cerr << "Erreur GestionnaireFilms: le fichier " << nomFichier << " n'a pas pu être ouvert\n";
    return false;
}

/// Ajoute un film au gestionnaire et met à jour les filtres en conséquence.
/// \param film         Le film à ajouter.
/// \return             True si le film a été ajouté avec succès, 
//						False si le film n’a pas pu être ajouté puisque son nom était déjà présent dans le filtre par nom (il ne peut y avoir qu’un film avec le même nom à la fois).
bool GestionnaireFilms::ajouterFilm(const Film& film)
{
	if (getFilmParNom(film.nom) != nullptr)
	{
		return false;
	}
	films_.push_back(std::move(std::make_unique<Film>(film)));
	filtreNomFilms_.emplace(film.nom, films_.back().get());
	filtreGenreFilms_[film.genre].push_back(films_.back().get());
	filtrePaysFilms_[film.pays].push_back(films_.back().get());
	return true;
}

/// Supprime un film du gestionnaire à partir de son nom.
/// \param nomFilm      Le nom du film à supprimer.
/// \return             True si le film a été trouvé et supprimé avec succès, 
//						False si le film n’a pas pu être supprimé puisqu’aucun film avec le nom donné n’a pu être trouvé.
bool GestionnaireFilms::supprimerFilm(const std::string& nomFilm)
{
	//TODO: trouver itérateur vers film de nom donné
	//		TODO: utiliser méthode de la STL avec fonction lambda pour cela
				//TODO: fonction lambda: cherche le unique_ptr de Film à partir du string donné en paramètre
	std::vector<std::unique_ptr<Film>>::iterator iteratorTrouve = find_if(films_.begin(), films_.end()
		, [&nomFilm](std::unique_ptr<Film>& film)
		{ return (film->nom == nomFilm); });

	//TODO: Tester valeur film , vérifier si le film existe dans le vecteur
	//	Si oui, déréferencer l'itérateur pour obtenir le film
	//	Si non, retourner false????
	if (iteratorTrouve == films_.end())
	{
		return false;
	}
	Film filmTrouve = *(iteratorTrouve->get());

	//TODO: Suppression dans filtreNomFilms_: Utiliser une méthode de suppression par clé
	filtreNomFilms_.erase(nomFilm);

	//TODO: Suppression dans filtreGenreFilms_ et filtrePaysFilms_
	//	initialiser une référence au vecteur du genre/pays du film donné
	//	effacer l'élement du vecteur à l'aide de sa valeur, et non de son index
	//		TODO: utiliser erase-remove idiom pour cela

		//Pour les Pays
	std::vector<const Film* >& vecteurPaysFilms = filtrePaysFilms_[filmTrouve.pays]; //initialisation de la référence
	auto iteratorPays = remove(vecteurPaysFilms.begin(), vecteurPaysFilms.end(), iteratorTrouve->get());
	vecteurPaysFilms.erase(iteratorPays, vecteurPaysFilms.end());

		//Pour les Genres
	std::vector<const Film* >& vecteurGenreFilms = filtreGenreFilms_[filmTrouve.genre]; //initialisation de la référence
	auto iteratorGenre = remove(vecteurGenreFilms.begin(), vecteurGenreFilms.end(), iteratorTrouve->get());
	vecteurGenreFilms.erase(iteratorGenre, vecteurGenreFilms.end());

	//TODO: effacer le film du vecteur de unique_ptr de Films
	films_.erase(iteratorTrouve);
	return true;
}

/// Retourne le nombre de films présentement dans le gestionnaire.
/// \return             Le nombre de films présentement dans le gestionnaire.
std::size_t GestionnaireFilms::getNombreFilms() const
{
	return films_.size();
}

/// Trouve et retourne un film en le cherchant à partir de son nom.
/// \param nom          Le nom du film à trouver.
/// \return             Un pointeur vers le film ou nullptr si aucun film avec le nom ne peut être trouvé.
const Film* GestionnaireFilms::getFilmParNom(const std::string& nom) const
{
	auto film = filtreNomFilms_.find(nom);
	if (film == filtreNomFilms_.end())
	{
		return nullptr;
	}
	return film->second;
}

/// Retourne une copie de la liste des films appartenant à un genre donné.
/// \param genre        Le genre du film à trouver.
/// \return             Une copie de la liste des films appartenant à un genre donné.
std::vector<const Film*> GestionnaireFilms::getFilmsParGenre(Film::Genre genre) const
{
	auto it = filtreGenreFilms_.find(genre);
	if (it == filtreGenreFilms_.end())
	{
		return std::vector<const Film*>();
	}
	return filtreGenreFilms_.at(genre);
}

/// Retourne une copie de la liste des films appartenant à un pays donné.
/// \param pays         Le pays du film à trouver.
/// \return             Une copie de la liste des films appartenant à un pays donné.
std::vector<const Film*> GestionnaireFilms::getFilmsParPays(Pays pays) const
{
	auto it = filtrePaysFilms_.find(pays);
	if (it != filtrePaysFilms_.end())
	{
		return filtrePaysFilms_.at(pays);
	}
	return std::vector<const Film*>();
}

/// Retourne une liste des films produits entre deux années passées en paramètre.
/// \param anneeDebut   Année définissant le début de notre intervalle de recherche des films.
/// \param anneeFin     Année définissant la fin de notre intervalle de recherche des films.
/// \return             Liste des films produits entre deux années.
std::vector<const Film*> GestionnaireFilms::getFilmsEntreAnnees(int anneeDebut, int anneeFin)
{
	std::vector<const Film*> vecteurRetour;

	//Utiliser foncteur EstDansIntervalleDatesFilm(anneeDebut, anneeFin)
	copy_if(films_.begin(), films_.end(), RawPointerBackInserter(vecteurRetour), EstDansIntervalleDatesFilm(anneeDebut, anneeFin));
	return vecteurRetour;
}


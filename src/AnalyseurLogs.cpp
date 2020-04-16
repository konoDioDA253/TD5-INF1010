/// Analyseur de statistiques grâce aux logs.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#include "AnalyseurLogs.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include "Foncteurs.h"

/// Ajoute les lignes de log en ordre chronologique à partir d'un fichier de logs.
/// \param nomFichier               Le fichier à partir duquel lire les logs.
/// \param gestionnaireUtilisateurs Référence au gestionnaire des utilisateurs pour lier un utilisateur à un log.
/// \param gestionnaireFilms        Référence au gestionnaire des films pour pour lier un film à un log.
/// \return                         True si tout le chargement s'est effectué avec succès, false sinon.
bool AnalyseurLogs::chargerDepuisFichier(const std::string& nomFichier,
                                         GestionnaireUtilisateurs& gestionnaireUtilisateurs,
                                         GestionnaireFilms& gestionnaireFilms)
{
    std::ifstream fichier(nomFichier);
    if (fichier)
    {
        logs_.clear();
        vuesFilms_.clear();

        bool succesParsing = true;

        std::string ligne;
        while (std::getline(fichier, ligne))
        {
            std::istringstream stream(ligne);

            std::string timestamp;
            std::string idUtilisateur;
            std::string nomFilm;

            if (stream >> timestamp >> idUtilisateur >> std::quoted(nomFilm))
            {
                // TODO: Uncomment une fois que la fonction creerLigneLog est écrite
                 creerLigneLog(timestamp, idUtilisateur, nomFilm, gestionnaireUtilisateurs, gestionnaireFilms);
            }
            else
            {
                std::cerr << "Erreur AnalyseurLogs: la ligne " << ligne
                          << " n'a pas pu être interprétée correctement\n";
                succesParsing = false;
            }
        }
        return succesParsing;
    }
    std::cerr << "Erreur AnalyseurLogs: le fichier " << nomFichier << " n'a pas pu être ouvert\n";
    return false;
}

/// Crée et ajoute une ligne de log dans le vecteur de logs
/// \param timestamp                Ordre de classement du ligneLog dans le vecteur logs_.
/// \param idUtilisateur            l'identifiant de l'utilisateur.
/// \param nomFilm                  le nom du film de la ligneLog.
/// \param gestionnaireUtilisateurs Référence au gestionnaire des utilisateurs pour lier un utilisateur à un log.
/// \param gestionnaireFilms        Référence au gestionnaire des films pour pour lier un film à un log.
/// \return                         True si le film et l’utilisateur existaient et le log a été ajouté avec succès, false sinon.
bool AnalyseurLogs::creerLigneLog(const std::string& timestamp, const std::string& idUtilisateur, const std::string& nomFilm,
	GestionnaireUtilisateurs& gestionnaireUtilisateurs, GestionnaireFilms& gestionnaireFilms)
{
	LigneLog ligneLog = {timestamp, (gestionnaireUtilisateurs.getUtilisateurParId(idUtilisateur)), (gestionnaireFilms.getFilmParNom(nomFilm))};
	if (gestionnaireFilms.getFilmParNom(nomFilm) == nullptr || gestionnaireUtilisateurs.getUtilisateurParId(idUtilisateur) == nullptr)
	{
		return false;
	}
	ajouterLigneLog(ligneLog);
	return std::binary_search(logs_.begin(), logs_.end(), ligneLog, ComparateurLog());
}

/// Ajoute une ligne de log en ordre chronologique dans le vecteur de logs tout en mettant à jour le nombre de vues.
/// \param ligneLog                 ligne de log à ajouter dans le vecteur de logs_.
/// \return                         Aucune valeur de retour.
void AnalyseurLogs::ajouterLigneLog(const LigneLog& ligneLog)
{
	//Insertion de la ligneLog dans le vecteur de logs_
	std::vector<LigneLog>::iterator it = std::lower_bound(logs_.begin(), logs_.end(), ligneLog, ComparateurLog());
	logs_.insert(it, ligneLog);

	//Incrémenter le nombre de vues du Film de ligneLog	
	vuesFilms_[ligneLog.film]++;
}

/// Retourne le nombre de vues pour un film passé en paramètre
/// \param film                     Film servant à donner le nombre de vues.
/// \return                         Nombre de vues du film.
int AnalyseurLogs::getNombreVuesFilm(const Film* film) const
{
	auto it = vuesFilms_.find(film);
	if (it == vuesFilms_.end())
	{
		return 0;
	}
	return vuesFilms_.at(film);
}

/// Retourne le film le plus regardé parmi les données chargées dans l’analyseur de logs. Si les logs sont vides(aucun film n’est trouvé), retourne nullptr
/// \return                         Le film le plus populaire parmi les données.
const Film* AnalyseurLogs::getFilmPlusPopulaire() const
{
	if (vuesFilms_.empty() == true)
	{
		return nullptr;
	}
	return std::max_element(vuesFilms_.begin(), vuesFilms_.end(), ComparateurSecondElementPaire<const Film* ,int>())->first;

}

/// Retourne une liste des films les plus regardés et leur nombre de vues parmi lesdonnées chargées dans l'analyseur de logs.
/// \param nombre                   Nombre de films les plus populaires à retourner.
/// \return                         Liste des films les plus regardés accompagnés de leur nombre de vues.
std::vector<std::pair<const Film*, int>> AnalyseurLogs::getNFilmsPlusPopulaires(std::size_t nombre) const
{
	//std::min(nombre, vuesFilms_.size());
	std::vector<std::pair<const Film*, int>> vecteurFilmsPopulaires(std::min(nombre, vuesFilms_.size()));
	std::partial_sort_copy(vuesFilms_.begin(), vuesFilms_.end(), vecteurFilmsPopulaires.begin(), vecteurFilmsPopulaires.end(), 
		[](const std::pair<const Film*, int>& paire_1, const std::pair<const Film*, int>& paire_2)
			{ return (paire_1.second > paire_2.second); });
	return vecteurFilmsPopulaires;
}

/// Retourne le nombre de films vus par utilisateur
/// \param utilisateur              Utilisateur pour lequel on veut compter le nombre de vues.
/// \return                         Nombre de films vus par un utilisateur.
int AnalyseurLogs::getNombreVuesPourUtilisateur(const Utilisateur* utilisateur) const
{
	return static_cast<int>(std::count_if(logs_.begin(), logs_.end(), [&utilisateur](const LigneLog& ligneLog) {return (ligneLog.utilisateur == utilisateur); }));
}

/// Retourne un vecteur des films uniques regardés par un utilisateur parmi les données chargées dans l'analyseur de logs.
/// \param utilisateur              Utilisateur pour lequel on veut compter le nombre de vues.
/// \return                         Vecteur des films uniques regardés par un utilisateur donné en paramètre.
std::vector<const Film*> AnalyseurLogs::getFilmsVusParUtilisateur(const Utilisateur* utilisateur) const
{
	std::unordered_set<const Film*> set;
	for (const auto& element : logs_)
	{
		if (utilisateur == element.utilisateur)
		{
			set.insert( element.film);
		}
	}
	return std::vector<const Film*>(set.begin(), set.end()) ;
}




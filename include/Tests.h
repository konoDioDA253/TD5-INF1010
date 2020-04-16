/// Tests automatisés.
/// \author Misha Krieger-Raynauld
/// \date 2020-03-04

#ifndef TESTS_H
#define TESTS_H

#define TEST_GESTIONNAIRE_UTILISATEURS_ACTIF /*false*/true
#define TEST_FONCTEURS_ACTIF /*false*/true
#define TEST_GESTIONNAIRE_FILMS_ACTIF /*false*/true
#define TEST_ANALYSEUR_LOGS_ACTIF /*false*/true

namespace Tests
{
    void testAll();
    double testGestionnaireUtilisateurs();
    double testFoncteurs();
    double testGestionnaireFilms();
    double testAnalyseurLogs();
} // namespace Tests

#endif // TESTS_H

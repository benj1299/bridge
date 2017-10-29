#pragma once

#include <stdio.h>
#include "Player.h"
#include "Cards.h"
#include "Functions.h"

typedef struct GameConfig GameConfig;
struct GameConfig {
    int levee_goal; //nombre de levées à réaliser
    int atout;
    int donneur;
    int avancement[]; //nombre de levées en cours
};

void CreateTeams(Player players[]);
void DistribCards(int nb_player, int nb_cards, Player players[4]);
void majuscule(char *chaine);
int ConvertCardsString(char card[8]);
int SumRaisesTeam(Player players[], int team);

/*
 Jeu d'enchère de début de partie
 */
int Auction(GameConfig game_config)
{
    int contrat_tmp = 10; //temporaire mettre à 0
    int contrat = 0;
    char card[7], color[8];
    int winner = -1;
    char passe[] = "PASSE", enchere[] = "ENCHERE";
    
    for (int i = 0; i < 4; i++)
    {
        char choice[10];
        int action = 1;
        
        while (action)
        {
            scanf("Entrer PASSE pour passer ou ENCHERE pour encherir : %s", choice);
            majuscule(choice);
            
            if (strcmp(choice, passe) == 0) {
                action = 0;
            }
            
            else if(strcmp(choice, enchere) == 0)
            {
                scanf("Entrez une carte : %s", card);
                scanf("Entrez une couleur : %s", color);
                
                majuscule(card);
                majuscule(color);
                
                if(ConvertCardsString(card) > 7){
                    printf("La carte utilisée n'est pas autorisé.\n");
                    continue;
                }
                
                //contrat_tmp = ConvertCardsString(card) + ConvertColorString(color);
                
                if (contrat_tmp > contrat)
                {
                    contrat = contrat_tmp;
                    contrat_tmp = 0;
                    
                    game_config.levee_goal = 6 + ConvertCardsString(card);
                    game_config.atout = ConvertColorString(color);
                    winner = i;
                    
                    action = 0;
                }
                
                else
                {
                    printf("L'enchère est trop basse, veuillez en sélectionner une plus haute ou passer\n");
                    continue;
                }
            }
            
            else {continue;}
        }
    }
    return winner;
}

/*
 End Game
 */
int EndGame(Player players[], GameConfig game_config){
    char choice[5], stop[] = "STOP";
    int team;
    
    for (int i = 0; i < 4; i++) {
        printf("%s a réalisé %d levées\n", players[i].name_player, players[i].nb_raises);
    }
    
    printf("L'équipe Nord-Sud a réalisé %d levées et l'équipe Est-Ouest a réalisé %d levées\n",
      SumRaisesTeam(players, 0), SumRaisesTeam(players, 1));
    
    for (int i = 0; i < 4; i++) {
        if (players[i].role == 1) {
            
            if (i < 2) {team = SumRaisesTeam(players, 0)}
            else {team = SumRaisesTeam(players, 1)}
            
            if (SumRaisesTeam(players, team) >= game_config.levee_goal) {
                printf("%s a réaliser son contrat, l'équipe gagne");
            }
            
        }
    }
    
    scanf("Taper STOP pour arrêter de jouer ou appuyer sur entrer pour continuer :", choice);
    majuscule(choice);
    if(strcmp(choice,stop) == 0){return 1;}
    else
    {
        for (int i=0; i < 4; i++) {
            players[i].nb_points_total += players[i].nb_raises;
        }
        return 0;
    }
    
}


/*
 Déclancheur de partie
 */
void CreateGame(int nb_player, int nb_card)
{
    Player players[4];
    GameConfig game_config;
    
    while (1){
        CreateTeams(players);
        DistribCards(nb_player, nb_card, players);
        game_config.donneur = RandRange(0, 4);
        
        int winner = Auction(game_config);
        players[winner].role = 1;
        if (EndGame(players, game_config)) {break;}
    }
    
    printf("Merci d'avoir joué a notre super jeu, on vous kiff <3 !");
}

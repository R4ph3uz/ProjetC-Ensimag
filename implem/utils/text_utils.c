#include "text_utils.h"
#include <math.h>
#include <stdlib.h>
#include "ei_draw.h"
#include "ei_utils.h"
#include "ei_types.h"
#include <string.h>
/*--------------------------------------------------------------------------------------------------------------------------*/

char* restrict_text(char* text, uint8_t taille) {
    char* res = malloc(sizeof(char)*(taille+1) ) ;
    strncpy(res, text, taille);
    res[taille] = '\0';
    return res;
}

/*--------------------------------------------------------------------------------------------------------------------------*/
char*  insert_char(char* text, char character, uint8_t where) {
    int taille = (int) strlen(text)+1;
    char* res = malloc(sizeof(char)*(taille+1));

    strncpy(res,text,where);
    res[where] = character;
    for(int i = where+1; i< taille;i++)
        res[i] = text[i-1];
    res[taille] = '\0';
    free(text);
    return res;
}

/*--------------------------------------------------------------------------------------------------------------------------*/

char* insert_word(char* text, const char* word, uint8_t where) {
    // Taille initiale du texte et du mot à insérer
    int taille_text = strlen(text);
    int taille_word = strlen(word);
    // Nouvelle taille après insertion du mot
    int nouvelle_taille = taille_text + taille_word;

    // Allocation de mémoire pour la nouvelle chaîne de caractères
    char* res = malloc(sizeof(char) * (nouvelle_taille + 1)); // +1 pour le caractère nul de fin

    // Copier la partie du texte avant l'insertion
    strncpy(res, text, where);

    // Copier le mot à insérer
    strncpy(res + where, word, taille_word);

    // Copier la partie restante du texte après l'insertion
    strcpy(res + where + taille_word, text + where);

    // Libérer la mémoire du texte original
    free(text);

    // Retourner la nouvelle chaîne de caractères
    return res;
}

/*--------------------------------------------------------------------------------------------------------------------------*/

char * delete_char(char *text, uint8_t where) {
    int taille = strlen(text)-1;
    if(0<where && where <=taille+1 ) {
        char* res =malloc(sizeof(char)*(taille+1));

        strncpy(res,text,where-1);
        for(int i = where-1; i< taille;i++)
            res[i] = text[i+1];
        res[taille] = '\0';

        return res;
    }
    return text;
}

/*--------------------------------------------------------------------------------------------------------------------------*/

int find_selection_entry(ei_entry_t entry, ei_point_t position) {
    char* temp = malloc(sizeof(char)*(strlen(entry->text)+1));
    int width,height;
    int i;
    for(i = 0 ; i < strlen(entry->text); i++) {
        temp[i] = entry->text[i];
        temp[i+1] = '\0';
        hw_text_compute_size(temp, *entry->text_font, &width, &height);
        if(position.x < entry->widget.screen_location.top_left.x + width -entry->decal_x ) {
            temp[i] = '\0';
            hw_text_compute_size(temp, *entry->text_font, &width, &height);
            free(temp);

            return entry->widget.screen_location.top_left.x + width -entry->decal_x;
        }
    }

    free(temp);
    return entry->widget.screen_location.top_left.x + width -entry->decal_x;

}
/*--------------------------------------------------------------------------------------------------------------------------*/
char* cut_text(char* text, uint8_t debut, uint8_t fin) {
    int taille_cut = fin-debut;
    char* res = malloc(sizeof(char)*(strlen(text)-taille_cut+1) ) ;
    strncpy(res,text,debut);
    for(int i = fin; i < strlen(text); i++)
        res[i-taille_cut] = text[i];
    res[strlen(text)-taille_cut] = '\0';
    return res;
}

/*--------------------------------------------------------------------------------------------------------------------------*/

int find_position_cursor_selection_entry(ei_entry_t entry, ei_point_t position) {
    char* temp = malloc(sizeof(char)*(strlen(entry->text)+1));
    int width,height;
    for(int i = 0 ; i < strlen(entry->text); i++) {
        temp[i] = entry->text[i];
        temp[i+1] = '\0';
        hw_text_compute_size(temp, *entry->text_font, &width, &height);
        if(position.x < entry->widget.screen_location.top_left.x + width -entry->decal_x ) {
            free(temp);
            return i;
        }
    }
    free(temp);
    return strlen(entry->text);
}

/*--------------------------------------------------------------------------------------------------------------------------*/
void find_word(char* text, int where, int* debut, int*fin){
    *debut = 0 ;
    for(int i = where-1 ; i >= 0; i--){
        if(text[i] == ' '){
            *debut = i+1;
            break;
        }

    }

    *fin = strlen(text);
    for(int i = where+1;  i < strlen(text);i++ ){
        if(text[i] == ' '){
            *fin = i;
            break;
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------------------*/

char* texte_selectionne(char* text, int32_t debut, int32_t fin){
    int taille_cut = fin-debut;
    char* res = malloc(sizeof(char)*(strlen(text)-taille_cut+1) ) ;
    for(int i = debut; i < fin; i++)
        res[i-debut] = text[i];
    res[taille_cut] = '\0';
    return res;
}

/*--------------------------------------------------------------------------------------------------------------------------*/

int bypass_control(const char *string, int position, int direction) {
    int len = strlen(string);

    // Vérification des bornes de la position initiale
    if (position < 0 || position >= len) {
        return position; // Position initiale hors des limites, renvoie la position initiale
    }
    if (direction != 1 && direction != -1) {
        return -1; // Direction invalide
    }

    // Gestion des cas où la direction dépasse les limites de la chaîne
    if ((position == 0 && direction == -1) || (position == len - 1 && direction == 1)) {
        return position; // Ne pas dépasser les limites, renvoie la position initiale
    }

    // Avancer ou reculer pour sauter les espaces initiaux
    while (position >= 0 && position < len && string[position] == ' ') {
        position += direction;
    }

    // Avancer ou reculer pour passer les caractères alphanumériques
    while (position >= 0 && position < len && isalnum(string[position])) {
        position += direction;
    }

    // Gestion des cas où on a dépassé les limites après les boucles
    if (position < 0 || position > len) {
        position -= direction; // Revient à la dernière position valide
    }

    // Retourner la position finale
    return position;
}
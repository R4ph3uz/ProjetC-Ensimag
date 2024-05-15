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
    int taille = strlen(text)+1;
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

char * delete_char(char *text, uint8_t where) {
    int taille = strlen(text)-1;
    if(0<where && where <=taille+1 ) {
        char* res =malloc(sizeof(char)*(taille+1));

        for(int i = 0 ; i <where-1; i++)
            res[i] = text[i];
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
    for(int i = 0 ; i< debut ; i++)
        res[i] = text[i];
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
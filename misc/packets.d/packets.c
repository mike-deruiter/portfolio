#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER  100

// function prototypes
void buff_alloc();
int read_packet();
void sort_pvector();
void print_pvector();
char *trim_whitespace(char *s, int size);

typedef struct {
    int message_id;
    int packet_id;
    int total_packets;
    char *message;
} packet;

int buf_mult;
packet *p_vector;
int v_size;

int main(int argc, char *argv[])
{

    buf_mult = 1;
    v_size = 0;
    
    buff_alloc();
    
    while (read_packet()) {
    }
    
    sort_pvector();
    
    print_pvector();
}

void buff_alloc()
{
    p_vector = malloc(sizeof(char) * (BUFFER * buf_mult++));
}

int read_packet()
{
    size_t buf_size = 80;

    if (v_size >= (BUFFER * buf_mult)) {
        buff_alloc();
    }

    if (scanf("%d", &p_vector[v_size].message_id) > 0 &&
        scanf("%d", &p_vector[v_size].packet_id) > 0 &&
        scanf("%d", &p_vector[v_size].total_packets) > 0) {
        
        p_vector[v_size].message = malloc(sizeof(char) * 81);
        
        if (getline(&(p_vector[v_size].message), &buf_size, stdin) > 0) {
            p_vector[v_size].message = 
                trim_whitespace(p_vector[v_size].message, 80);
            ++v_size;
        } else {
            return false;
        }
    } else {
        return false;
    }
    
    return true;
}

char *trim_whitespace(char *s, int s_size)
{
    char *t = s;
    char *rtn = malloc(sizeof(char) * (s_size + 1));
    
    for (; *s == ' ' || *s == '\t'; *s++) {}
    
    strcpy(rtn, s);
    free(t);
    
    return rtn;
}

void sort_pvector()
{
    int i, j, k;

    packet temp;

    // perform insertion sort by message_id
    for (i = 1; i < v_size; ++i) {
        temp = p_vector[i];
        for (j = i - 1; j >= 0 && 
             p_vector[j].message_id > temp.message_id; --j) {
             
            p_vector[j+1] = p_vector[j];
        }   // 2nd for loop
        p_vector[j+1] = temp;
    }       // 1st for loop
    
    // now sort each section by packet_id
    for (k = 0; k < v_size; k += p_vector[k].total_packets) {
        for (i = k + 1; i < k + p_vector[k].total_packets; ++i) {
            temp = p_vector[i];
            for (j = i - 1; j >= k && 
                 p_vector[j].packet_id > temp.packet_id; --j) {
            
                p_vector[j+1] = p_vector[j];
            } // 3rd for loop
            p_vector[j+1] = temp;
        }     // 2nd for loop
    }         // 1st for loop
}             // function

void print_pvector()
{
    int i;
    
    for (i = 0; i < v_size; ++i) {   
        printf("%4d    %-2d  %-2d  %s", p_vector[i].message_id,
               p_vector[i].packet_id, p_vector[i].total_packets,
               p_vector[i].message);
    }
}

#include <pthread.h>

struct station {
    int passenger_wait;
    int Avaible_seats;
    int seats;
    pthread_mutex_t lock;
    pthread_mutex_t lock2;
    pthread_cond_t train_arrive ;
    pthread_cond_t waiting_passenger ;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
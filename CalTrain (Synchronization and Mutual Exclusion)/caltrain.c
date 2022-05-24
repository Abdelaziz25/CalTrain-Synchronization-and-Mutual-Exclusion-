#include <pthread.h>
#include "caltrain.h"
void
station_init(struct station *station)
{
    station->seats=0;
    station->passenger_wait=0;
    pthread_mutex_init(&station->lock,NULL);
    pthread_cond_init(&station->train_arrive,NULL);
    pthread_cond_init(&station->waiting_passenger,NULL);
}
void
station_load_train(struct station *station, int count)
{
    /// lock as no more than train access this critical section at the same time
    pthread_mutex_lock(&(station->lock));
    station->Avaible_seats=count;
    /// check if there is avaible seats and passenger waiting
    /// wait until passengers become on board
    while(station->Avaible_seats!=0 && station->passenger_wait!=0)
    {
        pthread_cond_broadcast(&station->train_arrive);

        pthread_cond_wait(&station->waiting_passenger,&station->lock);
    }
    /// making seats equal zero for the next train
    station->Avaible_seats=0;
    pthread_mutex_unlock(&(station->lock));
}
void
station_wait_for_train(struct station *station)
{
    /// lock as no more than passenger access this critical section at the same time
    pthread_mutex_lock(&(station->lock));
    station->passenger_wait++;
    /// train is full so passengers need to wait
    while(station->seats == station->Avaible_seats)
    {
        pthread_cond_wait(&station->train_arrive,&station->lock);
    }
    station->seats++;
    pthread_mutex_unlock(&(station->lock));
}
void
station_on_board(struct station *station)
{
    /// lock as no more than passenger access this critical section at the same time
    pthread_mutex_lock(&(station->lock));
    station->seats--;
    station->passenger_wait--;
    station->Avaible_seats--;
    ///train wait until there is no empty seats or there is no waiting passenger.
    if(station->Avaible_seats==0 || (station->passenger_wait==0 && station->seats==0))
    {
        pthread_cond_signal(&station->waiting_passenger);
    }
    pthread_mutex_unlock(&(station->lock));
}
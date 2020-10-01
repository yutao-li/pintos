#include "pintos_thread.h"

struct station
{
	// FILL ME IN
	int has_train, seat, board, waiting;
	struct lock lo;
	struct condition train, passenger, load;
};

void station_init(struct station *station)
{
	// FILL ME IN
	station->has_train = 0;
	station->waiting = 0;
	lock_init(&station->lo);
	cond_init(&station->train);
	cond_init(&station->passenger);
	cond_init(&station->load);
}

void station_load_train(struct station *station, int count)
{
	// FILL ME IN
	if (count == 0)
		return;
	lock_acquire(&station->lo);
	while (station->has_train != 0)
		cond_wait(&station->train, &station->lo);
	if (station->waiting > 0)
	{
		station->has_train = 1;
		station->seat = count;
		station->board = count;
		cond_signal(&station->passenger, &station->lo);
		cond_wait(&station->load, &station->lo);
		cond_signal(&station->train, &station->lo);
	}
	lock_release(&station->lo);
}

void station_wait_for_train(struct station *station)
{
	// FILL ME IN
	lock_acquire(&station->lo);
	station->waiting += 1;
	while (station->has_train == 0 || station->seat == 0)
		cond_wait(&station->passenger, &station->lo);
	station->seat -= 1;

	if (station->seat > 0)
		cond_signal(&station->passenger, &station->lo);
	lock_release(&station->lo);
}

void station_on_board(struct station *station)
{
	// FILL ME IN
	lock_acquire(&station->lo);
	station->board -= 1;
	station->waiting -= 1;
	if (station->board == 0 || station->waiting == 0)
	{
		station->has_train = 0;
		cond_signal(&station->load, &station->lo);
	}
	lock_release(&station->lo);
}

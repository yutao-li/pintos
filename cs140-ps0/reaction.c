#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction
{
	// FILL ME IN
	struct lock lo;
	struct condition cond_h, cond_o;
	int num_h, num_o;
};

void reaction_init(struct reaction *reaction)
{
	// FILL ME IN
	lock_init(&reaction->lo);
	cond_init(&reaction->cond_h);
	cond_init(&reaction->cond_o);
	reaction->num_h = 0;
	reaction->num_o = 0;
}

void reaction_h(struct reaction *reaction)
{
	// FILL ME IN
	lock_acquire(&reaction->lo);
	reaction->num_h += 1;
	if (reaction->num_h >= 2 && reaction->num_o >= 1)
	{
		reaction->num_h -= 2;
		reaction->num_o -= 1;
		make_water();
		cond_signal(&reaction->cond_h, &reaction->lo);
		cond_signal(&reaction->cond_o, &reaction->lo);
	}
	else
		cond_wait(&reaction->cond_h, &reaction->lo);
	lock_release(&reaction->lo);
}

void reaction_o(struct reaction *reaction)
{
	// FILL ME IN
	lock_acquire(&reaction->lo);
	reaction->num_o += 1;
	if (reaction->num_h >= 2)
	{
		reaction->num_h -= 2;
		reaction->num_o -= 1;
		make_water();
		cond_signal(&reaction->cond_h, &reaction->lo);
		cond_signal(&reaction->cond_h, &reaction->lo);
	}
	else
		cond_wait(&reaction->cond_o, &reaction->lo);
	lock_release(&reaction->lo);
}

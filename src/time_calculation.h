/*
 authors:
 -Cristina Parasiliti Parracello
 -Dario Safarally
 -Roberta Maccarrone
 */

void time(struct timeval);

/*Function that return the time of function execution*/
void time(struct timeval start_time) {
struct timeval end_time;
gettimeofday(&end_time,NULL);
double start_dtime=(double)start_time.tv_sec+(double)start_time.tv_usec/1000000.0;
double end_dtime=(double)end_time.tv_sec+(double)end_time.tv_usec/1000000.0;
double diff=end_dtime-start_dtime;
printf("\n Execution time: %lf seconds\n",diff);
}

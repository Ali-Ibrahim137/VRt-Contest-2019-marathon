# VRt-Contest-2019-marathon

I used a very simple greedy solution to solve the problem.

The idea is to divide the workers into groups of size $7$, I find the path for the first worker, While choosing the next location I always take the location that starts first, taking into consideration the start time of this job, and the current time + time to go to this location, after the first worker of the group is over there will be some gaps in the current locations still not done, I try to fill this gaps with jobs that need at most $6$ workers and so on until all this jobs are done for this group, this way I guarantee that all the jobs are been worked by simultaneity.

Every time after I use a new worker I calculate the profit to check if using this worker gives me extra profit or not. The idea can be improved a lot, but unfortunately I was busy all week with some other stuff.


Link to the contest on codeforces:

[Dashboard](https://codeforces.com/contest/1160)

clf;

subplot(611);
y=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\theory_rt.txt',31,1);
plot(y, "-r");
z=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\practice_rt.txt',31,1);
plot(z,"+Black");
xtitle("Mean Response Time Evaluation");

subplot(612);
y=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\theory_wt.txt',31,1);
plot(y, "-r");
z=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\practice_wt.txt',31,1);
plot(z,"+Black");
xtitle("Mean Waiting Time Evaluation");

subplot(613);
y=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\theory_ql.txt',31,1);
plot(y, "-r");
z=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\practice_ql.txt',31,1);
plot(z,"+Black");
xtitle("Mean Queue Length Evaluation");

subplot(614);
y=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\theory_en.txt',31,1);
plot(y, "-r");
z=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\practice_en.txt',31,1);
plot(z,"+Black");
xtitle("Mean Number of Customers Evaluation");

subplot(615);
y=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\theory_u.txt',31,1);
plot(y, "-r");
z=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\practice_u.txt',31,1);
plot(z,"+Black");
xtitle("Utilization Evaluation");

subplot(616);
y=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\theory_i.txt',31,1);
plot(y, "-r");
z=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\practice_i.txt',31,1);
plot(z,"+Black");
xtitle("Idle Time Evaluation");


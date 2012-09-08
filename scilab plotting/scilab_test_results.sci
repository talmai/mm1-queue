clf;

subplot(521);
q1s=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\01_service.txt',4000,1);
q1s_s=sort(q1s);
xtitle("Service Time Distribution - Check-In Queue");
plot(q1s_s,"+g");
subplot(522);
q1w=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\01_wait.txt',4000,1);
q1w_s=sort(q1w);
xtitle("Waiting Time Distribution - Check-In Queue");
plot(q1w_s,"+g");

subplot(523);
q2s=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\02_service.txt',4000,1);
q2s_s=sort(q2s);
xtitle("Service Time Distribution - Security Gate 01 Queue");
plot(q2s_s,"+b");
subplot(524);
q2w=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\02_wait.txt',4000,1);
q2w_s=sort(q2w);
xtitle("Waiting Time Distribution - Security Gate 01 Queue");
plot(q2w_s,"+b");

subplot(525);
q3s=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\03_service.txt',4000,1);
q3s_s=sort(q3s);
xtitle("Service Time Distribution - Security Gate 02 Queue");
plot(q3s_s,"+r");
subplot(526);
q3w=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\03_wait.txt',4000,1);
q3w_s=sort(q3w);
xtitle("Waiting Time Distribution - Security Gate 02 Queue");
plot(q3w_s,"+r");

subplot(527);
q4s=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\04_service.txt',4000,1);
q4s_s=sort(q4s);
xtitle("Service Time Distribution - Security Gate 03 Queue");
plot(q4s_s,"+y");
subplot(528);
q4w=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\04_wait.txt',4000,1);
q4w_s=sort(q4w);
xtitle("Waiting Time Distribution - Security Gate 03 Queue");
plot(q4w_s,"+y");

subplot(529);
q5s=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\05_service.txt',4000,1);
q5s_s=sort(q5s);
xtitle("Service Time Distribution - ID and X-ray Queue");
plot(q5s_s,"+Black");
subplot(5,2,10);
q5w=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source_code\05_wait.txt',4000,1);
q5w_s=sort(q5w);
xtitle("Waiting Time Distribution - ID and X-ray  Queue");
plot(q5w_s,"+Black");


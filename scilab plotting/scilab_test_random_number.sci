clf;

subplot(421);
y=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source\rnd_01.txt',1000,1);
plot(y, "+r");
xtitle("RND Exponential # Values - Seed: 1");
z=sort(y);
subplot(422);
plot(z,"-r");
xtitle("Sorted Numbers");

subplot(423);
a=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source\rnd_02.txt',1000,1);
plot(a,"+g");
xtitle("RND Exponential # Values - Seed: 7.51391e+008");
b=sort(a);
subplot(424);
plot(b,"-g");
xtitle("Sorted Numbers");

subplot(425);
g=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source\rnd_03.txt',1000,1);
plot(g,"+b");
xtitle("RND Exponential # Values - Seed: 5.648e+008");
h=sort(g);
subplot(426);
plot(h,"-b");
xtitle("Sorted Numbers");

subplot(427);
g=read('C:\talmai\UC\20CS798001_perf_eval\simulation\source\rnd_03.txt',1000,1);
plot(g,"+Black");
xtitle("C++ rand() Exponential # Values - Seed: 1234");
h=sort(g);
subplot(428);
plot(h,"-Black");
xtitle("Sorted Numbers");


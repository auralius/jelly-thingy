clc;
clear all;

data = load ('output.txt');

figure;
hold on;

for i = 1 : 2: size(data,1) -  1
    x = [data(i,1) data(i+1,1)];
    y = [data(i,2) data(i+1,2)];
    z = [data(i,3) data(i+1,3)];
    line(x,y,z);
end

clc;
clear all;
n=100;
inputs =randperm(n);
% 产生1-100之间的随机整数序列
% multiplier = 2;
% B = arrayProduct(multiplier,inputs)
% matlab自带例子
out1 = test_mex1(inputs);
fprintf("the test_mex1 return result is %d\n",out1);
fprintf("---------------------------------------------\n");
out2 = test_mex2(inputs);
fprintf("the test_mex2 return result is %d\n",out2);

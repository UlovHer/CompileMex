clc;
clear all;
n=100;
inputs =randperm(n);
% ����1-100֮��������������
% multiplier = 2;
% B = arrayProduct(multiplier,inputs)
% matlab�Դ�����
out1 = test_mex1(inputs);
fprintf("the test_mex1 return result is %d\n",out1);
fprintf("---------------------------------------------\n");
out2 = test_mex2(inputs);
fprintf("the test_mex2 return result is %d\n",out2);

# <center>Matlab中调用C/C++编译生成mex文件</center>
- ### MEX文件
&ensp;&ensp;&ensp;&ensp;&ensp;MEX(即 MEX 可执行程序二进制文件)指自动加载的、可以像任何 MATLAB 函数一样调用的程序。Matlab中矩阵预算特别方便，但如果有无法避免的循环甚至多层嵌套的话，会非常影响程序的效率，因此通常会将这种需要大量循环的模块用c++编写然后在matlab中调用。Matlab中的的c++编程称为mex编程(matlab executive)，其中需要些c++源文件，然后在Matlab中用mex命令编译它，然后就可以在Matlab中像调用函数一样调用c++代码了。Windows(64 位)扩展名mexw64。

------------
- ##### 创建方法：
&ensp;&ensp;&ensp;&ensp;&ensp;参考Matlab自带说明文档的 arrayProduct.c示例编写C/C++面向过程的源码文件。mex的源文件一定要include头文件"mex.h“，mex源文件的入口函数为void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])，nlhs( number left hand s):左边参数个数，也就是matlab函数输出值得个数，mxArray *plhs[]是一个指针数组，数组中的每一个元素都是一个指针，指向输出的矩阵；nrhs 是右边参数个数，也就是输入参数的个数，mxArray *prhs[]数组中的每个指针指向输入矩阵。mxGetPr(i)函数可以返回一个double*型的指针，指向矩阵的第i个元素，在matlab代码中调用：mex(a)，那也就是 prhs[0]是输入矩阵a的地址。或者使用mxGetDoubles(prhs[1])，也可以获取到输入矩阵a。(Matlab中默认的数据类型是double)
取到输入矩阵a。(Matlab中默认的数据类型是double)
```cpp
	#include "mex.h"
	#include <iostream>
	
	double add(double* inputs, int len, double* out)
	{
		std::cout << "*****come into add function*****" << std::endl;
		//printf("*****come into add function*****\n");
		double sum = 0;
		for (int i = 0; i < len; i++)
		{
			sum += *(inputs + i);
		}
		*out = sum;
		//不能返回sum地址,sum为局部变量
		return sum;
	}
	
	void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
	{
		double* input = mxGetPr(prhs[0]);
		// 取得输入矩阵第一个数据指针
		int m = mxGetM(prhs[0]);
		int n = mxGetN(prhs[0]);
		// 得到输入矩阵A的行数和列数
		//printf("inputs args numbers %d\n", nrhs);
		//printf("output args numbers %d\n", nlhs);
		std::cout << "inputs args numbers " << nrhs << "\toutput args numbers " << nlhs << std::endl;
		//printf("%d %d %f\n", m, n, *input);
		std::cout << "inputs args numbers are ";
		for (int i = 0; i < m*n; i++)
		{
			std::cout << *(input + i) << "\t";
			//printf("the input argvs are %f\t", *(input + i));
		}
		std::cout << std::endl;
		//printf("\n");
		//printf("*****call add function*****\n");
		std::cout << "*****call add function*****" << std::endl;
		plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
		// 为输出矩阵B分配存储空间
		double* out_matrix = mxGetPr(plhs[0]);
		// 取得输出矩阵第一个数据指针
		double res = add(input, m*n, out_matrix);
		// 调用add函数
		std::cout << "the sum is " << res << std::endl;
		//printf("the sum is %f\n", res);
}
```
&ensp;&ensp;&ensp;&ensp;&ensp;编写C++面向过程的源文件，遵循C++ MEX 函数的基本设计结构。C++ MEX 函数作为从 matlab mex::Function 继承的名为 MexFunction 的类来实现。MexFunction 类将覆盖函数调用运算符 operator()。此实现会创建一个可以像函数一样调用的函数对象。从 Matlab 中调用 MEX 函数将实例化此函数对象，其状态在对同一个 MEX 函数的后续调用中保持不变。下面是 C++ MEX 函数的基本设计。它是 matlab mex::Function 的子类，必须命名为 MexFunction。MexFunction 类将覆盖函数调用运算符 operator()。
```cpp
	#include "mex.hpp"
	#include "mexAdapter.hpp"
	
	class MexFunction : public matlab::mex::Function {
	public:
	    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {
	        // Function implementation
	        ...
	    }
	};
```
&ensp;&ensp;&ensp;&ensp;&ensp;MEX 函数的输入和输出作为 matlab::mex::ArgumentList 中的元素进行传递。每个输入或输出参数都是matlab::mex::ArgumentList 中包含的一个 matlab::data::Array。

------------
- ##### 使用方法
a. 选择C/C++编译器，在Matlab命令行窗口中输入mex -setup/mex -setup c/mex -setup cpp可选择需要VS或者GCC编译器
<br>![img1](https://github.com/UlovHer/CompileMex/blob/master/imgs/1.png)<br>
b. 将test_mex.cpp文件考到Matlab的工作文件夹下，mex -v -compatibleArrayDims test_mex.cpp编译文件，生成test_mex.mexw64文件。接着像调用Matlab函数一样调用该文即可。
<br>![img2](/imgs/2.png)<br>
c. 要调用 MEX 函数，请使用该文件的名称，但不带文件扩展名。调用语法取决于 MEX 函数定义的输入和输出参数。调用时，MEX 文件必须位于 MATLAB 路径或当前工作文件夹中。
```cpp
		clc;
		clear all;
		n=100;
		inputs =randperm(n);
		out = test_mex(inputs);
		// 根据程序实现功能输入参数
		fprintf("the mex return result is %d\n",out);
```
<br>![img3](/imgs/3.png)<br>

------------
- ##### 与EXE区别
&ensp;&ensp;&ensp;&ensp;&ensp;Mex文件与Matlab解释器在同一个过程空间运行，当用户调用一个MEX文件时Matlab解释器就会动态的连接到MEX文件。可独立运行的C/C++应用程序(exe)可以独立于Matlab环境而运行，而MEX文件则不行，它需要Matlab的支持。还有EXE中可以调用MEX文件。

------------
- ##### 参考资料
&ensp;&ensp;&ensp;&ensp;&ensp;C MEX 源文件编写：https://ww2.mathworks.cn/help/matlab/matlab_external/standalone-example  
&ensp;&ensp;&ensp;&ensp;&ensp;C++ MEX 源文件编写：https://ww2.mathworks.cn/help/matlab/matlab_external/c-mex-source-file.html  
&ensp;&ensp;&ensp;&ensp;&ensp;mex编译使用：https://ww2.mathworks.cn/help/matlab/ref/mex.html

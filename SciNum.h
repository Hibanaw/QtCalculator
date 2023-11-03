#ifndef SCINUM_H
#define SCINUM_H

#include<iostream>
#include<stack>
#include<queue>
#include<cmath>
#include<cstring>

class SciNum
{
	private:
		int symbol;//符号，1为正-1为负
		int coefficient[200];//从低到高分别为10^-99~10^100次方的系数。10^0系数为coefficient[100]
		double mantissa;//科学计数法的尾数1.23*10^5的1.23
		int exponent;//科学计数法的指数 1.23*10^5的5
		double num;//该数的值
		int error_flag;//表示是否报错
		std::string error;//报错信息

        static int compare(const SciNum &n1, const SciNum &n2);//比较，比n2大返回1，比n2小返回-1，相等返回0
	public: 
		SciNum();
        SciNum(const char *n);
		void show_num();
		void calculate_num();//计算科学计数法和浮点数存的值
		void calculate_sci_to_num();//使用科学计数法的值计算剩下的值
        SciNum operator+(const SciNum& n2) const;//重载加法
        SciNum operator-(const SciNum& n2) const;//重载减法
        SciNum operator*(const SciNum& n2) const;//重载乘法
        SciNum operator/(const SciNum& n2) const;//重载除法
        SciNum operator^(const SciNum& n2) const;//用于乘方
        static SciNum sin(const SciNum n);
        static SciNum cos(const SciNum n);
		static SciNum calculateExpression(const char *formula, const SciNum& Ans);

		double getNum();
		int getExp();
		double getMantisaa();
		int isError();//判断是否出错
		std::string errorMessage();
		void setError(std::string errorMessage);


};

#endif

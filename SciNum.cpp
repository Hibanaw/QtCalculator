#include "SciNum.h"

SciNum::SciNum()
{
	memset(coefficient,0,sizeof(int)*200);
	symbol=0;
	error="\0";
	error_flag=0;
	mantissa=0;
	exponent=0;
}

SciNum::SciNum(char *n)
{
	int decimal_point_flag=0;//代表找到的小数点的数量
	std::stack<char>int_part;

	{//先都赋成0
		memset(coefficient,0,sizeof(int)*200);
		error="\0";
		error_flag=0;
		mantissa=0;
		exponent=0;
		symbol=1;
	}

	char*p=n;
	if(*p=='-')
	{
		symbol=-1;
		p++;
	}
	while(*p>='0'&&*p<='9')
	{
		int_part.push(*p);
		p++;
	}
	if(int_part.size()==0)//第一个数就不是数字 语法错误
	{
		error_flag=1;
		error="语法错误(数首位不为数字)";
		return;
	}
	
	//当前输入的数字为整数部分
	exponent=int_part.size()-1;//数字位数减一为其的指数项
	char c;
	double temp_num=1;//储存10的exponent-i次方
	for(int i=int_part.size()-1;i>=0;i--)
	{
		c=int_part.top();
		int_part.pop();
		coefficient[100+exponent-i]=c-'0';
		num+=temp_num*(c-'0');
		temp_num*=10;
	}
	
	if(*p!='.'&&*p!='\0')
	{
		error_flag=1;
		error="语法错误（数结尾异常）";
		return;
	}
	else if(*p=='.')//开始输入小数部分
	{
		p++;
		std::queue<char>dec_part;
		while(*p>='0'&&*p<='9')
		{
			dec_part.push(*p);
			p++;
		}
		if(dec_part.size()==0)//第一个数就不是数字 语法错误
		{
			error_flag=1;
			error="语法错误(小数首位不为数字)";
			return;
		}
		if(*p!='\0')
		{
			error_flag=1;
			error="语法错误(数结尾异常)";
			return;
		}
		temp_num=0.1;
		for(int i=1;dec_part.size()>0;i++)
		{
			c=dec_part.front();
			dec_part.pop();
			coefficient[100-i]=c-'0';
			num+=temp_num*(c-'0');
			temp_num/=10;
		}

		if(exponent==0&&coefficient[100]==0)
		{
			int i;
			for(i=100;i>=0&&coefficient[i]==0;i--);//
			if(i==-1)exponent=0;
			else exponent=i-100;
		}
	}

	//计算尾数
	int i;
	for(i=199;coefficient[i]==0&&i>=0;i--);//寻找第一个有数的位
	if(i==-1)//说明该数为0
	{
		mantissa=0;
		symbol=0;
	}
	else//找到了一个有数的位（此数不为0）
	{
		double temp_mant=1;
		for(;i>=0;i--)
		{
			mantissa+=coefficient[i]*temp_mant;
			temp_mant/=10;
		}
	}

	if(symbol==-1)
	{
		mantissa*=-1;
		num*=-1;
	}
}

void SciNum::calculate_num()
{
	if(error_flag)return;

	mantissa=0;
	num=0;
	exponent=0;
	
	int i;
	for(i=199;i>=0&&coefficient[i]==0;i--);//找到第一个非0值
	if(i==-1)
	{
		symbol=0;
		return;
	}
	exponent=i-100;
	double temp_mant=1;
	for(;i>=0;i--)
	{
		mantissa+=coefficient[i]*temp_mant;
		temp_mant/=10;
	}
	if(exponent>=0)
	{
		num=mantissa;
		for(int j=0;j<exponent;j++)num*=10;
	}
	else
	{
		num=mantissa;
		for(int j=0;j>exponent;j--)num/=10;
	}
	if(symbol==-1)
	{
		mantissa*=-1;
		num*=-1;
	}
}

void SciNum::calculate_sci_to_num()
{
	if(error_flag)return;
	
	memset(coefficient,0,sizeof(int)*200);
	if(abs(mantissa)<1e-6)
	{
		symbol=0;
		num=0;
		return;
	}
	else if(mantissa>0)symbol=1;
	else if(mantissa<0)symbol=-1;

	double temp=mantissa;
	temp=abs(temp);
	for(int i=exponent+100;i>=0;i--)
	{
		coefficient[i]=(int)temp;
		temp*=10;
		while(temp>10)temp-=10;
		if(abs(temp)<1e-14)break;
	}

	this->calculate_num();
}

void SciNum::show_num()
{
	if(error_flag)
	{
		std::cout << error << std::endl;
		return;
	}
	std::cout << mantissa << "*10^" << exponent << std::endl;
	std::cout << num << std::endl;
}

int SciNum::compare(SciNum n2)
{
	if(symbol<n2.symbol)return -1;
	if(symbol>n2.symbol)return 1;
	int cmp=0;
	for(int i=199;i>=0;i--)
	{
		if(coefficient[i]==n2.coefficient[i])
		{
			if(i!=0)continue;
			else cmp=0;
		}
		else 
		{
			cmp=coefficient[i]-n2.coefficient[i];
			break;
		}
	}
	if(cmp>0&&symbol>0)return 1;
	else if(cmp>0&&symbol<0)return -1;
	else if(cmp==0)return 0;
	else if(cmp<0&&symbol>0)return -1;
	else if(cmp<0&&symbol<0)return 1;
}

SciNum SciNum::operator+(SciNum n2)
{
	SciNum ans;
	if(error_flag==1)
	{
		ans.error_flag=1;
		ans.error=error;
		return ans;
	}
	if(n2.error_flag==1)
	{
		ans.error_flag=1;
		ans.error=n2.error;
		return ans;
	}

	if(symbol==0)
	{
		return n2;
	}
	else if(n2.symbol==0)
	{
		return *this;
	}
	if(symbol==n2.symbol)//符号相同
	{
		ans.symbol=symbol;
		int carry=0;
		for(int i=0;i<200;i++)
		{
			int temp=coefficient[i]+n2.coefficient[i]+carry;
			if(temp>=10)
			{
				carry=1;
				temp-=10;
			}
			else carry=0;
			ans.coefficient[i]=temp;
		}
		if(carry==1)
		{
			ans.error_flag=1;
			ans.error="数据溢出";
			return ans;
		}
		ans.calculate_num();
		return ans;
	}
	else if(symbol==1)//正数加负数
	{
		SciNum temp=n2;
		temp.symbol=1;
		return *this-temp;
	}
	else//负数加正数
	{
		SciNum temp=*this;
		temp.symbol=1;
		return n2-temp;
	}
}

SciNum SciNum::operator-(SciNum n2)
{
	SciNum ans;
	if(error_flag==1)
	{
		ans.error_flag=1;
		ans.error=error;
		return ans;
	}
	if(n2.error_flag==1)
	{
		ans.error_flag=1;
		ans.error=n2.error;
		return ans;
	}

	if(symbol==0)
	{
		ans=n2;
		ans.symbol*=-1;
		return ans;
	}
	else if(n2.symbol==0)
	{
		return *this;
	}

	if(symbol==n2.symbol)
	{
		int abscmp=0;//比较绝对值大小
		if(symbol>0)
		{
			abscmp=this->compare(n2);
		}
		else abscmp=-(this->compare(n2));
		if(abscmp<0)
		{
			ans=n2-*this;
			ans.symbol=-symbol;
			ans.calculate_num();
			return ans; 
		}
		else if(abscmp==0)
		{
			return ans;
		}
		else
		{
			ans.symbol=symbol;
			int borrow=0;//代表是否借位
			for(int i=0;i<=199;i++)
			{
				int temp=coefficient[i]-borrow;
				if(temp<n2.coefficient[i])
				{
					temp+=10;
					borrow=1;
				}
				else borrow=0;
				ans.coefficient[i]=temp-n2.coefficient[i];
			}
			ans.calculate_num();
			return ans;
		}
	}
	else
	{
		SciNum temp=n2;
		temp.symbol*=-1;
		return *this+temp;
	}
}

SciNum SciNum::operator*(SciNum n2)
{
	SciNum ans;
	if(error_flag==1)
	{
		ans.error_flag=1;
		ans.error=error;
		return ans;
	}
	if(n2.error_flag==1)
	{
		ans.error_flag=1;
		ans.error=n2.error;
		return ans;
	}

	if(symbol==0||n2.symbol==0)//0乘任何数都为0
	{
		return ans;
	}

	for(int i=0;i<200;i++)
	{
		if(coefficient[i]==0)continue;
		SciNum multi_temp;//用于储存每一位乘n2的值
		int carry=0;//进位
		for(int j=0;j<200;j++)
		{
			int temp=coefficient[i]*n2.coefficient[j]+carry;
			carry=temp/10;
			temp%=10;
			if(temp!=0)
			{
				if(i+j-100>=0&&i+j-100<=199)multi_temp.coefficient[i+j-100]=temp;
				else if(i+j-100>199)
				{
					ans.error_flag=1;
					ans.error="数据溢出";
				}
			}

		}
		multi_temp.symbol=1;
		multi_temp.calculate_num();
		ans=ans+multi_temp;
	}

	if(symbol==n2.symbol)ans.symbol=1;
	else ans.symbol=-1;

	ans.calculate_num();
	return ans;
}

SciNum SciNum::operator/(SciNum n2)
{
	SciNum ans;
	if(error_flag==1)
	{
		ans.error_flag=1;
		ans.error=error;
		return ans;
	}
	if(n2.error_flag==1)
	{
		ans.error_flag=1;
		ans.error=n2.error;
		return ans;
	}

	if(symbol==0)
	{
		return ans;
	}
	else if(n2.symbol==0)
	{
		ans.error_flag=1;
		ans.error="除数不能为0";
		return ans;
	}

	SciNum dividend=*this;//被除数
	SciNum num10("10");
	SciNum n2ei=n2;
	dividend.symbol=1;
	n2ei.symbol=1;
	int i=100;
	while(dividend.compare(n2ei*num10)==1)
	{
		n2ei=n2ei*num10;
		i++;
	}
	while(i>=0)
	{
		while(dividend.compare(n2ei)>=0)
		{
			dividend=dividend-n2ei;
			ans.coefficient[i]++;
		}
		for(int j=0;j<199;j++)
		{
			n2ei.coefficient[j]=n2ei.coefficient[j+1];
		}
		n2ei.coefficient[199]=0;
		i--;
	}

	if(symbol==n2.symbol)ans.symbol=1;
	else ans.symbol=-1;

	ans.calculate_num();
	return ans;
}

SciNum SciNum::operator^(SciNum n2)
{
	SciNum ans;
	if(error_flag==1)
	{
		ans.error_flag=1;
		ans.error=error;
		return ans;
	}
	if(n2.error_flag==1)
	{
		ans.error_flag=1;
		ans.error=n2.error;
		return ans;
	}

	if(symbol==0)
	{
		return ans;
	}
	if(n2.symbol==0)
	{
		SciNum temp("1");
		return temp;
	}

	double new_mantissa=pow(mantissa,n2.num);//计算尾数的n2次方,exp为指数部分
	int exp=0;
	while(abs(new_mantissa)>10-1e-6)
	{
		new_mantissa/=10;
		exp++;
	}
	while(abs(new_mantissa)<1-1e-6)
	{
		new_mantissa*=10;
		exp--;
	}

	double exppart_mantissa=pow(10,n2.num*exponent);
	int exp2=0;
	while(exppart_mantissa>10-1e-6)
	{
		exppart_mantissa/=10;
		exp2++;
	}
	while(exppart_mantissa<1-1e-6)
	{
		exppart_mantissa*=10;
		exp2--;
	}
	ans.mantissa=new_mantissa*exppart_mantissa;
	ans.exponent=exp+exp2;
	if(ans.mantissa>10-1e-6)
	{
		ans.mantissa/=10;
		ans.exponent++;
	}

	if(ans.exponent>100)
	{
		ans.error_flag=1;
		ans.error="数据溢出";
		return ans;
	}
	ans.calculate_sci_to_num();
	return ans;
}

SciNum SciNum::Sin()
{
	SciNum ans;
	if(error_flag)return *this;
	if(num>1e10)
	{
		error_flag=1;
		error="数据溢出";
		return *this;
	}
	if(abs(num)<1e-6)return *this;
	double temp=sin(num);
	int exp=0;
	while(temp>10-1e-6)
	{
		temp/=10;
		exp++;
	}
	while(abs(temp)<1-1e-6)
	{
		temp*=10;
		exp--;
	}
	ans.num=sin(num);
	ans.exponent=exp;
	ans.mantissa=temp;
	ans.calculate_sci_to_num();
	return ans;
}
SciNum SciNum::Cos()
{
	SciNum ans;
	if(error_flag)return *this;
	if(num>1e10)
	{
		error_flag=1;
		error="数据溢出";
		return *this;
	}
	if(abs(num)<1e-6)
	{
		SciNum b("1");
		ans=b-*this;
		return ans;
	}
	double temp=cos(num);
	int exp=0;
	while(temp>10-1e-6)
	{
		temp/=10;
		exp++;
	}
	while(abs(temp)<1-1e-6)
	{
		temp*=10;
		exp--;
	}
	ans.num=cos(num);
	ans.exponent=exp;
	ans.mantissa=temp;
	ans.calculate_sci_to_num();
	return ans;
}
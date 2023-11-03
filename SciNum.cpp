#include "SciNum.h"

SciNum::SciNum():coefficient(), symbol(), error(), error_flag(), mantissa(), exponent(), num()
{}

SciNum::SciNum(const char *n):coefficient(), symbol(1), error(), error_flag(), mantissa(), exponent(), num()
{
	int decimal_point_flag=0;//代表找到的小数点的数量
	std::stack<char>int_part;

    const char* p=n;
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
	}
	
	//当前输入的数字为整数部分
	exponent=int_part.size()-1;//数字位数减一为其的指数项
	char c;
	double temp_num=1;//储存10的exponent-i次方
	for(int i=int_part.size()-1;i>=0&&error_flag==0;i--)
	{
		c=int_part.top();
		int_part.pop();
		coefficient[100+exponent-i]=c-'0';
		num+=temp_num*(c-'0');
		temp_num*=10;
	}
	
	if(*p!='.'&&*p!='\0'&&error_flag==0)
	{
		error_flag=1;
		error="语法错误（数结尾异常）";
	}
	else if(*p=='.'&&error_flag==0)//开始输入小数部分
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
	for(i=199;i>=0&&coefficient[i]==0;i--);//寻找第一个有数的位
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
    std::memset(coefficient,0,sizeof(int)*200);
    if(std::abs(mantissa)<1e-6)
	{
		symbol=0;
		num=0;
		return;
	}
	else if(mantissa>0)symbol=1;
	else if(mantissa<0)symbol=-1;

	double temp=mantissa;
    temp=std::abs(temp);
	for(int i=exponent+100;i>=0;i--)
	{
		coefficient[i]=(int)temp;
		temp*=10;
		while(temp>10-1e-14)temp-=10;
        if(std::abs(temp)<1e-14)break;
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

int SciNum::compare(const SciNum &n1, const SciNum &n2)
{
    if(n1.symbol<n2.symbol)return -1;
    if(n1.symbol>n2.symbol)return 1;
	int cmp=0;
	for(int i=199;i>=0;i--)
	{
        if(n1.coefficient[i]==n2.coefficient[i])
		{
			if(i!=0)continue;
			else cmp=0;
		}
		else 
		{
            cmp=n1.coefficient[i]-n2.coefficient[i];
			break;
		}
	}
    if(cmp>0&&n1.symbol>0)return 1;
    else if(cmp>0&&n1.symbol<0)return -1;
	else if(cmp==0)return 0;
    else if(cmp<0&&n1.symbol>0)return -1;
    else if(cmp<0&&n1.symbol<0)return 1;
    return 0;
}

SciNum SciNum::operator+(const SciNum& n2) const
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

SciNum SciNum::operator-(const SciNum& n2) const
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
            abscmp=compare(*this, n2);
		}
        else abscmp=-(compare(*this, n2));
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

SciNum SciNum::operator*(const SciNum& n2) const
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

SciNum SciNum::operator/(const SciNum& n2) const
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

	if(n2.symbol==0)
	{
		ans.error_flag=1;
		ans.error="除数不能为0";
		return ans;
	}
	else if(symbol==0)
	{
		return ans;
	}

	SciNum dividend=*this;//被除数
	SciNum num10("10");
	SciNum n2ei=n2;
	dividend.symbol=1;
	n2ei.symbol=1;
	int i=100;
    while(compare(dividend, n2ei*num10)==1)
	{
		n2ei=n2ei*num10;
		i++;
	}
	while(i>=0)
	{
        while(compare(dividend, n2ei)>=0)
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

SciNum SciNum::operator^(const SciNum& n2) const
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
    while(std::abs(new_mantissa)>10-1e-6)
	{
		new_mantissa/=10;
		exp++;
	}
    while(std::abs(new_mantissa)<1-1e-6)
	{
		new_mantissa*=10;
		exp--;
	}
	double exppart_mantissa=pow(10,n2.num*exponent);
	if(exppart_mantissa<pow(10,-200))
	{
		return ans;
	}
	else if(exppart_mantissa>pow(10,200))
	{
		ans.setError("数据溢出");
		return ans;
	}
	
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

	if(ans.exponent>=100)
	{
		ans.error_flag=1;
		ans.error="数据溢出";
		return ans;
	}
	ans.calculate_sci_to_num();
	return ans;
}

SciNum SciNum::sin(SciNum n)
{
	SciNum ans;
    if(n.error_flag)return n;
    if(n.num>1e10)
	{
        n.error_flag=1;
        n.error="数据溢出";
        return n;
	}
    if(std::abs(n.num)<1e-6)return n;
    double temp=std::sin(n.num);
	int exp=0;
	while(temp>10-1e-6)
	{
		temp/=10;
		exp++;
    }
    while(std::abs(temp)<1-1e-6)
	{
		temp*=10;
		exp--;
    }
    ans.num=std::sin(n.num);
	ans.exponent=exp;
	ans.mantissa=temp;
	ans.calculate_sci_to_num();
	return ans;
}
SciNum SciNum::cos(SciNum n)
{
	SciNum ans;
    if(n.error_flag)return n;
    if(n.num>1e10)
	{
        n.error_flag=1;
        n.error="数据溢出";
        return n;
	}
    if(std::abs(n.num)<1e-6)
	{
		SciNum b("1");
        ans=b-n;
		return ans;
    }
    double temp=std::cos(n.num);
	int exp=0;
	while(temp>10-1e-6)
	{
		temp/=10;
		exp++;
	}
    while(std::abs(temp)<1-1e-6)
	{
		temp*=10;
		exp--;
    }
    ans.num=std::cos(n.num);
	ans.exponent=exp;
	ans.mantissa=temp;
	ans.calculate_sci_to_num();
	return ans;
}
SciNum SciNum::  calculateExpression(const char *formula, const SciNum& Ans)
{
	char s[100]={};
	SciNum result;

	int count=0;
	
	if(*formula=='\0')
	{
		result.setError("括号不匹配！");
		return result;
	}
	{//此程序块用于补右括号，并把算式赋值进s
		int i;
		for(i=0;*(formula+i)!='\0';i++)
		{
			s[i]=*(formula+i);
			
			if(*(formula+i)=='(')
			{
				count++;
			}
			else if(*(formula+i)==')')
			{
				count--;
			}
			if(i==99)
			{
				result.error_flag=1;
				result.error="输入过长!";
				return result;
			}
		}
		if(count==0)
		{
			s[i]='\0';
		}
		else if(count<0)
		{
			result.setError("语法错误");
			return result;
		}
		else//count>0
		{
			while(count--)
			{
				if(i>=99)
				{
					result.error_flag=1;
					result.error="输入过长!";
					return result;
				}
				s[i++]=')';
			}
			s[i]='\0';
		}
		
	}
	
	std::stack<SciNum> NumStack;
	std::stack<char> OperatorStack;

	int flag=0;//flag代表当前的状态，0代表下一个是数，1代表下一个是符号
	char *p=s;
	while(*p!='\0')
	{
		if(flag==0)
		{
			
			if(*p=='(')//左括号，找到则计算该括号内部的值，使用递归
			{
				int count=1;//已经数到的左括号-右括号数量
				char newnumber[100]={};
				int pnew=0;
				p++;
				while(1)
				{
					if(*p=='(')count++;
					else if(*p==')')
					{
						if(count==1)
						{
							p++;
							break;
						}
						else count--;
					}
					newnumber[pnew++]=*p++;
				}
				newnumber[pnew]='\0';
				SciNum n;
				n=calculateExpression(newnumber, Ans);
				NumStack.push(n);
				flag=1;
			}
			else if(*p=='A')
			{
				p++;
				if(*p!='n')
				{
					result.setError("语法错误");
					return result;
				}
				p++;
				if(*p!='s')
				{
					result.setError("语法错误");
					return result;
				}
				p++;
				NumStack.push(Ans);
				flag=1;
			}
			else if(*p=='c')
			{
				SciNum n;//cos后接的数
				p++;
				if(*p!='o')
				{
					result.setError("语法错误");
					return result;
				}
				p++;
				if(*p!='s')
				{
					result.setError("语法错误");
					return result;
				}
				p++;
				if(*p!='(')
				{
					result.setError("语法错误");
					return result;
				}
				else
				{
				int count=1;//已经数到的左括号-右括号数量
				char newnumber[100]={};
				int pnew=0;
				p++;
				while(1)
				{
						if(*p=='(')count++;
						else if(*p==')')
						{
							if(count==1)
							{
								p++;
								break;
							}
							else count--;
						}
						newnumber[pnew++]=*p++;
					}
					newnumber[pnew]='\0';
					n=calculateExpression(newnumber, Ans);
				}
				n=SciNum::cos(n);
				NumStack.push(n);
				flag=1;
			}
			else if(*p=='s')
			{
				SciNum n;//cos后接的数
				p++;
				if(*p!='i')
				{
					result.setError("语法错误");
					return result;
				}
				p++;
				if(*p!='n')
				{
					result.setError("语法错误");
					return result;
				}
				p++;
				if(*p!='(')
				{
					result.setError("语法错误");
					return result;
				}
				else
				{
				int count=1;//已经数到的左括号-右括号数量
				char newnumber[100]={};
				int pnew=0;
				p++;
				while(1)
				{
						if(*p=='(')count++;
						else if(*p==')')
						{
							if(count==1)
							{
								p++;
								break;
							}
							else count--;
						}
						newnumber[pnew++]=*p++;
					}
					newnumber[pnew]='\0';
					n=calculateExpression(newnumber, Ans);
				}
				n=SciNum::sin(n);
				NumStack.push(n);
				flag=1;
			}
			else 
			{
				char newnumber[100]={};
				int pnew=0;
				int pointflag=0;
				newnumber[pnew++]=*p++;
				while((*p<='9'&&*p>='0')||*p=='.')
				{
					if(*p=='.')
					{
						pointflag++;
					}
					newnumber[pnew++]=*p++;
				}
				newnumber[pnew]='\0';
				SciNum n(newnumber);
				NumStack.push(n);
				flag=1;
			}
		}
		else
		{
			if(*p=='+'||*p=='-')
			{
				while(!(OperatorStack.empty()))
				{
					SciNum n2=NumStack.top();
					NumStack.pop();
					SciNum n1=NumStack.top();
					NumStack.pop();
					char opera=OperatorStack.top();
					OperatorStack.pop();
					if(opera=='+')
					{
						n1=n1+n2;
						NumStack.push(n1);
					}
					else if(opera=='-')
					{
						n1=n1-n2;
						NumStack.push(n1);
					}
					else if(opera=='*')
					{
						n1=n1*n2;
						NumStack.push(n1);
					}
					else if(opera=='/')
					{
						n1=n1/n2;
						NumStack.push(n1);
					}
					else if(opera=='^')
					{
						n1=n1^n2;
						NumStack.push(n1);
					}
					else
					{
						printf("特殊符号\n");
						exit(0);
					}
				}
				OperatorStack.push(*p);
				p++;
			}
			else if(*p=='*'||*p=='/')
			{
				while(!OperatorStack.empty()&&(OperatorStack.top()=='*'||OperatorStack.top()=='/'||OperatorStack.top()=='^'))//计算前面所有的乘、除、乘方
				{
					SciNum n2=NumStack.top();
					NumStack.pop();
					SciNum n1=NumStack.top();
					NumStack.pop();
					char opera=OperatorStack.top();
					OperatorStack.pop();
					if(opera=='*')
					{
						n1=n1*n2;
						NumStack.push(n1);
					}
					else if(opera=='/')
					{
						n1=n1/n2;
						NumStack.push(n1);
					}
					else if(opera=='^')
					{
						n1=n1^n2;
						NumStack.push(n1);
					}
				}
				OperatorStack.push(*p);
				p++;
			}
			else if(*p=='^')
			{
				if(!OperatorStack.empty()&&OperatorStack.top()=='^')//计算前面所有乘方(按理来讲顶多只有一个)
				{
					SciNum n2=NumStack.top();
					NumStack.pop();
					SciNum n1=NumStack.top();
					NumStack.pop();
					char opera=OperatorStack.top();
					OperatorStack.pop();
					
					n1=n1^n2;
					NumStack.push(n1);
				}
				OperatorStack.push(*p);
				p++;
			}
			else
			{
				result.error_flag=1;
				result.error="输入错误";
				return result;
			}
			flag=0;
		}
	} 

	while(!(OperatorStack.empty()))//计算剩下的
	{
		if(NumStack.size()==1)
		{
			SciNum temp;
			temp.error_flag=1;
			temp.error="输入错误";
			NumStack.push(temp);
			break;
		}
		SciNum n2=NumStack.top();
		NumStack.pop();
		SciNum n1=NumStack.top();
		NumStack.pop();
		char opera=OperatorStack.top();
		OperatorStack.pop();
		if(opera=='+')
		{
			n1=n1+n2;
			NumStack.push(n1);
		}
		else if(opera=='-')
		{
			n1=n1-n2;
			NumStack.push(n1);
		}
		else if(opera=='*')
		{
			n1=n1*n2;
			NumStack.push(n1);
		}
		else if(opera=='/')
		{
			n1=n1/n2;
			NumStack.push(n1);
		}
		else if(opera=='^')
		{
			n1=n1^n2;
			NumStack.push(n1);
		}
		else
		{
			printf("特殊符号\n");
			exit(0);
		}
	}
	result=NumStack.top();
	NumStack.pop();
	return result;
}

double SciNum::getNum()
{
	return num;
}

int SciNum::getExp()
{
	return exponent;
}

double SciNum::getMantisaa()
{
	return mantissa;
}

int SciNum::isError()
{
	return error_flag;
}

std::string SciNum::errorMessage()
{
	return error;
}

void SciNum::setError(std::string errorMessage)
{
	error_flag=1;
	error=errorMessage;
	return;
}
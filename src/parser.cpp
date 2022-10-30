#include <parser.h>

Parser::Parser(std::map<std::string, Operation> &operations)
	: operations(operations) { }

Token::Token(Token &&token)
	: type(std::move(token.type)), 
	  name(std::move(token.name)) { }

Token::Token(Token const& token)
	: type(token.type), 
	  name(token.name) { }

Token::Token(Token::Type &&type, std::string &&name)
	: type(std::move(type)), name(std::move(name)) {  }

Token::Token(Token::Type const &type, std::string const& name)
	: type(type), name(name) {  }

bool Token::operator==(Token const &token)
{
	return token.name == this->name;
}

bool Token::operator==(std::string const& str)
{
	return str == this->name;
}

std::vector<Token> Parser::infixToPostfix(std::string& infix)
{
	infix = '(' + infix + ')';
	int l = infix.size();
	std::stack<Token> st;
	std::vector<Token> output;

	for (int i = 0; i < l; i++) 
    {
		while (isspace(infix[i]))
			i++;

		std::string sym = std::string(1, infix[i]);

		if (isdigit(infix[i]))
			output.emplace_back(std::move(GetOperand(infix, i)));

		else if (infix[i] == '(')
		{
			st.push(Token(Token::Bracket, "("));
		}

		else if (infix[i] == ')')
		{
			while (st.top() != "(")
			{
				output.push_back(st.top());
				st.pop();
			}
			st.pop();
		}

		else if (operations.contains(sym))
		{
			Token token(Token::Operation, sym);
			while (!st.empty())
			{
				if (GetPriority(token) <= GetPriority(st.top()))
				{
					output.push_back(st.top());
					st.pop();
				}
				else break;
			}

			st.push(Token(Token::Operation, sym));
		}

		else if (isalpha(infix[i]))
		{
			auto token = GetFunction(infix, i);
			if (token.has_value())
			{
				st.push(token.value());
			}
		}
	}

	while(!st.empty())
	{
		output.push_back(st.top());
		st.pop();
	}
	return output;
}


Token Parser::GetOperand(std::string const& str, int &index)
{
	std::string operand;
	while (isspace(str[index]))
		index++;

	while (isdigit(str[index]))
		operand += str[index++];

	index--;
	return Token(Token::Type::Operand, operand);
}

std::optional<Token> Parser::GetFunction(std::string const& str, int& index)
{
	std::shared_ptr<PluginManager> pluginManager;
	pluginManager = pluginManager->Instance();

	std::string func_name;
	while (isalpha(str[index]))
	{
		func_name += str[index++];
	}
	index--;

	if (pluginManager->ContainsBinary(func_name))
	{
		return Token(Token::BinaryFunction, func_name);
	}
	else if (pluginManager->ContainsUnary(func_name))
	{
		return Token(Token::UnaryFunction, func_name);
	}

	try
	{
		pluginManager->LoadUnaryFunc(func_name);
		return Token(Token::UnaryFunction, func_name);
	}
	catch(const std::exception& e)
	{
		try
		{
			pluginManager->LoadBinaryFunction(func_name);
			return Token(Token::Type::BinaryFunction, func_name);
		}
		catch(const std::exception& e)
		{
			std::cerr << '\n' << e.what() << '\n';
			return {  };
		}
	}
}

int Parser::GetPriority(Token const &token)
{
	if (token.type == Token::Operation)
	{
		auto search = operations.find(token.name);
		if (search != operations.end())
		{
			return search->second.priority;
		}
		return -1;
	}

	else if (token.type == Token::BinaryFunction || token.type == Token::UnaryFunction)
		return 4;
	else
		return -1;
}
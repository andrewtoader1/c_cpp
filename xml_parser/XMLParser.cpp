// Project 3 -- XML Parsing Project

/** XML parsing class implementation.
    @file XMLParser.cpp */

#include <string>
#include <assert.h>
#include "XMLParser.hpp"

// Constructor implementation
XMLParser::XMLParser()
{
	// allocate memory for an empty stack and empty bag to store 
	elementNameBag = new Bag<std::string>;
	parseStack = new Stack<std::string>;

	// initialize parse and tokenize bools to false
	input_tokenized = false;
	input_parsed = false;
}  // end default constructor

// Destructor
XMLParser::~XMLParser()
{
	clear();					// call member variable to clear the bag and stack

	delete elementNameBag;		// release the pointers to bag and stack from memory
	delete parseStack;
}  // end destructor

// tokenizeInputString method
// tokenizes the input string
// @pre: a string of xml which the user would like to validate
// @pre: (NOTE:: IF THERE WAS CONTENT STORED IN CLASS MEMBER VARIABLES BEFORE WHICH USER FAILED TO CLEAR, THIS METHOD WILL CLEAR THE CLASS BEFORE TOKENIZING)
// @post: the tokenized input stored (with valid tags) in the vector and bag class variables
// @return: true if the tokenizing was sucessful, false if some tags were invalid
bool XMLParser::tokenizeInputString(const std::string &inputString)
{
	clear();					// clear the class member variables to ensure that we will parse the right strings

	if(inputString == "")		// return false if the input string is empty
	{
		clear();
		return false;
	}

	bool input_isspace = true;
	for(int i = 0; i < inputString.length(); i++)	// check that the input is not whitespace, and if it is return false
	{
		if(!isspace(inputString[i]))
		{
			input_isspace = false;
			break;
		}

	}
	if(input_isspace)
	{
		clear();
		return false;
	}

	string markup_check = "";	// string to hold items inside markup
	string content = "";		// string to hold content
	string name = "";			// string to hold the name of the tag
	bool is_tag = false;		// flag to signal to write to markup check if "<" has been detected

	for(int i = 0; i < inputString.length(); i++)
	{
		if(inputString[i] == '<')				// if there is a "<" then set is_tag to true and go to next letter
		{
			if(i == inputString.length() - 1)	// if there is a '<' as the last charachter of the input string, return false
			{
				clear();
				return false;
			}

			// add any content to the tokenized input vector as long as its not empty or a space
			if((content.length() != 0) && (!isspace(content[0])))
			{
				// create a temporary TokenStruct to add to vector
				TokenStruct temp;
				temp.tokenString = content;
				temp.tokenType = CONTENT;

				tokenizedInputVector.push_back(temp);
			}
			content = ""; 					// clear the content string for next time it processes content

			is_tag = true;
			markup_check.push_back('<');
			continue;
		}
		else if(inputString[i] == '>')			// if there is a ">" then check the markup_check string to validate tag
		{
			markup_check.push_back('>');

			if(markup_check[1] == '/' && markup_check[markup_check.length() - 2] == '/')	// if it is an inalid tag : </tag/> then clear and return false
			{
				clear();
				return false;
			}

			bool is_declaration = false;
			if(markup_check[1] == '?' && markup_check[markup_check.length() - 2] == '?')	// if it is a declaration, raise the is declaration flag
			{
				is_declaration = true;
			}
			else if(markup_check[1] != '?' && markup_check[markup_check.length() - 2] == '?')	// if there are not 2 ?s in declaration, return false and clear
			{
				clear();
				return false;
			}
			else if(markup_check[0] == '<' && markup_check[1] == '?' && markup_check[markup_check.length() - 1] == '>' && markup_check[markup_check.length() - 2] != '?')	// if there are not 2 ?s in the declaration, return false and clear
			{
				clear();
				return false;
			}

			for(int j = 1; j < markup_check.length(); j++)				// copy the name part of the tag to variable named "name"
			{
				if(markup_check[j] !=  ' ' && markup_check[j] != '>')	// end of name is signaled by either space or ">"
				{
					if((j == markup_check.length() - 2) && ((markup_check[j] == '/') || (markup_check[j] == '?')))	// if it is an empty tag, dont add the "/" charachter by breaking out of the loop
					{
						break;
					}
					if((j == 1) && ((markup_check[j] == '/') || (markup_check[j] == '?')))	// if it is an end tag, exclude the "/" charachter from the begining
					{
						continue;
					}
					name.push_back(markup_check[j]);
				}
				else
				{
					break;
				}
			}

			if(name.length() == 0)		// if there is nothing in the name, clear the class variables and return false
			{
				clear();
				return false;
			}
			// if the name starts with a not allowed charachter clear and return false
			if(name[0] == '-' || name[0] == '.' || name[0] == '0' || name[0] == '1' || name[0] == '2' || name[0] == '3' || name[0] == '4' || name[0] == '5' || name[0] == '6' || name[0] == '7' || name[0] == '8' || name[0] == '9')
			{
				clear();
				return false;
			}
			for(int j = 0; j < name.length(); j++)		// if name contains any forbidden charachters, return false and clear
			{
				if(name[j] == '!' || name[j] == '\"' || name[j] == '#' || name[j] == '$' || name[j] == '%' || name[j] == '&' || name[j] == '\'' || name[j] == '(' || name[j] == ')' || name[j] == '*' || name[j] == '+' || name[j] == ',' || name[j] == '/' || name[j] == ';' || name[j] == '<' || name[j] == '=' || name[j] == '>' || name[j] == '?' || name[j] == '@' || name[j] == '[' || name[j] == '\\' || name[j] == ']' || name[j] == '^' || name[j] == '`' || name[j] == '{' || name[j] == '|' || name[j] == '}' || name[j] == '~')
				{
					clear();
					return false;
				}
			}

			// if they passed the tests and it is a description tag, add the whole tag to the bag
			if(is_declaration)
			{
				name = markup_check.substr(2, markup_check.length() - 4);	// create a new string for the name including the attributes

				// create a temporary TokenStruct to add to vector
				TokenStruct temp;
				temp.tokenString = name;
				temp.tokenType = DECLARATION;

				tokenizedInputVector.push_back(temp);

				elementNameBag->add(name);			// add name to the elementNameBag

				is_tag = false;						// set is_tag to false to stop adding to markup_check
				markup_check = "";					// clear the variable markup_check
				name = "";							// clear the variable name
				continue;							// skip to the next letter
			}

			// if they passed these tests, add the name to the bag
			elementNameBag->add(name);

			// create a token struct to add to array
			TokenStruct temp;
			temp.tokenString = name;

			if(markup_check[1] == '/')
			{
				temp.tokenType = END_TAG;
			}
			else if(markup_check[markup_check.length() - 2] == '/')
			{
				temp.tokenType = EMPTY_TAG;
			}
			else
			{
				temp.tokenType = START_TAG;
			}

			tokenizedInputVector.push_back(temp);

			is_tag = false;						// set is_tag to false to stop adding to markup_check
			markup_check = "";					// clear the variable markup_check
			name = "";							// clear the variable name
			continue;							// skip to the next letter
		}

		if(is_tag && (i == inputString.length() - 1) && inputString[i] != '>')		// if there is an open tag with no close and it is the end of the string, return false
		{
			clear();
			return false;
		}

		if(is_tag && inputString[i] == '<')		// if there is a nested < within a < then clear the class and return false
		{
			clear();
			return false;
		}
		if(!is_tag && inputString[i] == '>')	// if there is an ending bracket and there was no begin bracket return false and clear
		{
			clear();
			return false;
		}

		if(is_tag)								// if the is_tag flag is true then append to the string markup_check
		{
			markup_check.push_back(inputString[i]);
		}

		if(!is_tag)								// if the is_tag flag is false, then string contains content
		{
			content.push_back(inputString[i]);
		}
	}
	input_tokenized = true;
	return true;		// if made it to here, all tags are valid, so return true
}  // end

// parseTokenizedInput method here
bool XMLParser::parseTokenizedInput()
{
	if(elementNameBag->isEmpty())		// if the element name bag contains nothing, return false
	{
		clear();
		return false;
	}

	string start_str;			// string to hold the start tag popped off of the stack
	TokenStruct parse_line;		// tokenStruct to be able to get information from the tokenized input vector
	bool started_file = false;

	// we wil check validity by pushing the start tags on the stack and when an output tag comes, pop the stack and in valid xml, they should be equal
	for(int i = 0; i < tokenizedInputVector.size(); i++)		// loop through tokenized input vector to determine validity
	{
		parse_line = tokenizedInputVector[i];

		if(started_file && parseStack->isEmpty())
		{
			clear();
			return false;
		}
		else if(parse_line.tokenType == START_TAG)
		{
			if(!started_file)
			{
				started_file = true;
			}

			parseStack->push(parse_line.tokenString);
		}
		else if(parse_line.tokenType == END_TAG)
		{
			if(parseStack->isEmpty())			// if the stack is empty(no start tags) and this is an end tag, that is invalid xml
			{
				clear();
				return false;
			}
			else
			{
				start_str = parseStack->peek();
				parseStack->pop();

				if(start_str != parse_line.tokenString)	// if the last tag off of the stack does not match this end tag, it is not valid xml
				{
					clear();
					return false;
				}
			}
		}
		else if(parse_line.tokenType == DECLARATION)		// if there is a declaration within open tags, that is invalid xml
		{
			if(started_file)
			{
				clear();
				return false;
			}
		}
		else if(parse_line.tokenType == CONTENT)
		{
			if(parseStack->isEmpty())						// content can only be valid if contained within a start/end tag combination
			{												// (NOTE:) validity of that start/end tag combination will be tested when the end tag is compared with the string popped off of the stack
				clear();
				return false;
			}
		}
		else if (parse_line.tokenType == EMPTY_TAG)			// Empty tags can only be valid if contained within a start/end tag combination
		{
			if(parseStack->isEmpty())
			{
				clear();
				return false;
			}
		}
		else		// WILL NEVER REACH THIS but if somehow some other type of data, return false
		{
			clear();
			return false;
		}
	}
	if(!parseStack->isEmpty())	// if the stack of start tags is not empty, return false becuase there are unmatched start and end tags
	{
		clear();
		return false;
	}
	input_parsed = true;	// change class flag to signal that input was parsed
	return true;			// if has not returned false until now, it is valid xml
}

// clear method
void XMLParser::clear()
{
	elementNameBag->clear();		// call the clear methods on both the stack and the bag
	parseStack->clear();
	tokenizedInputVector.clear();
	input_tokenized = false;		// set the tokenized and parsed flags to false again
	input_parsed = false;
}

vector<TokenStruct> XMLParser::returnTokenizedInput() const
{
	return tokenizedInputVector;
}

// containsElementName method
// @pre: tokenized and parsed input string sucessfully placed in class variables
// @post: uses class bag to return if there is an item or not
// @return: true if there is an element in the bag, false if there is no element, throws error if not parsed or tokenized
bool XMLParser::containsElementName(const std::string &inputString) const
{
	if(input_tokenized && input_parsed)					// return contains function of class bag if input has been parsed and tokenized
	{
		return elementNameBag->contains(inputString);
	}
	else												// throw an error if input was not parsed or tokenized sucessfully
	{
		throw logic_error("Either Input was not tokenized or parsed, or failed either or both");
	}
}

// frequencyElementName method
// @pre: tokenized and parsed input string sucessfully placed in class variables
// @post: uses class bag to return frequency of the item
// @return: number of items by that name in the bag, throws error if not parsed or tokenized
int XMLParser::frequencyElementName(const std::string &inputString) const
{
	if(input_tokenized && input_parsed)							// return frequencyOf function of class bag if input has been parsed and tokenized
	{
		TokenStruct ip;
		for(int i = 0; i < tokenizedInputVector.size(); i++)	// find the number of start/end tag combinations to not count them twice
		{
			ip = tokenizedInputVector[i];
			if(ip.tokenType == START_TAG && ip.tokenString == inputString)
			{
				parseStack->push(ip.tokenString);
			}
		}
		int freq = elementNameBag->getFrequencyOf(inputString);		// get the frequency of the input string
		freq = freq - parseStack->size();						// subtract the number of start tags with that input string fro the frequency of that string
		parseStack->clear();
		return freq;
	}
	else												// throw an error if input was not parsed or tokenized sucessfully
	{
		throw logic_error("Either Input was not tokenized or parsed, or failed either or both");
	}
}


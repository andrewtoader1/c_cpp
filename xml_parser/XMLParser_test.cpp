#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include <iostream>
#include "catch.hpp"
#include "XMLParser.hpp"

using namespace std;

// Stack class Tests

TEST_CASE( "Test Bag add", "[ADT Bag]" )
{
	   INFO("Hint: testing Bag add()");
		// Create a Bag to hold ints
		Bag<int> intBag;
		int testSize = 2;
		int bagSize;
		bool success;
		for (int i=0; i<testSize; i++) {
			success = intBag.add(i);
			REQUIRE(success);
			bagSize = intBag.size();
			success = (bagSize == (i+1));
			REQUIRE(success);
		}
}

TEST_CASE( "Test Stack push and size", "[ADT Stack]" )
{
	   INFO("Hint: testing Stack push()");
		// Create a Stack to hold ints
		Stack<int> intStack;
		int testSize = 3;
		int stackSize;
		bool success;

		for (int i=0; i<testSize; i++)
		{
			success = intStack.push(i);
			REQUIRE(success);
			stackSize = intStack.size();
			success = (stackSize == (i+1));
			REQUIRE(success);
		}
}

TEST_CASE( "Test Stack Peek and isEmpty", "[ADT Stack]" )
{
	Stack<std::string> strStk;

	REQUIRE(strStk.isEmpty());

	// test the empty peek
	std::string value;
	bool empty_peek = false;
	try								// try to peek stack and catch any exceptions thrown
	{
		value = strStk.peek();
	}
	catch(logic_error e)
	{
		empty_peek = true;			// set a flag to true to signify that an exception was caught
	}
	REQUIRE(empty_peek);			// if there is a logic error thrown, it passes the test

	// test a peek with values
	std::string pushItem = "andrew";
	strStk.push(pushItem);
	REQUIRE(strStk.peek() == pushItem);

	pushItem = "stacktest";
	strStk.push(pushItem);
	REQUIRE(strStk.peek() == pushItem);

	REQUIRE(!strStk.isEmpty());
}

TEST_CASE( "Test Stack Pop", "[ADT Stack]" )
{
	Stack<int> stk;

	// test popping empty stack
	REQUIRE(!stk.pop());

	// test popping real stack
	stk.push(1);
	stk.push(2);
	stk.push(45);
	stk.push(33);

	REQUIRE(stk.pop());
	REQUIRE(stk.peek() == 45);
	REQUIRE(stk.pop());
	REQUIRE(stk.peek() == 2);
	REQUIRE(stk.pop());
	REQUIRE(stk.peek() == 1);
	REQUIRE(stk.pop());
	REQUIRE(stk.size() == 0);
}

TEST_CASE( "Test Stack Clear", "[ADT Stack]" )
{
	Stack<int> stk;

	stk.push(1);
	stk.push(2);
	stk.push(45);
	stk.push(33);

	stk.clear();

	REQUIRE(stk.isEmpty());
	REQUIRE(!stk.pop());
}

// Parser tests
TEST_CASE( "Test XMLParser tokenizeInputString and clear", "[XMLParser]" )
{
	   INFO("Hint: tokenize single element test of XMLParse");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "<test>stuff</test>";
		bool success;
		success = myXMLParser.tokenizeInputString(testString);
		REQUIRE(success);

		myXMLParser.clear();		// test the clear functionality

		testString = "<srcTag src=\"something here\"/>";
		REQUIRE(myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();

		testString = "<weirdTag as ad>";
		REQUIRE(myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();

		testString = "</brokenTag<>";
		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();

		testString = "<.Invalid/>";
		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = "<test>stuff</test/>";

		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = "<?html unicode=\"uint8\"?>";
		REQUIRE(myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = "\n</tag unicode=\"uint8\"/>";
		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = "< /tag>";
		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = " <tag /> ";
		REQUIRE(myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = "";
		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = " ";
		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = " \n";
		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = " \n <";
		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = " \n <>";
		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = " \n >";
		REQUIRE(!myXMLParser.tokenizeInputString(testString));

		myXMLParser.clear();
		testString = " some t?ex/.t ";
		REQUIRE(myXMLParser.tokenizeInputString(testString));
}

TEST_CASE( "Test Stack handout-0", "[XMLParser]" )
{
		// Create a Stack to hold ints
		Stack<int> intStack;
		int testSize = 10;
		int stackSize;
		bool success = false;
		for (int i=0; i<testSize; i++) {
			intStack.push(i);
			stackSize = intStack.size();
			success = (stackSize == (i+1));
			REQUIRE(success);
		}
		for (int i = testSize-1; i>=0; i--) {
			REQUIRE(intStack.isEmpty() == false);
            int elem = intStack.peek();
			REQUIRE(elem == i);
            REQUIRE(intStack.isEmpty() == false);
			intStack.pop();
		}
        REQUIRE(intStack.isEmpty() == true);
}

TEST_CASE( "Test Stack handout-1", "[XMLParser]" )
{
	   Stack<char> charStack;
       std::string s("Hello world");
       int i = 1;
       for (auto c:s) {
           charStack.push(c);
           REQUIRE(charStack.size() == i);
           i++;
       }
       for (i = 0; i < 5; i++) {
           REQUIRE(charStack.isEmpty() == false);
           char c = charStack.peek();
           REQUIRE(c == s[s.length()-1-i]);
           REQUIRE(charStack.isEmpty() == false);
           charStack.pop();
       }
       REQUIRE(charStack.size() == 6);
       charStack.clear();
       REQUIRE(charStack.isEmpty() == true);      
}

// Assume that the beginning and the end of CONTENT will not be filled with whitespace
TEST_CASE( "Test XMLParser tokenizeInputString Handout-0", "[XMLParser]" )
{
	   INFO("Hint: tokenize single element test of XMLParse");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "<test>stuff</test>";
		bool success;
		success = myXMLParser.tokenizeInputString(testString);
		REQUIRE(success);
		std::vector<TokenStruct> result = {TokenStruct{StringTokenType::START_TAG, std::string("test")},
											TokenStruct{StringTokenType::CONTENT, std::string("stuff")},
											TokenStruct{StringTokenType::END_TAG, std::string("test")}};
		std::vector<TokenStruct> output = myXMLParser.returnTokenizedInput();
		REQUIRE(result.size() == output.size());
		for (int i = 0; i < result.size(); i++) {
			REQUIRE(result[i].tokenType == output[i].tokenType);
			REQUIRE(result[i].tokenString.compare(output[i].tokenString) == 0);
		}
}


// Assume that the beginning and the end of CONTENT will not be filled with whitespace
TEST_CASE( "Test XMLParser tokenizeInputString Handout-1", "[XMLParser]" )
{
	   INFO("Hint: tokenize multiple elements test of XMLParse");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Note src='gmail'>  <From>Tom</From> <To>Alice</To> </Note>";
		bool success;
		success = myXMLParser.tokenizeInputString(testString);
		REQUIRE(success);
		std::vector<TokenStruct> result = { TokenStruct{StringTokenType::DECLARATION, std::string("xml version=\"1.0\" encoding=\"UTF-8\"")},
									TokenStruct{StringTokenType::START_TAG, std::string("Note")},
									TokenStruct{StringTokenType::START_TAG, std::string("From")},
									TokenStruct{StringTokenType::CONTENT, std::string("Tom")},
									TokenStruct{StringTokenType::END_TAG, std::string("From")},
									TokenStruct{StringTokenType::START_TAG, std::string("To")},
									TokenStruct{StringTokenType::CONTENT, std::string("Alice")},
									TokenStruct{StringTokenType::END_TAG, std::string("To")},
									TokenStruct{StringTokenType::END_TAG, std::string("Note")}};
		std::vector<TokenStruct> output = myXMLParser.returnTokenizedInput();

		REQUIRE(result.size() == output.size());
		for (int i = 0; i < result.size(); i++) {
			REQUIRE(result[i].tokenType == output[i].tokenType);
			REQUIRE(result[i].tokenString.compare(output[i].tokenString) == 0);
		}
}

TEST_CASE( "Test XMLParser parseTokenizedInput", "[XMLParser]" )
{
	XMLParser myXMLParser;

	REQUIRE(!myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString(" <someTag>Content</someTag> "));
	REQUIRE(myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("<?xml version =\"1.0\"?><html><head>Content here</head><body>Content here<empty src=\"f\"/></body></html>"));
	REQUIRE(myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("</tag>reversed<tag>"));
	REQUIRE(!myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("<tag>"));
	REQUIRE(!myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("</tag>"));
	REQUIRE(!myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("<empty/>\n<tag>content</tag>"));
	REQUIRE(!myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("<tag>content<empty/>\n\n\n</tag>"));
	REQUIRE(myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("<tag1>content<tag2cross></tag1></tag2cross>"));
	REQUIRE(!myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("<tag1>content<tag2cross></tag1></tag2cross>"));
	REQUIRE(!myXMLParser.parseTokenizedInput());

	REQUIRE(!myXMLParser.tokenizeInputString("<tag1 <anothertag>>content</tag1>"));
	REQUIRE(!myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("<?xml?><head>sometext</head>\n<body>sometext</body>"));
	REQUIRE(!myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("<?xml?><enclosed><head>sometext</head>\n<body>sometext</body></enclosed>"));
	REQUIRE(myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("<?xml?><eclosed><?xml-spreadsheet?><head>sometext</head>\n<body>sometext</body></enclosed>"));
	REQUIRE(!myXMLParser.parseTokenizedInput());

	REQUIRE(myXMLParser.tokenizeInputString("<food><name>Belgian Waffles</name><price>$5.95</price><description>Two of our famous Belgian Waffles with plenty of real maple syrup</description><calories>650</calories></food>"));
	REQUIRE(myXMLParser.parseTokenizedInput());
}

TEST_CASE( "Test XMLParser parseTokenizedInput Handout-0", "[XMLParser]" )
{
	   INFO("Hint: tokenize single element test of XMLParse");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "<test myattr='abcdef'>stuff<this_is_empty_tag/></test>";
		bool success;
		success = myXMLParser.tokenizeInputString(testString);
		REQUIRE(success);
		std::vector<TokenStruct> result = {TokenStruct{StringTokenType::START_TAG, std::string("test")},
											TokenStruct{StringTokenType::CONTENT, std::string("stuff")},
											TokenStruct{StringTokenType::EMPTY_TAG, std::string("this_is_empty_tag")},
											TokenStruct{StringTokenType::END_TAG, std::string("test")}};
		std::vector<TokenStruct> output = myXMLParser.returnTokenizedInput();
		REQUIRE(result.size() == output.size());
		success = myXMLParser.parseTokenizedInput();
		REQUIRE(success);
		output = myXMLParser.returnTokenizedInput();
		REQUIRE(result.size() == output.size());
		for (int i = 0; i < result.size(); i++) {
			REQUIRE(result[i].tokenType == output[i].tokenType);
			REQUIRE(result[i].tokenString.compare(output[i].tokenString) == 0);
		}
}

TEST_CASE( "Test XMLParser Final Handout-0", "[XMLParser]" )
{
	   INFO("Hint: Product");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		ifstream myfile ("./product.txt");
		std::string inputString((std::istreambuf_iterator<char>(myfile) ), (std::istreambuf_iterator<char>()) );

		bool success;
		success = myXMLParser.tokenizeInputString(inputString);
		REQUIRE(success);
		success = myXMLParser.parseTokenizedInput();
		REQUIRE(success);
		REQUIRE(myXMLParser.containsElementName("catalog"));
		REQUIRE(myXMLParser.frequencyElementName("catalog") == 1);
		REQUIRE(myXMLParser.containsElementName("product"));
		REQUIRE(myXMLParser.frequencyElementName("product") == 1);
		REQUIRE(myXMLParser.containsElementName("catalog_item"));
		REQUIRE(myXMLParser.frequencyElementName("catalog_item") == 2);
		REQUIRE(myXMLParser.containsElementName("item_number"));
		REQUIRE(myXMLParser.frequencyElementName("item_number") == 2);
		REQUIRE(myXMLParser.containsElementName("size"));
		REQUIRE(myXMLParser.frequencyElementName("size") == 6);
		REQUIRE(myXMLParser.containsElementName("color_swatch"));
		REQUIRE(myXMLParser.frequencyElementName("color_swatch") == 15);
}


TEST_CASE( "Test XML containsElementName and frequencyElementName", "[XMLParser]" )
{
	XMLParser myXMLParser;

	string testString = "<?xml-script version=\"1.0\"?><test>text</test>";
	myXMLParser.tokenizeInputString(testString);

	bool caught = false;

	try
	{
		myXMLParser.containsElementName("test");
	}
	catch (logic_error e)
	{
		caught = true;
	}
	REQUIRE(caught);		// if the input has not been parsed, there should be a logic error returned

	caught = false;

	try
	{
		myXMLParser.frequencyElementName("test");
	}
	catch (logic_error e)
	{
		caught = true;
	}

	REQUIRE(caught);	// if the input has not been parsed, there should be a logic erorr returned



	myXMLParser.clear();

	myXMLParser.tokenizeInputString(testString);
	myXMLParser.parseTokenizedInput();

	REQUIRE(myXMLParser.containsElementName("test"));
	REQUIRE(myXMLParser.frequencyElementName("test") == 1);
	
	REQUIRE(myXMLParser.containsElementName("xml-script version=\"1.0\""));
}

TEST_CASE( "Test XML clear() and returnTokenizedInput", "[XMLParser]" )
{
	XMLParser myXMLParser;
	vector<TokenStruct> a;
	TokenStruct test_element;

	a = myXMLParser.returnTokenizedInput();
	REQUIRE(a.size() ==0);

	REQUIRE(myXMLParser.tokenizeInputString("<?xml-script version=\"1.0\"?>\n<enclosed><head>Content</head><body>More Content<div>Div Content</div><empty src=\"dontworryaboutit\"/></body></enclosed>"));
	REQUIRE(myXMLParser.parseTokenizedInput());

	a.clear();
	a = myXMLParser.returnTokenizedInput();

	REQUIRE(a.size() == 13);

	test_element = a.at(0);
	REQUIRE(test_element.tokenType == DECLARATION);
	REQUIRE(test_element.tokenString == "xml-script version=\"1.0\"");

	test_element = a.at(4);
	REQUIRE(test_element.tokenType == END_TAG);
	REQUIRE(test_element.tokenString == "head");

	test_element = a.at(5);
	REQUIRE(test_element.tokenType == START_TAG);
	REQUIRE(test_element.tokenString == "body");

	myXMLParser.clear();

	a = myXMLParser.returnTokenizedInput();
	REQUIRE(a.size() == 0);
}






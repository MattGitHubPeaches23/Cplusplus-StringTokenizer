// Copyright (c) 2019, Matthew Chalifoux.
#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;
const int MAX_COLUMNS = 256;
const int MAX_ROWS = 6;
const int MAX_BUFFER = 1000;

class Token
{
public:
    Token();
    Token(string str, int type);
    Token & operator =(const Token & h);
    friend ostream& operator <<(ostream& outs, const Token& t);
    int type();
    string type_string();
    string token_str();
private:
    string _token;
    int _type;
};

class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    bool done();
    bool more();            //true: there are more tokens
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);
    //set a new string as the input string
    void set_string(char str[]);
    //extract one token (very similar to the way cin >> works)
    //friend STokenizer& operator >> (STokenizer& s, Token& t);

    // THE STATE MACHINE
    // Fill all cells of the array with -1
    void init_table(int _table[][MAX_COLUMNS]);
    // Mark a range of cells in the array.
    void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to,
                    int state);
    // Mark this row and column
    void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state);
    // Mark this state (row) with a 1 (success)
    void mark_success(int _table[][MAX_COLUMNS], int state);
    // Mark this state (row) with a 0 (fail)
    void mark_fail(int _table[][MAX_COLUMNS], int state);
    // Function to get token
    // True if state is a success state
    bool is_success(int _table[][MAX_COLUMNS], int state);
    void get_token(string str, int &position, int _table[][MAX_COLUMNS],
                   int state, string& token, int &type);

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);
    //extract the longest string that match
    //     one of the acceptable token types
    //bool get_token(int start_state, string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};
int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];



int main(int argc, char *argv[])
{
    char s[] = "it was the night of october 17th. pi was still 3.14.";
    STokenizer stk(s);
    Token t;
    //The all too familiar golden while loop:
    stk>>t;
    //cout << "typstring = " << t.type_string() << endl;
    while(stk.more())
    {
        //process token here...
        cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;
        t = Token();
        stk>>t;
    }



    // Test for _buffer and pos
//    cout << "_buffer[] = " << test._buffer << endl
//         << "_pos = " << test._pos << endl;
//    cout << "token = " << token << endl;
//    cout << "position = " << position << endl;
//    cout << "type = " << type << endl;



    return 0;
}



string Token::type_string()
{
    string h;
    if(1 == _type)
    {
        h = "UNKNOWN";
    }
    else if(2 == _type)
    {
        h = "SPACE";
    }
    else if(3 == _type)
    {
        h = "NUMBER";
    }
    else if(4 == _type)
    {
        h = "ALPHA";
    }
    return h;
}

string Token::token_str()
{
    return _token;
}
Token::Token(string str, int type)
{
    _token = str;
    _type = type;
}
Token &Token::operator =(const Token & h)
{
    _token = h._token;
    _type = h._type;
}

Token::Token()
{
    _token = "";
    _type = 0;
}
ostream& operator <<(ostream& outs, const Token& t)
{
    string sub;
    sub = t._token;
    outs << "|" << sub << "|" << endl;
    return outs;
}

STokenizer& operator >> (STokenizer& s, Token& t)
{
    //test.get_token(test._buffer,position,test._table,0,token,type);
    string token;
    int type;
    STokenizer tester;
    //cout << s._buffer << " " << s._pos << endl;
    //s._pos = 51;
    s.get_token(s._buffer, s._pos, s._table, 0, token, type);
    //cout << "pos = " << s._pos << endl;
    //cout << "TYPE = " << type << endl;
    Token sub(token,type);
    t = sub;
    //cout << "string token = " << t._token << "  type = " << t._type << endl;
//    cout << "token = " << t.token << endl;
//    cout << "position = " << s._pos << endl;
    return s;
}
bool STokenizer::more()
{
    if(_pos<=strlen(_buffer))
    {
        return true;
    }
    else if(_pos > strlen(_buffer))
    {
        return false;
    }
}

STokenizer::STokenizer(char str[])
{
    //_buffer[MAX_BUFFER] = str[];
    strcpy(_buffer, str);
    _pos = 0;
    int sub[MAX_ROWS][MAX_COLUMNS];
    make_table(_table);
//    for(int i = 0; i < MAX_ROWS; ++i)
//    {
//        for(int j = 0; j < MAX_COLUMNS; ++j)
//        {
//            _table[i][j] = sub[i][j];
//        }
//    }
}

STokenizer::STokenizer()
{
    _buffer[MAX_BUFFER] = '\0';
    _pos = 0;
    int sub[MAX_ROWS][MAX_COLUMNS];
    make_table(sub);
    for(int i = 0; i < MAX_ROWS; ++i)
    {
        for(int j = 0; j < MAX_COLUMNS; ++j)
        {
            _table[i][j] = sub[i][j];
        }
    }
}

void STokenizer::get_token(string str, int &position, int _table[][MAX_COLUMNS],
               int state, string& token, int & type)
{
    // Little bit of code that helped me understand what is going on
//    char m;
//    m = str[0];
//    cout << "m = " << m << endl;
//    cout << "ASCII for m = " << int(m) << endl;
   char m;
   int n;
   int secP = 0;
   int start;
   start = position;
   bool done = false;
   do{
       bool over = false;
       m = str[position];
       n = int(m);
//       cout << "m = " << m << endl;
//       cout << "n = " << n << endl;
       if(4 == _table[state][n] && 0 == state && over == false)
       {
           state = 4;
           over = true;
           //cout << "here" << endl;
       }
       else if(1 == _table[state][n] && 0 == state && over == false)
       {
           state = 1;
           over = true;
           //cout << "here99" << endl;
       }
       else if(2 == _table[state][n] && 0 == state && over == false)
       {
           state = 2;
           over = true;
       }
       else if(-1 == _table[state][n] && 0 == state && over == false)
       {
           over = true;
           done = true;
           type = 1;
       }
       else if(6 == _table[state][n] && 0 == state && over == false)
       {
           over = true;
           done = true;
           type = 2;
       }
       if(1 == _table[state][n] && 1 == state && over == false)
       {
            //cout << "here2888" << endl;
            over = true;
       }
       else if(2 == _table[state][n] && 1 == state && over == false)
       {
           if(position + 1 == str.size())
           {
               //cout << "TRUE" << endl;
               type = 3;

               state = 1;
           }
           else if(position + 1 != str.size())
           {
               state = 2;
           }
           over = true;
           //cout << "here3" << endl;
       }
       else if(-1 == _table[state][n] && 1 == state && over == false)
       {
           over = true;
           done = true;
           --position;
           type = 3;
           //cout << "HIT" << endl;
       }
       if(3 == _table[state][n] && 2 == state && over == false)
       {
           state = 3;
           over = true;
//           cout << "here4";
//           cout << "_table STATE = " << _table[2][n] << endl;
       }
       else if(-1 == _table[state][n] && 2 == state && over == false)
       {
           over = true;
           done = true;
           if(1 == secP)
           {
               type = 1;
           }
           else if(1 != secP)
           {
               type = 3;
           }
           --position;
           --secP;
//            cout << "_table STATE = " << _table[2][n] << endl;
//            cout << "n = " << n << endl;
       }
       if(3 == _table[state][n] && 3 == state && over == false)
       {
            //cout << "here5" << endl;
            over = true;
       }
       else if(-1 == _table[state][n] && 3 == state && over == false)
       {
            over = true;
            done = true;
            --position;
            --secP;
            type = 3;
       }
       if(4 == _table[state][n] && 4 == state && over == false)
       {
           over = true;
       }
       else if(5 == _table[state][n] && 4 == state && over == false)
       {
           over = true;
           state = 5;
       }
       else if(-1 == _table[state][n] && 4 == state && over == false)
       {
           over = true;
           done = true;
           --position;
           type = 4;
       }
       if(5 == _table[state][n] && 5 == state && over == false)
       {
           over = true;
       }
       else if(4 == _table[state][n] && 5 == state && over == false)
       {
           over = true;
           state = 4;
       }
       else if(-1 == _table[state][n] && 5 == state && over == false)
       {
           over = true;
           done = true;
           --position;
           type = 4;
       }
       //cout << "state = " << state << endl << endl;
       if(position + 1 == str.size())
       {
           if(0 == state || 1 == state || 3 == state )
           {
               if(2 == _table[state][n])
               {
                   type = 3;
                   --position;
               }
               else if(6 == _table[state][n])
               {
                   type = 2;
               }

               else
               {
                   type = 3;
               }

           }
           else if (2 == state)
           {
                type = 1;
           }
           else if(4 == state || 5 == state)
           {
               type = 4;
           }
           done = true;
       }
       if(done == false)
       {
           ++position;
           ++secP;
       }
   } while(done == false);

   if(is_success(_table,state))
   {
       for(int i = start; i <= position; ++i)
       {
           token = token + str[i];
       }
       ++position;
   }
   else if(!is_success(_table, state))
   {
       if(0 != secP)
       {
           --position;
           for(int i = start; i <= position; ++i)
           {
               char a;
               a = str[i];
               token = token + a;
           }
       }
       else if(0 == secP)
       {
           char  a;
           a = str[position];
           token = token + a;
       }
       ++position;
   }
   // Test to check table
//   cout << "table = " << _table[2][53] << endl;
//   cout << "token = " << token << endl;
//   cout << "token size = " << token.size() << endl;
//   cout << "type = " << type << endl;
}
void STokenizer::init_table(int _table[][MAX_COLUMNS])
{
    for(int i = 0; i < MAX_ROWS; ++i)
    {
        for(int j = 0; j < MAX_COLUMNS; ++j)
        {
            _table[i][j] = -1;
        }
    }
    // Test for int_table function
    //    for(int i = 0; i < MAX_ROWS; ++i)
    //    {
    //        for(int j = 0; j < MAX_COLUMNS; ++j)
    //        {
    //            cout << _table[i][j] << " ";
    //        }
    //        cout << endl;
    //    }
}
void STokenizer::mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state)
{
    _table[row][column] = state;
    // Test for mark_cell function
//    cout << _table[0][45] << " " << _table[0][46] << endl;
}
void STokenizer::mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to,
                int state)
{
    for(int i = from; i <= to; ++i)
    {
        _table[row][i] = state;
    }
    // Test for mark_cells function
//    for(int i = 48; i <= 57; ++i)
//    {
//        cout << _table[0][i] << " ";
//    }
}
void STokenizer::mark_fail(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 0;
    // Test for mark_success and mark_fail functions
//    cout << _table[0][0] << " " << _table[1][0] << " " << _table[2][0]
//            << " " << _table[3][0] << endl;
}
void STokenizer::mark_success(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 1;
    // Test for mark_success and mark_fail functions
//    cout << _table[0][0] << " " << _table[1][0] << " " << _table[2][0]
//            << " " << _table[3][0] << endl;
}
bool STokenizer::is_success(int _table[][MAX_COLUMNS], int state)
{
    if(_table[state][0] == 1)
    {
        return true;
    }
    else{
        return false;
    }
    // Test for is_success function
//    for(int i = 0; i < 4; ++i)
//    {
//        cout << is_success(_table,i) << " ";
//    }
}

void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    init_table(_table);
    mark_fail(_table,0);
    mark_success(_table,1);
    mark_fail(_table,2);
    mark_success(_table,3);
    mark_success(_table,4);
    mark_success(_table,5);
    mark_cells(0,_table,45,46,2);
    mark_cells(0,_table,'0','9',1);
    mark_cells(0,_table,'A','z',4);
    mark_cells(1,_table,48,57,1);
    mark_cells(1,_table,45,46,2);
    mark_cells(2,_table,48,57,3);
    mark_cells(3,_table,48,57,3);
    mark_cells(4,_table,48,57,5);
    mark_cells(4,_table,65,127,4);
    mark_cells(5,_table,48,57,5);
    mark_cells(5,_table,65,127,4);
    mark_cell(0,_table,32,6);
    // Test for int_table function
//    for(int i = 0; i < MAX_ROWS; ++i)
//    {
//        for(int j = 0; j < MAX_COLUMNS; ++j)
//        {
//            cout << _table[i][j] << " ";
//        }
//        cout << endl;
//    }
}





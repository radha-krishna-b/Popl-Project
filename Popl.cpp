#include<iostream>
#include<cstring>
#include<map>
#include<vector>
#include<cmath>
#include<algorithm>\
#include<cctype>


using namespace std;


string table_name = "STORE";

/*
Concrete Syntax
---------------

    (Expression) (Valid if in intital environment)
    (Expression) ( Expression relationop Number ) ((Valid Relation operators are >,<,>=,<=,!=,=) Expression should be a valid)
    (Expression) ( Expression relationop String ) ((Valid Relational Operators are = , !=) Expression should be a valid)
    (Expression) ( Expression relationop Number/String ) logicalop (Expression relationop Number/String) ((Valid logical operators are AND, OR) (both the conditions should be a valid expression))



    Valid Expressions:
    ------------------

    1.  ItemName
    2.  ItemCost
    3.  ItemColor
    4.  ItemQuantity
    5.  ItemName (Item_Name = "Lays")
    6.  ItemQuantity (Item_Name = "Lays") AND/OR (Item_Color = "green")
    7.  ItemName (NOT Item_Name = "Lays")
    8.  ItemName (Item_Name = "Lays") AND/OR (Item_Quantity > 10)


*/

vector<string> get_words (string expression){

    vector<string> words;

    string temp = "";
    bool is_alpha = false;
    for(int i =0;i<expression.size();i++){

        if(expression[i] != ' '){
            temp += expression[i];
            is_alpha = true;
        }
        else{
            if(is_alpha == true){
                words.push_back(temp);
                temp = "";
                is_alpha = false;
            }
            else{
                continue;
            }
        }
    }
    if(temp != ""){
        words.push_back(temp);
    }
    return words;

}



int count_words(string expression){
    int count = 0;
    if(expression.size() == 0){
        return count;
    }
    else{
        bool omit_white_space = false;
        bool is_alpha = false;
        for(int i=0;i<expression.size();i++){
            if((is_alpha == true) && (expression[i] == ' ') && (omit_white_space == false)){
                count += 1;
                omit_white_space = true;
            }
            else if((expression[i] != ' ') && (omit_white_space == true)){
                omit_white_space = false;
            }
            else if(expression[i] != ' '){
                is_alpha = true;
            }
        }
    }
    return count+1;

}

bool is_number (string expression){

    for(int i =0;i<expression.size();i++){
        int temp = expression[i];
        if((temp >= '0') && (temp <='9')){
            continue;
        }
        else{
            return false;
        }
    }
    return true;
}

bool is_alpha_string(string expression){

    transform(expression.begin(), expression.end(), expression.begin(), ::tolower);

    if((expression[0] == '"') && (expression[expression.size()-1] == '"')){
        if(expression.size() > 2){
            for(int i = 1;i<expression.size()-1;i++){
                char temp = expression[i];
                if((temp >='a') && (temp <= 'z')){
                    continue;
                }
                return false;
            }
            return true;
        }
        else{
            return false;
        }
    }
    else{
        for(int i = 0;i<expression.size();i++){
            char temp = expression[i];
            if((temp >='a') && (temp <= 'z')){
                continue;
            }
            return false;
        }
        return true;
    }

}


bool lookup_initial_environment(string expression , vector<string> initial_environment){

    for(int i =0;i<initial_environment.size();i++){

        if(initial_environment[i] == expression){
            return true;
        }
    }

    return false;
}

int lookup_extended_environment(string expression, map<string,int> extended_environment){

    return 0;
}

bool is_relation_operator(string expression){

    if((expression == "=") || (expression == "<") || (expression == "!=") || (expression == ">") || (expression == ">=")
    || (expression == "<=")){
        return true;
    }
    else{
        return false;
    }
}

bool is_logical_operator(string expression){

    if((expression == "AND") || (expression == "OR") || (expression == "NOT")){
        return true;
    }
    else{
        return false;
    }
}

bool parse_expression(string expression, vector<string> initial_environment,
map<string,int> extended_environment, string& parsed_string){

    int number_words = count_words(expression);
    //cout<<"Number of Words : "<<number_words<<endl;
    cout<<endl;
    cout<<"Parsing . . . "<<expression<<" "<<endl<<endl;
    if(number_words == 1){
        if(is_alpha_string(expression) == true){
            if(lookup_initial_environment(expression,initial_environment) == true){
                cout<<"Parsed successfully : "<<expression<<endl<<endl;
                string curr = "";
                curr += ("( id " + (expression) + " )");
                parsed_string += curr;
                return true;
            }
            else if(( is_logical_operator(expression) == true ) && (extended_environment[expression] == -1)){
                string curr = "";
                curr += ("( logicalop " + (expression) + " )");
                parsed_string += curr;
                cout<<"Parsed successfully : "<<expression<<endl<<endl;
                return true;
            }
            else if(extended_environment[expression] == -1){
                string curr = "";
                curr += ("( string " + (expression) + " )");
                parsed_string += curr;
                cout<<"Parsed successfully : "<<expression<<endl<<endl;
                return true;
            }
            else{
                cout<<"Invalid Expression : Column not present"<<endl;
                return false;
            }
        }
        else{
            if(is_relation_operator (expression) == true){
                if(extended_environment[expression] == 0){
                     cout<<"Invalid Expression : Not a valid Column Name"<<endl;
                     return false;
                }
                else{
                    string curr = "";
                    curr += ("( relationalop " + (expression) + " )");
                    parsed_string += curr;
                    cout<<"Parsed successfully : "<<expression<<endl<<endl;
                    return true;
                }
            }
            else if (is_number(expression) == true){
                if(extended_environment[expression] == 0){
                     cout<<"Invalid Expression : Not a valid Column Name"<<endl;
                     return false;
                }
                else{
                    string curr = "";
                    curr += ("( num " + (expression) + " )");
                    parsed_string += curr;
                    cout<<"Parsed successfully : "<<expression<<endl<<endl;
                    return true;
                }
            }
            else{
                cout<<"Invalid Expression : Not a valid Column Name"<<endl;
                return false;
            }

        }
    }
    else{

        vector<string> words = get_words(expression);
        bool flag_1 = parse_expression(words[0],initial_environment,extended_environment,parsed_string);
        vector<string> temp_words;


        if(flag_1 == true){
            vector<string> parenthesis_stack;
            bool is_started = false;
            for(int i = 1;i<words.size();i++){

                if(is_logical_operator(words[i]) == true){
                    if(words[i-1] == ")" && (((i+1) < words.size()) && words[i+1] == "(")){
                        extended_environment[words[i]] = -1;
                        bool flag = parse_expression(words[i],initial_environment,extended_environment,parsed_string);
                        if(flag == false){
                            return false;
                        }
                        else{
                            continue;
                        }
                    }
                    else{
                        cout<<"Invalid Expression";
                        false;
                    }
                }

                if(words[i] == "("){
                    parenthesis_stack.push_back("(");
                    parsed_string += "(";
                    is_started = true;
                }
                else if( (parenthesis_stack.size() > 0) && (words[i] != "(") && (words[i] != ")")){
                     temp_words.push_back(words[i]);
                }
                else if(words[i] == ")"){
                    if(parenthesis_stack.size() > 0){

                        for(int j = 0;j<temp_words.size();j++){

                            if(is_logical_operator(temp_words[j]) == true){
                                cout<<"Invalid Expression";
                                return false;
                            }

                            extended_environment[temp_words[j]] = -1;
                            bool flag = parse_expression(temp_words[j],initial_environment,extended_environment,parsed_string);
                            if(flag == false){
                                return false;
                            }
                        }
                        parsed_string += ")";
                        parenthesis_stack.pop_back();
                        temp_words.clear();
                    }
                    else{
                        cout<<"Invalid Expression : Check parenthesis";
                        return false;
                    }
                }
            }

            if((parenthesis_stack.size() > 0) || (is_started == false)){
                cout<<"Invalid Expression : Check parenthesis";
                return false;
            }
            return true;

        }
    }
}

string interpret_expression(string expression, vector<string> initial_environment,
map<string,int> extended_environment, vector<string> commands){

    string sql_statement = "";

    int number_words = count_words(expression);

    if(number_words == 1){
        sql_statement += (commands[0] + " " + expression + " " + commands[1] + " " + table_name);
        return sql_statement;
    }
    else if(number_words = 6){

        vector<string> words = get_words(expression);
        sql_statement = interpret_expression(words[0],initial_environment,extended_environment,commands);
        sql_statement += (" " + commands[2] + " ");
        for(int i  = 1;i<words.size()-1;i++){
            sql_statement += (words[i] + " ");
        }
        sql_statement += words[words.size()-1];
        return sql_statement;
    }


    return sql_statement;
}




int main(){


    vector<string> commands;
    commands.push_back("SELECT");
    commands.push_back("FROM");
    commands.push_back("WHERE");

    // Will contain key_value pairs which can be used during parsing and evaluation
    map<string,int> extended_environment;

    vector<string> initial_environment;
    //intial environment will have all the column names specified in the table by default
    // We are considering a simple table which has 4 columns and this can be extended to any number of columns
    // 1. Item_Name - String (consisits of alphabets) (Data Type)
    // 1. Item_Cost - int (Data Type)
    // 1. Item_Quantity - int (Data Type)
    // 1. Item_Color - String (consisits of alphabets) (Data Type)
    initial_environment.push_back("ItemName");
    initial_environment.push_back("ItemCost");
    initial_environment.push_back("ItemQuantity");
    initial_environment.push_back("ItemColor");



    cout<<"Welcome to DSL for SQL Statement-Select on the table : "<<table_name<<endl<<endl;
    cout<<"Press q to quit"<<endl;

    while(1){
        cout<<endl;
        cout<<"Enter the expression : "<<endl;
        string expression;
        getline(cin, expression);
        //cin>>expression;
        cout<<endl;
        cout<<"You have entered : "<<endl;
        cout<<expression<<endl;

        if(expression == "q"){
            cout<<"Quitting . ."<<endl;
            break;
        }
        string parsed_string = "";
        bool is_parsed = parse_expression(expression,initial_environment,extended_environment,parsed_string);
        string sql_statement= "";
        if(is_parsed == true){
            cout<<"Parsed String : "<<parsed_string<<endl<<endl;
            cout<<"Evaluating . . "<<endl<<endl;
            sql_statement = (interpret_expression(expression,initial_environment,extended_environment,commands) + ";");
            cout<<"Evalauted Statement : "<<endl<<endl;
            cout<<sql_statement<<endl;
        }

    }




return 0;
}

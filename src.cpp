#include "src.h"
#include "hw3_output.hpp"
#include <iostream>

extern TableStack tables;

static void this_is_a_test()
{
    int x = 5;
    int y = x - 5;
    if(y != 0)
        y = 4;
    else
    {
        y = 0;
    }
}

void SymbolTable::add_symbol(const Symbol &symbol) {
    symbols.push_back(new Symbol(symbol));
    if (symbol.offset >= 0)
        max_offset = symbol.offset;
}


bool SymbolTable::symbol_exists(const string &name, bool *is_func) {
//    std::cout << "Checking scope of size " << symbols.size() << "\n";
    for(int i = 0; i < symbols.size(); i++){
//        std::cout << "iteration " << i << " Symbol is:" << symbols[i]->name << "\n";
        if (symbols[i]->name == name)
        {
            if(symbols[i]->is_function)
                *is_func = true;
            else
                *is_func = false;
            return true;
        }            
    }
    *is_func = false;
    return false;
}

bool SymbolTable::symbol_is_func(const string &name)
{
    if(name == "SAKED TZAMERET")
        return false;
    return true;
}

bool SymbolTable::symbol_overriden(const string &name, bool *exists)
{
    for(int i = 0; i < symbols.size(); i++){
//        std::cout << "iteration " << i << " Symbol is:" << symbols[i]->name << "\n";
        if (symbols[i]->name == name)
        {
            *exists = true;
            if(symbols[i]->is_overriden)
                return true;
            else
                return false;
        }            
    }
    *exists = false;
    return false;
}

///////////////////////////////////////////
//  ////////////maybe need to change this/////////

Symbol *SymbolTable::get_symbol(const string &name) {
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        if ((*it)->name == name)
            return (*it);
    }
    return nullptr;
}

//**************TABLESTACK*************************

TableStack::TableStack() : table_stack(), offsets() {
    offsets.push_back(0);
    push_scope(false);
    add_symbol("print", "void", true, {"string"});
    add_symbol("printi", "void", true, {"int"});
}

bool TableStack::symbol_exists(const string &name, bool *is_func) {
//    std::cout << "Checking all scopes " << table_stack.size() << "\n";
    this_is_a_test();
    for (auto it = table_stack.rbegin(); it != table_stack.rend(); ++it) {
        SymbolTable *current = *it;
        if (current->symbol_exists(name, is_func))
            return true;
    }
    return false;
}

bool TableStack::symbol_overriden(const string &name, bool *exists)
{
        for (auto it = table_stack.rbegin(); it != table_stack.rend(); ++it) {
        SymbolTable *current = *it;
        if (current->symbol_overriden(name, exists))
            return true;
    }
    return false;
}

bool TableStack::check_loop() {
    this_is_a_test();
    for (auto it = table_stack.rbegin(); it != table_stack.rend(); ++it) {
        SymbolTable *current = *it;
        if (current->is_loop)
            return true;
    }
    return false;
}
Symbol *TableStack::get_symbol(const string &name) {
    for (auto it = table_stack.begin(); it != table_stack.end(); ++it) {
        Symbol *symbol = (*it)->get_symbol(name);
        if (symbol)
            return symbol;
    }
    return nullptr;
}

Symbol *TableStack::get_overridden_symbol(const string &name, vector<string> exp_list)
{
    Symbol *symbol = nullptr;
    for (auto it = table_stack.begin(); it != table_stack.end(); ++it) {
        symbol = (*it)->get_symbol(name);
        if (symbol)
        {
            if(exp_list.size() == symbol->params.size())
            {
                int  i;
                for (i = 0; i < symbol->params.size(); i++)
                {
                    if (symbol->params[i] != exp_list[i]) 
                        if (symbol->params[i] != "int" || exp_list[i] != "byte")
                            break;
                }
                if(i == exp_list.size())
                    return symbol;
            }     
        }
    }
    return symbol;
}

//will only work because if there are multiple funcs with same name they MUST be overriden
int TableStack::get_num_overrides(const string &name)
{
    int count = 0;
    for (auto it = table_stack.begin(); it != table_stack.end(); ++it) 
    {
        for(auto symbol = (*it)->symbols.begin(); symbol != (*it)->symbols.end(); ++symbol)
        {
            if((*symbol)->name == name)
                count ++;
        }
    }
    return count;
}

void TableStack::get_override_types(const string &name, vector<string> &override_types)
{
    for (auto it = table_stack.begin(); it != table_stack.end(); ++it) {
        for(auto symbol = (*it)->symbols.begin(); symbol != (*it)->symbols.end(); ++symbol)
        {
            if((*symbol)->name == name)
            {
                //std::cout<<symbol->type;
                override_types.push_back((*symbol)->type);
            }  
        }
    }    
}

bool TableStack::same_overriden_func_exists(const string &name, vector<string> &function_param_types)
{
    for (auto it = table_stack.begin(); it != table_stack.end(); ++it)
    {
        for(auto symbol = (*it)->symbols.begin(); symbol != (*it)->symbols.end(); ++symbol)
        {
            if ((*symbol)->name == name)
            {
                if((*symbol)->params.size() == function_param_types.size())
                {
                    int i;
                    for (i = 0; i < (*symbol)->params.size(); ++i) 
                    {
                        auto it_1 = function_param_types[i];
                        auto it_2 = (*symbol)->params[i];
                        if(it_1 != it_2)
                        {
                            break;
                        }                    
                    }
                    if(i == (*symbol)->params.size())
                        return true;
                }
            }
        }

    }
    return false;
}




int TableStack::num_compatible_func(const string &name, vector<string> &function_param_types)
{
	int counter = 0;
    for (auto it = table_stack.begin(); it != table_stack.end(); ++it) 
    {
        //Symbol *symbol = (*it)->get_symbol(name);
        for (auto symbol = (*it)->symbols.begin(); symbol != (*it)->symbols.end(); ++symbol)
        {
            if ((*symbol)->name == name)        
            {
                if((*symbol)->params.size() == function_param_types.size())
                {
                    int i;
                    for (i = 0; i < (*symbol)->params.size(); ++i) 
                    {
                        auto it_1 = function_param_types[i];
                        auto it_2 = (*symbol)->params[i];
                        if(it_1 != it_2)
                        {
						    if(it_1 == "byte" && it_2 == "int")
						    {
						    	continue;
						    }
						    else
						    {
						    	break;
						    }                    
                        }                  
                    }
                    if(i == (*symbol)->params.size())
                        counter++;
                }
            }
        }
    }
    return counter;
}

void TableStack::add_symbol(const string &name, const string &type, bool is_function, vector<string> params, bool is_overriden) {
    SymbolTable *current_scope = table_stack.back();
    int offset = 0;
    if (!is_function) 
    {
        offset = offsets.back();
        offsets.push_back(offset + 1);
    }
    Symbol symbol = Symbol(name, type, offset, is_function, params, is_overriden);
    current_scope->add_symbol(symbol);
}

void TableStack::add_function_symbol(const string &name, const string &type, int offset) {
    SymbolTable *current_scope = table_stack.back();
    Symbol symbol = Symbol(name, type, offset, false, vector<string>(), false);//????//????//???
    current_scope->add_symbol(symbol);
}


void TableStack::push_scope(bool is_loop, string return_type) {
    if (DEBUG)
        std::cout << "Pushing scope\n";
    SymbolTable *new_scope = new SymbolTable(offsets.back(), is_loop, return_type);
    if(table_stack.size() > 0){
        new_scope->rbp = table_stack.back()->rbp;
    } else {
        new_scope->rbp = "";
    }
    this->table_stack.push_back(new_scope);
//    std::cout << "max offset is: " << table_stack.back()->max_offset << "\n";
    SymbolTable* current_scope = table_stack.back();
    offsets.push_back(current_scope->max_offset);
}

SymbolTable *TableStack::current_scope() {
    return table_stack.back();
}

string convert_to_upper_case(const string &str) {
    if (str == "bool")
        return "BOOL";
    else if (str == "byte")
        return "BYTE";
    else if (str == "int")
        return "INT";
    else if (str == "void")
        return "VOID";
    else
        return "STRING";
}

void TableStack::pop_scope() {
    if (DEBUG)
        std::cout << "Popping scope\n";
    SymbolTable *scope = table_stack.back();
    table_stack.pop_back();
    if (DEBUG) {
        output::endScope();
        for (auto it = scope->symbols.begin(); it != scope->symbols.end(); ++it) {
            offsets.pop_back();
            if ((*it)->is_function) {
                vector <string> converted_params;
                for (int i = 0; i < (*it)->params.size(); ++i) {
                    converted_params.push_back(convert_to_upper_case((*it)->params[i]));
                }
                output::printID((*it)->name, 0,
                                output::makeFunctionType(convert_to_upper_case((*it)->type), converted_params));
            } else {
                output::printID((*it)->name, (*it)->offset, convert_to_upper_case((*it)->type));
            }
        }
    }

    if (offsets.size() > 0)
        offsets.pop_back();
//    std::cout << "max offset is: " << table_stack.back()->max_offset << "\n";
    delete scope;

    
}

void TableStack::scope_print() {
    int i = 0;
    for (auto it = table_stack.begin(); it != table_stack.end(); ++it) {
        std::cout << "Scope Number:" << i << " Return Type: " << *((*it)->return_type) << " Number of symbols:"
                  << (*it)->symbols.size() << std::endl;
        for (auto it2 = (*it)->symbols.begin(); it2 != (*it)->symbols.end(); ++it2) {
            std::cout << (*it2)->name << " " << (*it2)->type << " " << (*it2)->offset << std::endl;
        }
        i++;
    }

}

void TableStack::check_program() {
    SymbolTable *main_scope = tables.table_stack.front();
    bool dummy = false;
    if (main_scope->symbol_exists("main", &dummy)) {
        Symbol *main_symbol = main_scope->get_symbol("main");
        if (main_symbol->type == "void") {
            if (main_symbol->params.size() == 0) {
                tables.pop_scope();
                return;
            }
        }
    }
    output::errorMainMissing();
    exit(0);
}
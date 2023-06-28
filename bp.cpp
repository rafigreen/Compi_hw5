#include "bp.hpp"
#include "gen.h"
#include "src.h"
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

extern Generator code_gen;

bool replace(string& str, const string& from, const string& to, const BranchLabelIndex index);

CodeBuffer::CodeBuffer() : buffer(), globalDefs() {}

CodeBuffer &CodeBuffer::instance() {
	static CodeBuffer inst;//only instance
	return inst;
}

string CodeBuffer::genLabel(){
	std::stringstream label;
	label << "label_";
	label << buffer.size();
	std::string ret(label.str());
//	label << ":";
//	emit(label.str());
	return ret;
}

int CodeBuffer::emit(const string &s){
    if(DEBUG)
        std::cout << "code generated: " <<  s << "\n";
    buffer.push_back(s);
	return buffer.size() - 1;
}

void CodeBuffer::emit_init(){
    code_gen.generate_global_code();
    declare_externs();
    define_prints();
}

void CodeBuffer::bpatch(const vector<pair<int,BranchLabelIndex>>& address_list, const std::string &label){
    if(DEBUG){
        std:cout << "backpatching " << address_list.size() <<  " addresses: " + label + " \n";
        for(int i = 0; i < address_list.size(); i++){
            std::cout << address_list[i].first << ", ";
            if(i == address_list.size() - 1)
                std::cout << "\n";
        }

    }

    for(vector<pair<int,BranchLabelIndex>>::const_iterator i = address_list.begin(); i != address_list.end(); i++){
    	int address = (*i).first;
    	BranchLabelIndex labelIndex = (*i).second;
		replace(buffer[address], "@", "%" + label, labelIndex);
    }
}

void CodeBuffer::printCodeBuffer(){
	for (std::vector<string>::const_iterator it = buffer.begin(); it != buffer.end(); ++it) 
	{
		cout << *it << endl;
    }
}

vector<pair<int,BranchLabelIndex>> CodeBuffer::makelist(pair<int,BranchLabelIndex> item)
{
	vector<pair<int,BranchLabelIndex>> newList;
	newList.push_back(item);
	return newList;
}

vector<pair<int,BranchLabelIndex>> CodeBuffer::merge(const vector<pair<int,BranchLabelIndex>> &l1,const vector<pair<int,BranchLabelIndex>> &l2)
{
	vector<pair<int,BranchLabelIndex>> newList(l1.begin(),l1.end());
	newList.insert(newList.end(),l2.begin(),l2.end());
	return newList;
}

// ******** Methods to handle the global section ********** //
void CodeBuffer::emitGlobal(const std::string& dataLine) 
{
	globalDefs.push_back(dataLine);
}

void CodeBuffer::printGlobalBuffer()
{
	for (vector<string>::const_iterator it = globalDefs.begin(); it != globalDefs.end(); ++it)
	{
		cout << *it << endl;
	}
}

// ******** Helper Methods ********** //
bool replace(string& str, const string& from, const string& to, const BranchLabelIndex index) {
	size_t pos;
	if (index == SECOND) {
		pos = str.find_last_of(from);
	}
	else { //FIRST
		pos = str.find_first_of(from);
	}
    if (pos == string::npos)
        return false;
    str.replace(pos, from.length(), to);
    return true;
}


CodeBuffer::CodeBuffer(const CodeBuffer &): buffer(), globalDefs() {

}

void CodeBuffer::operator=(const CodeBuffer &) {

}

void CodeBuffer::declare_externs(){
    emit("@.intFormat = internal constant [4 x i8] c\"%d\\0A\\00\"");
    emit("@.DIVIDE_BY_ZERO.str = internal constant [23 x i8] c\"Error division by zero\\00\"");

    emit("declare i32 @printf(i8*, ...)");
    emit("declare void @exit(i32)");

    emit("@.int_specifier = constant [4 x i8] c\"%d\\0A\\00\"");
    emit("@.str_specifier = constant [4 x i8] c\"%s\\0A\\00\"");
}
void CodeBuffer::define_prints(){
    emit("define void @print(i8*){");
    emit("call i32 (i8*, ...) @printf(i8* getelementptr([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)");
    emit("ret void");
    emit("}");

    emit("define void @printi(i32){");
    emit("%format_ptr = getelementptr [4 x i8], [4 x i8]* @.intFormat, i32 0, i32 0");
    emit("call i32 (i8*, ...) @printf(i8* getelementptr([4 x i8], [4 x i8]* @.intFormat, i32 0, i32 0), i32 %0)");
    emit("ret void");
    emit("}");
}


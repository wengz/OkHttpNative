//
// Created by wengzc on 2019/4/22.
//

#include "Headers.h"
#include "Util.h"

void Headers::checkName(string name) {


}

void Headers::checkValue(string value, string name){


}


Headers::Headers(Builder & builder) {
    size_t size = builder.nameAndValues.size();
    this->nameAndValues = new string[size];
    this->nameAndValuesSize = 0;
    for (int i = 0; i < size; i++) {
        (this->nameAndValues)[i] = builder.nameAndValues.at(i);
        this->nameAndValuesSize++;
    }
}

Headers::Headers(const Headers &rhs): nameAndValuesSize(rhs.nameAndValuesSize) {
    nameAndValues = new string[nameAndValuesSize];
    memcpy(nameAndValues, rhs.nameAndValues, sizeof(string)*nameAndValuesSize);
}

Headers &Headers::operator=(const Headers &rhs) {
    nameAndValuesSize = rhs.nameAndValuesSize;
    nameAndValues = new string[nameAndValuesSize];
    memcpy(nameAndValues, rhs.nameAndValues, sizeof(string)*nameAndValuesSize);
    return * this;
}

Headers::~Headers() {
    delete[] nameAndValues;
}

Headers::Builder & Headers::Builder::add (string name, string value){
    checkName(name);
    checkValue(value, name);
    return addLenient(name, value);
}

Headers::Builder & Headers::Builder::addLenient(string line) {
    size_t index = line.find(":", 1);
    if (index != string::npos) {
        return addLenient(line.substr(0, index), line.substr(index + 1));
    } else if (line[0] == ':') {
        return addLenient("", line.substr(1));
    } else {
        return addLenient("", line);
    }
}

Headers::Builder & Headers::Builder::removeAll (string name){
    auto itr = nameAndValues.begin();
    while (itr != nameAndValues.end()){
        if (strcasecmp(name.c_str(), (*itr).c_str()) == 0){
            itr = nameAndValues.erase(itr, itr + 1);
        }else{
            itr += 2;
        }
    }
    return *this;
}

Headers::Builder & Headers::Builder::add(string line) {
    size_t index = line.find(":");
    if (index == string::npos) {
        throw logic_error("Unexpected header: " + line);
    }
    string name = line.substr(0, index);
    name = trim(name);
    string value = line.substr(index + 1);
    return add(name, value);
}

Headers::Builder & Headers::Builder::addLenient (string name, string value){
    nameAndValues.push_back(name);
    nameAndValues.push_back(trim(value));
    return *this;
}

Headers * Headers::Builder::build() {
    return new Headers(*this);
}
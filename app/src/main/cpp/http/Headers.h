//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_HEADERS_H
#define OKHTTPNATIVE_HEADERS_H

#include <string>
#include <vector>
#include <strings.h>

using  namespace std;

class Headers {

public :

    static void checkName(string name);

    static void checkValue(string value, string name);

    //构造器
    class Builder {

    public :

        friend class Headers;

        Builder (){}

        Builder & addLenient (string line);

        Builder & add (string line);

        Builder & add (string name, string value);

        Builder & addLenient (string name, string value);

        Builder & removeAll (string name);

        Headers * build ();

    private :
        vector<string> nameAndValues;
    };
    //--构造器--end


    Headers(Builder & builder);

    Headers (){}

    Builder & newBuilder  (){
        Builder * res = new Builder;
        for (int i = 0; i < nameAndValuesSize; i++){
            res->nameAndValues.push_back(*(nameAndValues+i));
        }
        return *res;
    }

    size_t size() const{
        return nameAndValuesSize/2;
    }

    string name(int index) const {
        return nameAndValues[index*2];
    }

    string value (int index) const {
        return nameAndValues[index * 2 + 1];
    }

private :
    string * nameAndValues;
    size_t nameAndValuesSize;

};



#endif //OKHTTPNATIVE_HEADERS_H

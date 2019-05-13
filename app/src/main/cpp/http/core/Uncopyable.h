//
// Created by wengzc on 2019/5/12.
//

#ifndef OKHTTPNATIVE_UNCOPYABLE_H
#define OKHTTPNATIVE_UNCOPYABLE_H

class Uncopyable {

protected:

    Uncopyable(){}

    ~Uncopyable(){}

private :
    Uncopyable(const Uncopyable &);
    Uncopyable & operator=(const Uncopyable &);
};

#endif //OKHTTPNATIVE_UNCOPYABLE_H

//
// Created by pe200012 on 2021/09/20.
//

#ifndef JSONLIB_AST_H
#define JSONLIB_AST_H

class JSONValue {

};

class JSONNull : public JSONValue {

};

class JSONBoolean : public  JSONValue {

};

class JSONTrue : public JSONBoolean {

};

class JSONFalse : public JSONBoolean {

};

class JSONArray : public JSONValue {

};

class JSONObject : public JSONValue {

};

class JSONNumber : public JSONValue {

};

class JSONString : public JSONValue {

};

#endif //JSONLIB_AST_H

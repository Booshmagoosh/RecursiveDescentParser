#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <iostream>

#include "Type.h"

struct Value {
	int type = Type::Float;
	void* data = nullptr;

	Value() = default;

	Value(int number) {
		this->type = Type::Float;
		this->data = new float;
		*(float*)this->data = static_cast<float>(number);
	}

	Value(float number) {
		this->type = Type::Float;
		this->data = new float;
		*(float*)this->data = number;
	}

	Value(const Value& other) {
		this->type = other.type;
		free(this->data);
		this->data = new float;
		*(float*)this->data = *(float*)other.data;
	}

	~Value() {
		free(this->data);
		this->data = nullptr;
	}
	
	Value operator=(const Value& other) {
		this->type = other.type;
		free(this->data);
		this->data = new float;
		*(float*)this->data = *(float*)other.data;
		return *this;
	}

	Value operator +(const Value &value) const {
		Value result;
		result.type = value.type;
		switch(value.type) {
			case Type::Float:
				result.data = new float;
				*(float*)result.data = *(float*)this->data + *(float*)value.data;
				break;
		}
		return result;
	}

	Value operator -(const Value &value) const {
		Value result;
		result.type = value.type;
		switch(value.type) {
			case Type::Float:
				result.data = new float;
				*(float*)result.data = *(float*)this->data - *(float*)value.data;
				break;
		}
		return result;
	}

	Value operator *(const Value &value) const {
		Value result;
		result.type = value.type;
		switch(value.type) {
			case Type::Float:
				result.data = new float;
				*(float*)result.data = *(float*)this->data * *(float*)value.data;
				break;
		}
		return result;
	}

	Value operator /(const Value &value) const {
		Value result;
		result.type = value.type;
		switch(value.type) {
			case Type::Float:
				result.data = new float;
				*(float*)result.data = *(float*)this->data / *(float*)value.data;
				break;
		}
		return result;
	}

	std::string toString() {
		switch (this->type)
		{
		case Type::Float:
			return std::to_string(*(float*)this->data);
		}
		return "null";
	}
};

#endif // VALUE_H

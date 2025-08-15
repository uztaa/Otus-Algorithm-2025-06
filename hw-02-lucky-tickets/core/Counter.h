#pragma once

/* Абстрактный класс для подсчета чего-то. */
class Counter {
public:
	/* Что-то считает */
	virtual int64_t count(int n) const = 0LL;

	virtual ~Counter() = default;
};
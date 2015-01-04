/*******************************
Copyright (C) 2013-2015 gregoire ANGERAND

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************/

#ifndef N_CONCURENT_ASYNC_H
#define N_CONCURENT_ASYNC_H

#include "Promise.h"
#include <n/core/Functor.h>

namespace n {
namespace concurent {

template<typename T>
class AsyncTask : public Thread
{
	public:
		AsyncTask(const Promise<T> &p, const core::Functor<T()> &f) : func(f), promise(p) {
			deleteLater();
		}

		virtual void run() override {
			promise.success(func());
		}

	private:
		core::Functor<T()> func;
		Promise<T> promise;
};

template<>
class AsyncTask<void> : public Thread
{
	public:
		AsyncTask(const Promise<void> &p, const core::Functor<void()> &f) : func(f), promise(p) {
			deleteLater();
		}

		virtual void run() override {
			func();
			promise.success();
		}

	private:
		core::Functor<void()> func;
		Promise<void> promise;
};

template<typename R>
static SharedFuture<R> Async(const core::Functor<R()> &f) {
	Promise<R> promise;
	(new AsyncTask<R>(promise, f))->start();
	return promise.getFuture();
}

template<typename T, typename... Args, typename R = typename std::result_of<T(Args...)>::type>
static SharedFuture<R> Async(const T f, Args... args) {
	return Async<R>(core::Functor<R(Args...)>(f).curried(args...));
}

}
}


#endif // N_CONCURENT_ASYNC_H

export module common.task;
import std;

// Single-use synchronous tasks with non-void object results.
export template<class T>
class task {
public:
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        std::optional<T> result;
        std::exception_ptr error;
        std::coroutine_handle<> parent = std::noop_coroutine();

        task get_return_object() {
            return task{handle::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        void return_value(T value) { result.emplace(std::move(value)); }
        void unhandled_exception() noexcept {
            error = std::current_exception();
        }

        struct final_awaiter {
            bool await_ready() noexcept { return false; }
            std::coroutine_handle<> await_suspend(handle h) noexcept {
                return h.promise().parent;
            }
            void await_resume() noexcept {}
        };
        final_awaiter final_suspend() noexcept { return {}; }

        // This task supports awaiting only other synchronous tasks.
        template<class U>
        task<U>&& await_transform(task<U>&& child) noexcept {
            return std::move(child);
        }
    };

    task(const task&) = delete;
    task(task&& other) noexcept : h_(std::exchange(other.h_, {})) {}
    ~task() { if (h_) h_.destroy(); }

    auto operator co_await() && noexcept {
        struct awaiter {
            handle h;
            bool await_ready() noexcept { return false; }
            std::coroutine_handle<> await_suspend(
                std::coroutine_handle<> parent) noexcept {
                h.promise().parent = parent;
                return h;
            }
            T await_resume() { return task::take_result(h); }
        };
        return awaiter{h_};
    }

    T run() && {
        task owner = std::move(*this);
        owner.h_.resume();
        return take_result(owner.h_);
    }

private:
    explicit task(handle h) : h_(h) {}
    handle h_;

    static T take_result(handle h) {
        if (h.promise().error)
            std::rethrow_exception(h.promise().error);
        return std::move(*h.promise().result);
    }
};

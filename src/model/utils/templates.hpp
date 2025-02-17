#ifndef TEMPLATES_HPP
#define TEMPLATES_HPP

#include <vector>

namespace Utils {
    namespace Templates {

        template <typename T>
        int hash(T t) {
            return t.hash();
        }

        template <typename T>
        int hash_vector(std::vector<T> v) {
            size_t seed = v.size();

            for (const T &element : v) {
                seed ^= element.hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        };

        template <typename T>
        class UndoRedo {
          private:
            std::vector<T> _done;
            std::vector<T> _undone;

          public:
            UndoRedo() {};

            void do_(const T &t) {
                _done.push_back(t);
                _undone.clear();
            };

            T undo() {
                if (_done.empty()) throw std::runtime_error("Nothing to undo");

                T t = _done.back();
                _done.pop_back();
                _undone.push_back(t);
                return t;
            };

            T redo() {
                if (_undone.empty()) throw std::runtime_error("Nothing to redo");

                T t = _undone.back();
                _undone.pop_back();
                _done.push_back(t);
                return t;
            };

            T peek() {
                if (_done.empty()) throw std::runtime_error("Nothing to peek");

                return _done.back();
            };

            bool canUndo() { return !_done.empty(); };

            bool canRedo() { return !_undone.empty(); };
        };
    } // namespace Templates
} // namespace Utils

#endif // TEMPLATES_HPP

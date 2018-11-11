#ifndef QOSRNP_COVER_H
#define QOSRNP_COVER_H

#include <set>
#include <map>
#include <utility>
#include <initializer_list>

#include "miscellaneous.h"

namespace qosrnp {
    template <typename T, typename K>
    class Cover {
    public:
        typedef T        key_type;
        typedef K        value_type;

        Cover() = default;
        Cover(const std::map<key_type, std::set<value_type>>&, const std::set<value_type>&);
        Cover(std::map<key_type, std::set<value_type>>&&, std::set<value_type>&&);
        Cover(const Cover&);
        Cover(Cover&&);
        ~Cover() = default;

        Cover& operator=(const Cover&);
        Cover& operator=(Cover&&);

        bool operator==(const Cover&) const = delete;
        bool operator!=(const Cover&) const = delete;
        bool operator>(const Cover&) const = delete;
        bool operator>=(const Cover&) const = delete;
        bool operator<(const Cover&) const = delete;
        bool operator<=(const Cover&) const = delete;

        // insert a list of elements to the _set field.
        void insert_set(const std::initializer_list<value_type>&);
        // insert an element into the _set field.
        void insert_set(const value_type&);
        // insert elements of a given set into the correct set, identified by
        // given key, of the _family field. 
        void insert_family(const key_type&, const std::set<value_type>&);
        void insert_family(const key_type&, const std::initializer_list<value_type>&);
        void insert_family(const key_type&, const value_type&);

        std::map<key_type, std::set<value_type>> family() const { return _family; }
        std::set<value_type> set() const { return _set; }

        // search a minimum set cover of _set field using _family field,
        // using the greedy algorithm.
        std::set<key_type> minimum_set_cover() const;

    private:
        // return the key of the set with maximal size from given family.
        key_type max_set(std::map<key_type, std::set<value_type>>&) const;

    private:
        std::map<key_type, std::set<value_type>>    _family;
        std::set<value_type>                        _set;
    };

    template <typename T, typename K>
    Cover<T,K>::Cover(const std::map<T, std::set<K>>& f,
                      const std::set<K>& s)
    : _family(f), _set(s) {}

    template <typename T, typename K>
    Cover<T,K>::Cover(std::map<T, std::set<K>>&& f, std::set<K>&& s)
    : _family(std::move(f)), _set(std::move(s)) {}

    template <typename T, typename K>
    Cover<T,K>::Cover(const Cover&c)
    : _family(c._family), _set(c._set) {}

    template <typename T, typename K>
    Cover<T,K>::Cover(Cover&& c)
    : _family(std::move(c._family)), _set(std::move(c._set)) {}

    template <typename T, typename K>
    Cover<T,K>&
    Cover<T,K>::operator=(const Cover& c) {
        _family = c._family; _set = c._set;
        return *this;
    }

    template <typename T, typename K>
    Cover<T,K>&
    Cover<T,K>::operator=(Cover&& c) {
        _family = std::move(c._family);
        _set = std::move(c._set);
        return *this;
    }

    template <typename T, typename K>
    void
    Cover<T,K>::insert_set(const std::initializer_list<K>& il) {
        for (auto &i : il)
            _set.insert(i);
    }

    template <typename T, typename K>
    void
    Cover<T,K>::insert_set(const K& val) {
        _set.insert(val);
    }

    template <typename T, typename K>
    void
    Cover<T,K>::insert_family(const T& key, const std::set<K>& s) {
        for (auto &e : s)
            _family[key].insert(e);
    }

    template <typename T, typename K>
    void
    Cover<T,K>::insert_family(const T& key, const std::initializer_list<K>& il) {
        for (auto &i : il)
            _family[key].insert(i);
    }

    template <typename T, typename K>
    void
    Cover<T,K>::insert_family(const T& key, const K& val) {
        _family[key].insert(val);
    }

    template <typename T, typename K>
    T
    Cover<T,K>::max_set(std::map<T, std::set<K>>& f) const {
        bool flag = false;
        T m;

        for (auto &s : f) {
            if (!flag) {
                flag = true;
                m = s.first;
                continue;
            } else {
                if (s.second.size() > f[m].size())
                    m = s.first;
            }
        }
        return m;
    }

    template <typename T, typename K>
    std::set<T>
    Cover<T,K>::minimum_set_cover() const {
        std::set<T>                mi, tmp_s = _set;
        std::map<T, std::set<K>>   tmp_f = _family;
        T                          m;

        while (!tmp_s.empty()) {
            // find a set with maximal size from remaining
            // sets in the family.
            m = max_set(tmp_f);
            // delete each element in max from uncovered set.
            for (auto &e : tmp_f[m])
                tmp_s.erase(e);
            // delete each covered element from remaining cover sets.
            for (auto &f : tmp_f)
                if (f.first != m)
                    for (auto &e : tmp_f[m])
                        f.second.erase(e);
            // delete this max set from family.
            tmp_f.erase(m);
            // record this max set in the result.
            mi.insert(m);
            // if the whole family cannot guarantee a fully set cover,
            // return an empty set.
            if (tmp_f.empty() && !tmp_s.empty()) {
                mi.clear();
                return mi;
            }
        }
        return mi;
    }
}

#endif

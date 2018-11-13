#ifndef QOSRNP_COVER_H
#define QOSRNP_COVER_H

#include <set>
#include <map>
#include <utility>
#include <initializer_list>
#include <random>
#include <ctime>
#include <cstdlib>

#include "header.h"
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
        // search a minimum k-set cover of _set field using _family field,
        // using the greedy algorithm.
        std::set<key_type> k_set_cover(const size_type&);
        // search a random k-set cover of _set field using _family field,
        // using roulette wheel method.
        std::set<key_type> random_k_set_cover(std::default_random_engine&, const size_type&);

    private:
        // return the key of the set with maximal size from given family.
        key_type max_set(std::map<key_type, std::set<value_type>>&) const;
        // return the key of a random set selected using the roulette wheel method.
        key_type random_set(std::default_random_engine&, std::map<key_type, std::set<value_type>>&) const;
        // makde a k cover.
        bool make_k_cover(std::map<key_type, std::set<value_type>>&, const key_type&, const size_type&) const;
        // find all the elements only covered by a set with given key.
        std::set<value_type> necessary_elements(std::map<key_type, std::set<value_type>>&, const key_type&) const;
        // find all the elements that not covered only by a set with given key.
        std::set<value_type> optional_elements(std::map<key_type, std::set<value_type>>&, const key_type&) const;
        // check whether an element is only covered by given set.
        bool is_necessary(std::map<key_type, std::set<value_type>>&, const key_type&, const value_type&) const;
        bool make_random_k_cover(std::default_random_engine&, std::map<T, std::set<K>>&, 
                                 const T&, const size_type&) const;

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
        std::set<T>                mi;
        std::set<K>                tmp_s = _set;
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

    template <typename T, typename K>
    std::set<T>
    Cover<T,K>::k_set_cover(const size_type& k) {
        std::set<T>                  mi;
        std::set<K>                  tmp_s = _set;
        std::map<T, std::set<K>>     tmp_f = _family;
        T                            m;

        while (!tmp_s.empty()) {
            m = max_set(tmp_f);
            // make this set covers no more k elements.
            // If we cannot make this successfully, i.e.,
            // the number of elements covered only by this
            // set is larger than k, an empty set is returned
            // to notify that there is no feasible solution
            // to this instance.
            if (!make_k_cover(tmp_f, m, k)) {
                mi.clear();
                return mi;
            }
            
            for (auto &f : _family)
                if (f.first != m)
                    for (auto &e : tmp_f[m])
                        f.second.erase(e);

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

    template <typename T, typename K>
    bool
    Cover<T,K>::make_k_cover(std::map<T, std::set<K>>& f, 
                             const T& key, const size_type& k) const {
        size_type           num;
        std::set<K>         nec, opt;

        nec = necessary_elements(f, key);
        opt = optional_elements(f, key);

        if (nec.size() > k)
            return false;

        f[key].clear();
        for (auto &e : nec)
            f[key].insert(e);
        for (auto &e : opt)
            if (f[key].size() == k)
                break;
            else
                f[key].insert(e);

        return true;
    }

    template <typename T, typename K>
    std::set<K>
    Cover<T,K>::necessary_elements(std::map<T, std::set<K>>& f, const T& key) const {
        std::set<K>    necs;

        for (auto &e : f[key])
            if (is_necessary(f, key, e))
                necs.insert(e);
        return necs;
    }

    template <typename T, typename K>
    std::set<K>
    Cover<T,K>::optional_elements(std::map<T, std::set<K>>& f, const T& key) const {
        std::set<K>   opts;

        for (auto &e : f[key])
            if (!is_necessary(f, key, e))
                opts.insert(e);
        return opts;
    }

    template <typename T, typename K>
    bool
    Cover<T,K>::is_necessary(std::map<T, std::set<K>>& f, const key_type& key,
                             const value_type& val) const {
        for (auto &s : f)
            if (s.first != key)
                for (auto &e : s.second)
                    if (e == val)
                        return false;
        return true;
    }

    template <typename T, typename K>
    std::set<T>
    Cover<T,K>::random_k_set_cover(std::default_random_engine& en, 
                                   const size_type& k) {
        std::set<T>                  mi;
        std::set<K>                  tmp_s = _set;
        std::map<T, std::set<K>>     tmp_f = _family;
        T                            m;

        while (!tmp_s.empty()) {
            m = random_set(en, tmp_f);
            // make this set covers no more k elements.
            // If we cannot make this successfully, i.e.,
            // the number of elements covered only by this
            // set is larger than k, an empty set is returned
            // to notify that there is no feasible solution
            // to this instance.
            if (!make_random_k_cover(en, tmp_f, m, k)) {
                mi.clear();
                return mi;
            }
            
            for (auto &f : _family)
                if (f.first != m)
                    for (auto &e : tmp_f[m])
                        f.second.erase(e);

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

    template <typename T, typename K>
    T
    Cover<T,K>::random_set(std::default_random_engine& en,
                           std::map<T, std::set<K>>& f) const {
        size_type size = 0;

        for (auto &s : f)
            size += s.second.size();

        if (size == 0)
            throw std::range_error("no feasible set!");
        
        std::uniform_int_distribution<int> dis(0, size);
        int    r = dis(en), i = 0;

        for (auto &s : f) {
            if (s.second.size() == 0) continue;
            if (i <= r && r <= i + s.second.size())
                return s.first;
            i += s.second.size();
        }

    }

    template <typename T, typename K>
    bool
    Cover<T,K>::make_random_k_cover(std::default_random_engine& en,
                                    std::map<T, std::set<K>>& f, 
                                    const T& key, const size_type& k) const {
        size_type           num;
        std::set<K>         nec, opt;
        int                 i, r;

        nec = necessary_elements(f, key);
        opt = optional_elements(f, key);

        if (nec.size() > k)
            return false;

        f[key].clear();
        for (auto &e : nec)
            f[key].insert(e);

        while (f[key].size() < k && !opt.empty()) {
            i = 0; r = rand_range(en, 0, opt.size() - 1);
            for (auto &e : opt)
                if (i++ == r) {
                    f[key].insert(e);
                    opt.erase(e);
                    break;
                }
        }

        return true;
    }
}

#endif

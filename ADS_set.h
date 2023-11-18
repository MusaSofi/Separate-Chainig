#ifndef ADS_SET_H
#define ADS_SET_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename Key, size_t N = 7>
class ADS_set
{
public:
  class Iterator;
  using value_type = Key;
  using key_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using const_iterator = Iterator;
  using iterator = const_iterator;
  // using key_compare = std::less<key_type>;                         // B+-Tree
  using key_equal = std::equal_to<key_type>; // Hashing
  using hasher = std::hash<key_type>;        // Hashing

private:
  struct Element
  {
    key_type key;
    Element *head{nullptr};
    Element *next{nullptr};

    ~Element()
    {
      delete head;
      delete next;
    }
  };

  Element *table{nullptr};
  size_type current_size;
  size_type table_size;
  double max_proz{0.7};

  // methoden
  size_type h(const key_type &key) const { return hasher{}(key) % table_size; }

  void reserve(size_type n)
  {
    if (table_size * max_proz <= n)
    {
      size_type new_table_size{table_size};
      while (new_table_size * max_proz < n)
        new_table_size = new_table_size * 2 + 1;
      rehash(new_table_size);
    }
  }

  void rehash(size_type n)
  {
    std::vector<key_type> help;
    for (size_type idx = 0; idx < table_size; ++idx)
    {
      if (table[idx].head != nullptr)
      {
        Element *it = table[idx].head;
        do
        {
          help.push_back(it->key);
          it = it->next;
        } while (it != nullptr);
      }
    }
    delete[] table;
    table = new Element[n];
    table_size = n;
    current_size = 0;
    for (const auto elem : help)
    {
      add(elem);
    }
  }

  Element *add(key_type k)
  {
    size_type idx{h(k)};
    Element *help;
    help = new Element;
    help->next = table[idx].head;
    help->key = k;
    table[idx].head = help;
    ++current_size;
    return help; // adrsse von table an der Position idx
  }

  Element *locate(const key_type &key) const
  {
    size_type idx{h(key)};
    if (table[idx].head == nullptr)
    {
      return nullptr;
    }
    else
    {
      Element *it = table[idx].head;
      if (key_equal{}(it->key, key))
        return it;
      while (it->next != nullptr)
      {
        it = it->next;
        if (key_equal{}(it->key, key))
          return it;
      }
      return nullptr;
    }
  }

public:
  // leeres Konstruktor
  ADS_set() : table{new Element[N]}, current_size{0}, table_size{N} {}
  // Konstruktor mit List
  ADS_set(std::initializer_list<key_type> ilist) : ADS_set{}
  {
    insert(ilist);
  }

  template <typename InputIt>
  ADS_set(InputIt first, InputIt last) : ADS_set{}
  {
    insert(first, last);
  }

  // kopiekonstruktor
  ADS_set(const ADS_set &other)
  {
    clear();
    for (auto elem : other)
    {
      insert(elem);
    }
  }

  ~ADS_set()
  {
    delete[] table;
  }

  ADS_set &operator=(const ADS_set &other)
  {
    ADS_set tmp{other};
    swap(tmp);
    return *this;
  }
  ADS_set &operator=(std::initializer_list<key_type> ilist)
  {
    ADS_set tmp{ilist};
    swap(tmp);
    return *this;
  }

  size_type size() const
  {
    return current_size;
  }
  bool empty() const
  {
    return current_size == 0;
  }

  // insert mit List
  void insert(std::initializer_list<key_type> ilist)
  {
    insert(ilist.begin(), ilist.end());
  }
  // insert mit einem Wert
  std::pair<iterator, bool> insert(const key_type &key)
  {
    // if wert scho da ist
    if (auto e{locate(key)})
      return {iterator{table, table_size, e, h(key)}, false};

    reserve(current_size + 1);
    auto addedElem = add(key);
    return {iterator{table, table_size, addedElem, h(key)}, true};
  }

  template <typename InputIt>
  void insert(InputIt first, InputIt last)
  {
    for (auto it{first}; it != last; ++it)
    {
      if (count(*it) == 0)
      {
        reserve(current_size + 1);
        add(*it);
      }
    }
  }

  void clear()
  {
    delete[] table;
    table_size = N;
    current_size = 0;
    table = new Element[table_size];
  }
  size_type erase(const key_type &key)
  {
    size_type idx{h(key)};
    if (table[idx].head == nullptr)
    {
      return 0;
    }
    else
    {
      Element *help = table[idx].head;
      if (key_equal{}(help->key, key))
      {
        table[idx].head = table[idx].head->next;
        help->next = nullptr;
        delete help;
        --current_size;
        return 1;
      }
      Element *beforHelp;
      while (help->next != nullptr)
      {
        beforHelp = help;
        help = help->next;
        if (key_equal{}(help->key, key))
        {
          beforHelp->next = help->next;
          help->next = nullptr;
          delete help;
          --current_size;
          return 1;
        }
      }
    }
    return 0;
  }
  size_type count(const key_type &key) const
  {
    if (locate(key) == nullptr)
      return 0;
    else
      return 1;
  }
  iterator find(const key_type &key) const
  {
    if (auto e{locate(key)})
      return iterator{table, table_size, e, h(key)};
    return end();
  }

  void swap(ADS_set &other)
  {
    using std::swap;
    swap(table, other.table);
    swap(table_size, other.table_size);
    swap(current_size, other.current_size);
    swap(max_proz, other.max_proz);
  }

  const_iterator begin() const
  {
    if (current_size == 0)
      return end();
    else
      return const_iterator{table, table_size, nullptr, 0, current_size};
  }
  const_iterator end() const
  {
    return const_iterator{nullptr};
  }

  const_iterator z() const
  {
    if (current_size == 0)
    {
      return end();
    }
    else
      return const_iterator{table, table_size, nullptr, 0, current_size, true};
  }

  void dump(std::ostream &o = std::cerr) const
  {
    o << "table_size = " << table_size << ", current_size = " << current_size << std::endl;
    for (size_type idx = 0; idx < table_size; ++idx)
    {
      if (table[idx].head != nullptr)
      {
        Element *it = table[idx].head;
        o << idx << ":";
        do
        {
          o << " --> " << it->key;
          it = it->next;
        } while (it != nullptr);
        o << std::endl;
      }
      else
      {
        o << idx << ": " << std::endl;
      }
    }
  }

  friend bool operator==(const ADS_set &lhs, const ADS_set &rhs)
  {
    if (lhs.current_size != rhs.current_size)
      return false;
    for (const auto &k : lhs)
    {
      if (!rhs.count(k))
        return false;
    }
    return true;
  }
  friend bool operator!=(const ADS_set &lhs, const ADS_set &rhs)
  {
    return !(lhs == rhs);
  }
};

template <typename Key, size_t N>
class ADS_set<Key, N>::Iterator
{
  size_t counter = 0;

  bool normal;

  Element *verticalTable;
  size_t table_size;
  Element *pos;
  size_t idx;
  size_t current_size;
  bool isSpezial;

  void skip()
  {
    while (verticalTable[idx].head == nullptr && idx < table_size)
    {
      idx++;
    }
  }

public:
  using value_type = Key;
  using difference_type = std::ptrdiff_t;
  using reference = const value_type &;
  using pointer = const value_type *;
  using iterator_category = std::forward_iterator_tag;
  ///////alt
  Iterator(Element *verticalTable = nullptr, size_t table_size = 0, Element *pos = nullptr, size_t idx = 0, size_t current_size = 0, bool isSpezial = false) : verticalTable{verticalTable}, table_size{table_size}, pos{pos}, idx{idx}, current_size{current_size}, isSpezial{isSpezial}
  {
    ++counter;
    if (this->pos == nullptr)
    {
      if (this->verticalTable != nullptr)
      {
        if (this->verticalTable[this->idx].head == nullptr)
        {
          skip();
        }
        this->pos = this->verticalTable[this->idx].head;
      }
      else
      {
        pos = nullptr;
      }
    }
  }

  ///////neu
  /*
   Iterator(Element *verticalTable = nullptr,size_t table_size = 0, Element *pos = nullptr, size_t idx = 0) :current_size{current_size},verticalTable{verticalTable},table_size{table_size},  pos{pos}, idx{idx}
  {
    ++counter;
    if (this->pos == nullptr)
    {
      if (this->verticalTable != nullptr)
      {
        if (this->verticalTable[this->idx].head == nullptr)
        {
          skip();
        }
        this->pos = this->verticalTable[this->idx].head;
      }
      else
      {
        pos = nullptr;
      }
    }
  }*/
  // liefert wert

  reference operator*() const
  {
    return pos->key;
  }
  // liefert adresse
  pointer operator->() const
  {
    return &pos->key;
  }

  // prefix

  Iterator &operator++()
  {

    if (pos == nullptr)
    {
      return *this;
    }
    if (isSpezial)
    {

      do
      {
        ++counter;
        if(counter > current_size){
          pos = nullptr;
          return *this;
        }
        if (pos->next != nullptr)
        {
          pos = pos->next;
        }
        else
        {
          ++idx;
          skip();
          pos = verticalTable[idx].head;
        }

      } while (counter % 3 == 0 && counter < current_size);

      return *this;
    }
    else
    {
      if (pos->next != nullptr)
      {
        pos = pos->next;
      }
      else
      {
        ++idx;
        skip();
        if (idx == table_size)
        {
          pos = nullptr;
          return *this;
        }
        pos = verticalTable[idx].head;
      }

      return *this;
    }
  }

  /*
  Iterator &operator++()
  {
    if (pos == nullptr)
      return *this;

    if (pos->next != nullptr)
    {
      pos = pos->next;
    }
    else
    {
      ++idx;
      skip();
      if (idx == table_size)
      {
        pos = nullptr;
        return *this;
      }
      pos = verticalTable[idx].head;
    }

    return *this;
  }
  */
  // postfix
  Iterator operator++(int)
  {
    auto rc{*this};
    ++*this;
    return rc;
  }
  friend bool operator==(const Iterator &lhs, const Iterator &rhs)
  {
    return lhs.pos == rhs.pos;
  }
  friend bool operator!=(const Iterator &lhs, const Iterator &rhs)
  {
    return !(lhs == rhs);
  }
};

template <typename Key, size_t N>
void swap(ADS_set<Key, N> &lhs, ADS_set<Key, N> &rhs) { lhs.swap(rhs); }

#endif // ADS_SET_H
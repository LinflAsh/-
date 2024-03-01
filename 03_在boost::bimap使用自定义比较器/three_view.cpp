#include <boost/bimap.hpp>
#include <iostream>
#include <string>

typedef boost::bimap<int, std::string> bm_type;
bm_type bm;

void relations_view() {
    bm.clear();
    // Remember that bm alone can be used as a set of relations.
    // We can insert elements or iterate over them using this view.
    bm.insert(bm_type::value_type(1, "one"));
    bm.insert(bm_type::value_type(2, "two"));

    std::cout << "There are " << bm.size() << "relations" << std::endl;

    for (bm_type::const_iterator iter = bm.begin(), iend = bm.end(); iter != iend; ++iter) {
        // iter->left  : data : int
        // iter->right : data : std::string

        std::cout << iter->left << " <--> " << iter->right << std::endl;
    }
}

void left_view() {
    // bm.left works like a std::map<int, std::string>.We use it in the same way we will use a standard map.

    // The type of bm.left is bm_type::left_map and the type of bm.right is bm_type::right_map
    typedef bm_type::left_map::const_iterator left_const_iterator;
    for (left_const_iterator left_iter = bm.left.begin(), iend = bm.left.end(); left_iter != iend; ++left_iter) {
        // left_iter->first  : key  : int
        // left_iter->second : data : std::string

        std::cout << left_iter->first << " --> " << left_iter->second << std::endl;
    }

    // bm_type::left_-type- can be used as a shortcut for the more verbose bm_type::left_map::-type-
    bm_type::left_const_iterator left_iter = bm.left.find(2);
    assert(left_iter->second == "two");

    // This line produces the same effect of bm.insert( bm_type::value_type(3,"three") );
    bm.left.insert(bm_type::left_value_type(3, "three"));
}

void right_view() {
    // bm.right works like a std::map<std::string, int>. It is important to note that the key is the first type and the
    // data is the second one, exactly as with standard maps.
    bm_type::right_const_iterator right_iter = bm.right.find("two");

    // right_iter->first  : key  : std::string
    // right_iter->second : data : int

    assert(right_iter->second == 2);

    assert(bm.right.at("one") == 1);

    bm.right.erase("two");

    typedef bm_type::right_map::const_iterator right_const_iterator;
    // This line produces the same effect of bm.insert( bm_type::value_type(4,"four") );
    bm.right.insert(bm_type::right_value_type("four", 4));
    for (right_const_iterator right_iter = bm.right.begin(), iend = bm.right.end(); right_iter != iend; ++right_iter) {
        std::cout << right_iter->second << " <-- " << right_iter->first << std::endl;
    }
}

int main() {
    relations_view();
    left_view();
    right_view();
    return 0;
}
// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <utility>
#include <vector>
#include <set>
#include <boost/tokenizer.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/tree_node/associative_node.hpp>
#include <boost/tree_node/iterator/breadth_first.hpp>
#include <boost/test/minimal.hpp>

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <boost/move/move.hpp>
#endif

//[test__inverted_index__type_definitions
typedef std::set<std::size_t> Indices;
typedef boost::tree_node::associative_node<std::string,Indices> RadixTrieNode;
//]

//[test__inverted_index__insert
class InvertedIndex
{
    RadixTrieNode radix_root_node;

 public:
    void insert(std::string const& entry, std::size_t index)
    {
        boost::tokenizer<> word_parser(entry);

        for (
            boost::tokenizer<>::iterator word_itr = word_parser.begin();
            word_itr != word_parser.end();
            ++word_itr
        )
        {
            RadixTrieNode::pointer p = &radix_root_node;

            for (
                std::string::const_iterator itr_to_char = word_itr->begin();
                itr_to_char != word_itr->end();
                ++itr_to_char
            )
            {
                std::string letter = "";

                p = &dereference_iterator(p->emplace(letter += *itr_to_char));
            }

            get(*p, boost::tree_node::data_key()).insert(index);
        }
    }
    //]

    //[test__inverted_index__flatten
    void flatten()
    {
        for (
            boost::tree_node::breadth_first_iterator<RadixTrieNode> tree_itr(
                radix_root_node
            );
            tree_itr;
            ++tree_itr
        )
        {
            for (
                RadixTrieNode::iterator child_itr = tree_itr->begin();
                child_itr != tree_itr->end();
                ++child_itr
            )
            {
                while (1 == dereference_iterator(child_itr).size())
                {
                    std::string key = child_itr->first + dereference_iterator(
                        child_itr
                    ).begin()->first;
                    RadixTrieNode merger(
//<-
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//->
                        boost::move(
//<-
#endif
//->
                            dereference_iterator(
                                dereference_iterator(child_itr).begin()
                            )
//<-
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//->
                        )
//<-
#endif
//->
                    );

                    tree_itr->erase(child_itr->first);
//<-
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
                    child_itr = tree_itr->insert(key, merger);
#else
//->
                    child_itr = tree_itr->insert(key, boost::move(merger));
//<-
#endif
//->
                }
            }
        }
    }
    //]

    //[test__inverted_index__find
    Indices find(std::string const& word) const
    {
        Indices result;

        if (word.empty())
        {
            return result;
        }

        RadixTrieNode::const_pointer p = &radix_root_node;

        for (std::string prefix = ""; !p->empty();)
        {
            std::string suffix(word, prefix.size(), word.size());
            RadixTrieNode::const_iterator node_itr = p->lower_bound(suffix);

            if (!(node_itr == p->end()) && (suffix == node_itr->first))
            {
                result = get(
                    dereference_iterator(node_itr)
                  , boost::tree_node::data_key()
                );
                break;
            }
            else if (node_itr == p->begin())
            {
                break;
            }
            else
            {
                std::string next_prefix(suffix, 0, (--node_itr)->first.size());

                if (next_prefix == node_itr->first)
                {
                    prefix += next_prefix;
                    p = &dereference_iterator(node_itr);
                }
                else
                {
                    break;
                }
            }
        }

        return result;
    }
};
//]

//[test__inverted_index__initialize
int test_main(int argc, char** argv)
{
    std::vector<std::string> database(3);

    database[0] = "it is what it is";
    database[1] = "what is it";
    database[2] = "it is a banana";

    InvertedIndex inverted_index;

    for (std::size_t i = 0; i < database.size(); ++i)
    {
        inverted_index.insert(database[i], i);
    }

    inverted_index.flatten();
    //]

    //[test__inverted_index__use
    Indices indices = inverted_index.find("a");

    BOOST_CHECK(1 == indices.size());
    BOOST_CHECK(2 == *indices.begin());
    indices = inverted_index.find("banana");
    BOOST_CHECK(1 == indices.size());
    BOOST_CHECK(2 == *indices.begin());
    indices.insert(0);
    indices.insert(1);
    BOOST_CHECK(boost::range::equal(indices, inverted_index.find("is")));
    BOOST_CHECK(boost::range::equal(indices, inverted_index.find("it")));
    indices.erase(2);
    BOOST_CHECK(boost::range::equal(indices, inverted_index.find("what")));

    return 0;
}
//]

